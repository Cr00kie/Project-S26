#pragma once

#include "../Entities/Registry.h"
#include "../Components/Text.h"
#include "../Components/TextLayout.h"
#include "../Components/Transform.h"
#include "../Components/Bounds.h"

class TextLayoutSystem
{
public:
	void update(Registry& reg)
	{
        for (Entity e : reg.view<Text>())
        {
            // Check the entity has all components needed
            if (!reg.has<TextLayout>(e)) reg.emplace<TextLayout>(e);
            if (!reg.has<Bounds>(e)) continue; // no box, nothing to wrap against

            // Get components needed
            Text& text = reg.get<Text>(e);
            Bounds& bounds = reg.get<Bounds>(e);
            TextLayout& layout = reg.get<TextLayout>(e);

            // Check if text needs rebuilding its layout
            bool needsRebuild = layout.builtFromVersion != text.version
                || layout.builtFromWidth != bounds.width;
            if (!needsRebuild) continue;

            // Rebuild layout
            rebuild(text, bounds, layout);
            layout.builtFromVersion = text.version;
            layout.builtFromWidth = bounds.width;
        }
	}
private:
    // Ported directly from TextLabel::setText's tokenize/wrap pass,
    // and TextLabel::RecalculateHeight -- same logic, just reading
    // from Text/Bounds instead of member fields, and writing into a
    // TextLayout instead of `this`.
    void rebuild(Text& text, const Bounds& bounds, TextLayout& layout)
    {
        if (!text.font) return;

        layout.glyphCount = 0;
        layout.lines.clear();

        TextStyle currentGlyphStyle;
        TextLine flatLine; // all tokens, pre-wrap

        const std::string& s = text.rawText;
        for (std::size_t i = 0; i < s.size(); ++i)
        {
            switch (s[i])
            {
            case '<': {
                std::size_t styleMarkEnd = s.find('>', i + 1);
                assert(styleMarkEnd != std::string::npos);
                std::string markContent = s.substr(i + 1, styleMarkEnd - i - 1);
                i = styleMarkEnd;
                ParseStyleMark(markContent, currentGlyphStyle);
            } break;
            case ' ': {
                flatLine.tokens.push_back({ TokenType::Space, (unsigned char)' ' });
            } break;
            case '\n': {
                flatLine.tokens.push_back({ TokenType::NewLine });
            } break;
            default: {
                unsigned char c = s[i];
                flatLine.tokens.push_back({ TokenType::Glyph, c, currentGlyphStyle });
                ++layout.glyphCount;
            } break;
            }
        }

        // Wrap into lines that fit bounds.width
        TextLine currentLine;
        auto& tokens = flatLine.tokens;

        for (std::size_t i = 0; i < tokens.size(); ++i)
        {
            switch (tokens[i].type)
            {
            case TokenType::Space: {
                currentLine.tokens.push_back({ TokenType::Space, (unsigned char)' ' });

                float nextWordWidth = calculateNextWordSize(text, (int)i, tokens);
                if (currentLine.width + nextWordWidth > bounds.width)
                {
                    layout.lines.push_back(currentLine);
                    currentLine.tokens.clear();
                    currentLine.width = 0;
                }
                else
                {
                    currentLine.width += text.font->getGlyph((unsigned char)' ')->w * text.wordSpacing;
                }
            } break;
            case TokenType::NewLine: {
                if (!currentLine.tokens.empty())
                {
                    layout.lines.push_back(currentLine);
                    currentLine.tokens.clear();
                    currentLine.width = 0;
                }
            } break;
            default: {
                currentLine.tokens.push_back(tokens[i]);
                currentLine.width += text.font->getGlyph(tokens[i].glyph)->w * text.letterSpacing;
                ++layout.glyphCount;
            } break;
            }
        }

        if (!currentLine.tokens.empty())
            layout.lines.push_back(currentLine);

        recalculateHeight(text, layout);
    }

    void ParseStyleMark(const std::string& params, TextStyle& style)
    {
        std::stringstream styleMark(params);
        char styleMarkType;
        styleMark >> styleMarkType;

        switch (styleMarkType)
        {
        case 'c': {
            int r, g, b, a;
            styleMark >> r >> g >> b >> a;
            style.color = { (Uint8)r, (Uint8)g, (Uint8)b, (Uint8)a };
        } break;
        case 'f': style.isFlipped = !style.isFlipped; break;
        case 'r': styleMark >> style.rotation; break;
        case 's': styleMark >> style.scale; break;
        default: assert(false); // unknown style mark
        }
    }

    float calculateNextWordSize(Text& text, int wordStartIdx, const std::vector<TextToken>& tokens)
    {
        float width = 0;
        int j = wordStartIdx + 1;
        while (j < (int)tokens.size() && tokens[j].type != TokenType::Space)
        {
            if (tokens[j].type == TokenType::Glyph)
                width += text.font->getGlyph(tokens[j].glyph)->w * text.letterSpacing;
            ++j;
        }
        return width;
    }

    void recalculateHeight(Text& text, TextLayout& layout)
    {
        if (layout.lines.empty()) { layout.totalHeight = 0.f; return; }

        auto maxGlyphHeightIn = [&](const TextLine& line)
            {
                float maxH = 0.f;
                for (const TextToken& token : line.tokens)
                    if (token.type == TokenType::Glyph)
                        maxH = std::max(maxH, text.font->getGlyph(token.glyph)->h * token.style.scale);
                if (maxH == 0.f) maxH = (float)text.font->getGlyph(' ')->h;
                return maxH;
            };

        float totalHeight = 0.f;
        float tallestLine = 0.f;
        for (const TextLine& line : layout.lines)
        {
            float h = maxGlyphHeightIn(line);
            totalHeight += h;
            tallestLine = std::max(tallestLine, h);
        }

        if (layout.lines.size() > 1)
            totalHeight += tallestLine * (text.getLineSpacing() - 1.f) * (layout.lines.size() - 1);

        layout.totalHeight = totalHeight;
    }
};