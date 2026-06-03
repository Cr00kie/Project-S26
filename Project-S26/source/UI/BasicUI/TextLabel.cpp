#include "TextLabel.h"
#include "../../Tools/ServiceLocator.h"
#include "../../Resources/ResourceManager.h"
#include "../../Resources/Font.h"
#include <cassert>
#include <sstream>
#include <numbers>
#include <cmath>
#include <algorithm>

TextLabel::TextLabel(
	float x, float y,
	const std::string& text,
	const std::string& fontId,
	float boxW, float boxH,
	float scale, TextAlign alignment, float rotation,
	float lineSpacing, float wordSpacing, float letterSpacing,
	int visibleGlyphCount
)
	: UIElement(x, y, boxW, boxH, rotation, scale, scale)
	, m_TextAlignment(alignment)
	, m_fLineSpacing(lineSpacing), m_fWordSpacing(wordSpacing), m_fLetterSpacing(letterSpacing)
	, m_visibleGlyphCount(visibleGlyphCount)
{
	setFont(fontId);
	setText(text);

	// If no text box size specified, try to set it automatically
	if (boxW == -1 && boxH == -1) m_font->getTextSize(m_text, m_fW, m_fH);
}

TextLabel::~TextLabel()
{
	// Release font
	ServiceLocator::get<ResourceManager>().ReleaseResource<Font>(m_sFontID);
}

void
TextLabel::setFont(const std::string& fontId) {
	m_sFontID = fontId;
	// Request font
	ResourceManager& rm = ServiceLocator::get<ResourceManager>();
	m_font = rm.GetResource<Font>(m_sFontID);
}

const std::string& TextLabel::getFont() const
{
	return m_sFontID;
}

void
TextLabel::setText(const std::string& newText) {
	// Preserve "show all glyphs" semantics across text changes.
	const bool wasShowingAllGlyphs = (m_visibleGlyphCount < 0 || m_visibleGlyphCount >= m_glyphCount);

	m_glyphCount = 0;

	m_text = newText;

	TextStyle currentGlyphStyle; // default is white text

	// Clear previously parsed text
	m_tokenizedText.clear();

	TextLine currentLine;

	// Parse text to tokens
	for (std::size_t i = 0; i < m_text.size(); ++i)
	{
		switch (m_text[i])
		{
			// Parse style marks
		case '<': {
			std::size_t styleMarkEnd = m_text.find('>', i + 1);
			assert(styleMarkEnd != std::string::npos);
			std::string markContent = m_text.substr(i + 1, styleMarkEnd - i - 1);
			i = styleMarkEnd;

			ParseStyleMark(markContent, currentGlyphStyle);
		} break;
				// Parse space glyphs separetly to use as word separators
		case ' ': {
			currentLine.tokens.push_back({ TokenType::Space, unsigned char(' ') });

			// Check if next word fits
			float nextWordWidth = CalculateNextWordSize(int(i), currentLine.tokens);
			// If next word doesn't fit
			if (currentLine.width + nextWordWidth > m_fW)
			{
				// Make new line
				m_tokenizedText.push_back(currentLine);
				currentLine.tokens.clear();
				currentLine.width = 0;
			}
			else
			{
				// Only add space width if we haven't wrapped the next word
				currentLine.width += m_font->getGlyph(unsigned char(' '))->w * m_fWordSpacing;
			}
		}break;
				//Parse new line, it will always make a new line, 
				//even if the next word fits, to respect the explicit new line
		case '\n': {
			if (!currentLine.tokens.empty()) {
				m_tokenizedText.push_back(currentLine);
				currentLine.tokens.clear();
				currentLine.width = 0;
			}
		}break;
				 // Parse the rest of glyphs
		default: {
			unsigned char c = m_text[i];
			currentLine.tokens.push_back({ TokenType::Glyph, c, currentGlyphStyle });
			currentLine.width += m_font->getGlyph(c)->w * m_fLetterSpacing;
			++m_glyphCount;
		}break;
		}

	}

	// last word doesnt depend on a space at the end with this
	if (!currentLine.tokens.empty())
	{
		m_tokenizedText.push_back(currentLine);
	}
	if (wasShowingAllGlyphs) {
		m_visibleGlyphCount = -1;
	}
	else if (m_visibleGlyphCount > m_glyphCount) {
		m_visibleGlyphCount = m_glyphCount;
	}

	RecalculateHeight();
}

