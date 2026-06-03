#pragma once
#include "../UIElement.h"
#include "../../Resources/Font.h"

#include <string>
#include <memory>
#include <optional>
#include <span>
#include <vector>

class Font;

// A UI element that renders text with support for basic markup styles (color, flip, rotation, scale)
class TextLabel :
    public UIElement
{
private:
    // Text style (changed with markup styles)
	// It holds the current color, flip, rotation and scale of the glyph to render
    struct TextStyle
    {
        SDL_Color color{ 255, 255, 255, 255 };
        bool isFlipped = false;
        float rotation = 0;
        float scale = 1;
    };

	// Token type, it can be a glyph or a space
    enum class TokenType
    {
        Glyph,
        Space
    };

    // Token that holds preprocessed information of the glyph to render
    struct TextToken
    {
        TokenType type;
        unsigned char glyph;
        TextStyle style;
    };

	// A line of text, it holds the tokens in the line and the total width of the line
    struct TextLine
    {
        std::vector<TextToken> tokens;
        float width = 0;
    };
    
public:
	// Text alignment options for the text label
    enum class TextAlign
    {
        Left,
        Center,
        Right
    };

    // Creates an TextLabel
    // Arguments:
    //		- container: the container that will contain the TextLabel (scene or another container)
    //		- x: x position of the TextLabel
    //		- y: y position of the TextLabel
    //      - text: text to be displayed
    //      - font: asset ID of the font to be used 
    //		- boxW: width of the box that will contain the text (important for wrapping)  |
    //		- boxH: height of the box that will contain the text (important for wrapping) |-- If neither is set a box to contain the text in one line is calculated
    //		- scale: scale of the text
    //      - alingment: alingment of the text inside the box (LEFT, CENTER or RIGHT)
    //      - rotation: rotation of the text
    //      - lineSpacing: space between lines
    //      - wordSpacing: space between words
    //      - letterSpacing: space between letters
    TextLabel(
        float x,
        float y,
        const std::string& text,
        const std::string& fontId,
        float boxW = -1, float boxH = -1,
        float scale = 1.0f, TextAlign alignment = TextAlign::Left, float rotation = 0,
        float lineSpacing = 1, float wordSpacing = 1, float letterSpacing = 1,
        int visibleGlyphCount = -1
    );
    ~TextLabel();

	// Render the text label,
    // it will render each glyph in the text with its corresponding style
    void render(const Mat3f& parentTransform) override;

	// Set the text of the text label,
    void setText(const std::string& newText);
  // Set how many glyphs from the beginning are rendered (-1 means all glyphs)
  void setVisibleGlyphCount(int visibleGlyphCount);
  // Get current visible glyph limit (clamped, -1 is returned as total glyph count)
  int getVisibleGlyphCount() const;
  // Make every glyph visible
  void showAllGlyphs();
  // Get total number of glyph tokens in current text
  int getGlyphCount() const { return m_glyphCount; }
  // Set the text of the text label
  void setFont(const std::string& fontId);
  // Get the text of the text label
  const std::string& getFont() const;

  // Get the Height after wrapping. It can be more or less than default height, depending on glyph size
  float getTotalHeight() const { return m_fTotalHeight; }
  void setColor(int r, int g, int b, int a);

private:
  int m_glyphCount = 0;
  int m_visibleGlyphCount = -1;
  int getClampedVisibleGlyphCount() const;

	float getLineStartX(const TextLine& line) const;
    float getLineStartY(const size_t lineIndex) const;
	SDL_FRect computeStyledCaret(const SDL_FRect& caret, const TextStyle& style,
								float rotCos, float rotSin) const; 
	void renderGlyph(float x, float y,
					const TextToken& token, const SDL_FRect& styledCaret, const SDL_FRect& baseCaret, float rotation) const;
	
	// The width and height of the text label, it will be used to wrap the text if it's greater than 0
    float                   m_fTotalHeight;
	// The font used to render the text
    Font*                   m_font;
	// The ID of the font used to render the text,
    // it will be used to get the font from the resource manager
    std::string             m_sFontID;
	// The text to render
    std::string             m_text;
	// The preprocessed text,
    // it holds the tokens of each line and the total width of each line
    std::vector<TextLine>  m_tokenizedText;

	// The text alignment of the text label,
    // it will be used to align the text when rendering
    TextAlign               m_TextAlignment;
	// The spacing between letters, lines and words
    float                   m_fLetterSpacing;
    float                   m_fLineSpacing;
    float                   m_fWordSpacing;

	// Preprocess the text to get the tokens and the total width of each line
    float CalculateNextWordSize(int i, const std::vector<TextToken>& tokens);
	// Recalculate the height based on the number of lines and max glyph height in each line
	void RecalculateHeight();
	
	// Parse the markup styles in the text and update the current glyph style accordingly
    void ParseStyleMark(const std::string& params, TextLabel::TextStyle& currentGlyphStyle);
};