void TextLabel::setVisibleGlyphCount(int visibleGlyphCount)
{
	m_visibleGlyphCount = std::clamp(visibleGlyphCount, -1, m_glyphCount);
}

int TextLabel::getClampedVisibleGlyphCount() const
{
	if (m_visibleGlyphCount < 0) return m_glyphCount;
	return std::min(m_visibleGlyphCount, m_glyphCount);
}

int TextLabel::getVisibleGlyphCount() const
{
	return getClampedVisibleGlyphCount();
}

void TextLabel::showAllGlyphs()
{
	m_visibleGlyphCount = m_glyphCount;
}

void TextLabel::ParseStyleMark(const std::string& params, TextLabel::TextStyle& style)
{
	// Get style mark inner text as a stream
	std::stringstream styleMark(params);
	char styleMarkType;
	styleMark >> styleMarkType;

	switch (styleMarkType)
	{
		// Handle color style mark
	case 'c': {
		// Text color <c 255 000 000 255> - red text
		int r, g, b, a;
		styleMark >> r >> g >> b >> a;
		style.color = { Uint8(r), Uint8(g), Uint8(b), Uint8(a) };
	}break;
			// Flipped letters <f> - letter now appear flipped
	case 'f': {
		style.isFlipped = !style.isFlipped;
	}break;
			// Rotated letters <r 45> - letters appear rotated 45 degrees
	case 'r': {
		styleMark >> style.rotation;
	}break;
			// Resize letters <s 1.5> - letters appear scaled by x1.5
	case 's': {
		styleMark >> style.scale;
	}break;
	default: {
		// Unkown style mark
		assert(false);
	}
	}
}

float
TextLabel::getLineStartX(const TextLine& line) const {
	switch (m_TextAlignment) {
	case TextAlign::Left:   return 0;
	case TextAlign::Center: return (m_fW - line.width * m_scale.getX()) * 0.5f;
	case TextAlign::Right:  return  (m_fW - line.width * m_scale.getX());
	}
	return 0;
}

float
TextLabel::getLineStartY(const size_t lineIndex) const {
	float startY = 0.f;

	if (m_TextAlignment == TextAlign::Center) {
		startY = (m_fH - m_fTotalHeight) * 0.5f;
	}

	for (size_t i = 0; i < lineIndex; ++i) {
		float lineHeight = 0.f;

		for (const TextToken& token : m_tokenizedText[i].tokens) {
			if (token.type == TokenType::Glyph) {
				float glyphHeight =
					m_font->getGlyph(token.glyph)->h *
					m_scale.getY() *
					token.style.scale;

				lineHeight = std::max(lineHeight, glyphHeight);
			}
		}

		if (lineHeight == 0.f) {
			lineHeight = m_font->getGlyph(' ')->h * m_scale.getY();
		}

		startY += lineHeight * m_fLineSpacing;
	}

	return startY;
}

SDL_FRect
TextLabel::computeStyledCaret(const SDL_FRect& caret, const TextStyle& style,
	float rotCos, float rotSin) const {
	SDL_FRect sc = caret;

	sc.x += caret.w * style.scale / 2.f - m_fW / 2.f;
	sc.y += caret.h / 2.f - m_fH / 2.f;

	sc.w *= style.scale;
	sc.h *= style.scale;

	sc.y -= caret.h * (style.scale - 1.f) / 2.f;

	float oldX = sc.x;
	sc.x = sc.x * rotCos - sc.y * rotSin;
	sc.y = oldX * rotSin + sc.y * rotCos;

	return sc;
}

void TextLabel::renderGlyph(float x, float y, const TextToken& token, const SDL_FRect& styledCaret, const SDL_FRect& baseCaret, float rotation) const
{
	SDL_FlipMode flip = token.style.isFlipped
		? SDL_FLIP_HORIZONTAL
		: SDL_FLIP_NONE;

	m_font->renderCharacter(
		token.glyph,
		{
			styledCaret.x - styledCaret.w / 2.f + x,
			styledCaret.y - styledCaret.h / 2.f + y,
			styledCaret.w, styledCaret.h
		},
		flip,
		token.style.rotation + rotation,
		token.style.color
	);
}

void
TextLabel::render(const Mat3f& parentTransform) {
	if (!m_font || m_text.empty()) return;
	if (m_fW <= 0 || m_fH <= 0) return;

	Mat3f globalTransform = parentTransform * m_transform;
	Vec2f position = globalTransform.getTranslation();
	float globX = position.getX();
	float globY = position.getY();
	float rotation = globalTransform.getRotation();
	float angleRad = globalTransform.getAngle();

	float rotCos = std::cos(angleRad);
	float rotSin = std::sin(angleRad);

	SDL_FRect caret = { 0, 0,
		m_font->getGlyph(m_text[0])->w * m_scale.getX(),
		m_font->getGlyph(m_text[0])->h * m_scale.getY() };

	const int visibleGlyphLimit = getClampedVisibleGlyphCount();
	bool stopRendering = false;

	for (size_t lineIdx = 0; lineIdx < m_tokenizedText.size(); ++lineIdx) {
		TextLine& line = m_tokenizedText[lineIdx];

		if (stopRendering) break;
		caret.x = getLineStartX(line);
		caret.y = getLineStartY(lineIdx);

		for (TextToken& token : line.tokens) {
			caret.w = m_font->getGlyph(token.glyph)->w * m_scale.getX();
			caret.h = m_font->getGlyph(token.glyph)->h * m_scale.getY();
			if (token.type == TokenType::Space) {
				caret.x += caret.w * m_fWordSpacing;
				continue;
			}

			SDL_FRect styledCaret = computeStyledCaret(caret, token.style, rotCos, rotSin);

			renderGlyph(globX, globY, token, styledCaret, caret, rotation);

			caret.x += caret.w * token.style.scale * (token.type == TokenType::Glyph
				? m_fLetterSpacing : m_fWordSpacing);
		}
	}

	UIElement::render(parentTransform);
}

float TextLabel::CalculateNextWordSize(int wordStartIdx, const std::vector<TextToken>& tokens)
{
	float width = 0;
	int j = wordStartIdx + 1;
	// Aggregate tokens width until a space is found
	while (j < tokens.size() && tokens[j].type != TokenType::Space)
	{
		if (tokens[j].type == TokenType::Glyph)
		{
			width += m_font->getGlyph(tokens[j].glyph)->w * m_fLetterSpacing;
		}
		++j;
	}
	return width;
}

void TextLabel::RecalculateHeight()
{
	if (m_tokenizedText.empty()) {
		m_fTotalHeight = 0.f;
		return;
	}

	float totalHeight = 0.f;

	for (const TextLine& line : m_tokenizedText) {
		// Find the maximum glyph height in this line
		float maxLineHeight = 0.f;

		for (const TextToken& token : line.tokens) {
			if (token.type == TokenType::Glyph) {
				float glyphHeight = m_font->getGlyph(token.glyph)->h * m_scale.getY() * token.style.scale;
				maxLineHeight = std::max(maxLineHeight, glyphHeight);
			}
		}

		// If line has no glyphs, use height of space character
		if (maxLineHeight == 0.f) {
			maxLineHeight = m_font->getGlyph(' ')->h * m_scale.getY();
		}

		totalHeight += maxLineHeight;
	}

	// Add spacing between lines (lineCount - 1) times
	if (m_tokenizedText.size() > 1) {
		float maxLineHeight = 0.f;
		for (const TextLine& line : m_tokenizedText) {
			for (const TextToken& token : line.tokens) {
				if (token.type == TokenType::Glyph) {
					float glyphHeight = m_font->getGlyph(token.glyph)->h * m_scale.getY() * token.style.scale;
					maxLineHeight = std::max(maxLineHeight, glyphHeight);
				}
			}
		}
		if (maxLineHeight == 0.f) {
			maxLineHeight = m_font->getGlyph(' ')->h * m_scale.getY();
		}
		totalHeight += maxLineHeight * (m_fLineSpacing - 1.f) * (m_tokenizedText.size() - 1);
	}

	m_fTotalHeight = totalHeight;
}

void TextLabel::setColor(int r, int g, int b, int a)
{
	if (m_text.empty())
		return;

	std::ostringstream oss;
	oss << "<c "
		<< r << " "
		<< g << " "
		<< b << " "
		<< a << ">";

	oss << m_text;

	setText(oss.str());
}

