#pragma once

#include "../Entities/Registry.h"
#include "../Components/Text.h"
#include "../Components/TextLayout.h"
#include "../Components/Transform.h"

class TextRenderSystem
{
public:
	void render(Registry& reg)
	{
		for (Entity e : reg.view<Text>())
		{
            if (!reg.has<TextLayout>(e) || !reg.has<Transform>(e) || !reg.has<Bounds>(e)) continue;

            Text& txt = reg.get<Text>(e);
            TextLayout& layout = reg.get<TextLayout>(e);
            Transform& trs = reg.get<Transform>(e);
            Bounds& bounds = reg.get<Bounds>(e);

            if (!txt.font || txt.rawText.empty()) continue;
            if (bounds.width <= 0 || bounds.height <= 0) continue;

            renderText(txt, layout, trs, bounds);
		}
	}

private:

    void renderText(Text& text, TextLayout& layout, Transform& trs, Bounds& bnds)
    {
        Vec2f position = trs.worldMatrix.getTranslation();
        float globX = position.getX();
        float globY = position.getY();
        float rotation = trs.worldMatrix.getRotation();
        Vec2f scale = trs.scale;

        float rotCos = std::cos(rotation);
        float rotSin = std::sin(rotation);

        SDL_FRect caret = { 0, 0,
            text.font->getGlyph(text.rawText[0])->w * trs.scale.getX(),
            text.font->getGlyph(text.rawText[0])->h * trs.scale.getY() };

        const int visibleGlyphLimit = std::clamp(
            text.visibleGlyphCount < 0 ? layout.glyphCount : text.visibleGlyphCount,
            0, layout.glyphCount);

        bool stopRendering = false;

        for (size_t lineIdx = 0; lineIdx < layout.lines.size(); ++lineIdx)
        {
            TextLine& line = layout.lines[lineIdx];

            if (stopRendering) break;
            caret.x = getLineStartX(line, text.textAlignment, bnds.width, scale.getX());
            caret.y = getLineStartY(layout, lineIdx, text.textAlignment, bnds.height, scale.getY(), text.lineSpacing, text.font);

            for (TextToken& token : line.tokens)
            {
                caret.w = text.font->getGlyph(token.glyph)->w * trs.scale.getX();
                caret.h = text.font->getGlyph(token.glyph)->h * trs.scale.getY();
                if (token.type == TokenType::Space)
                {
                    caret.x += caret.w * text.wordSpacing;
                    continue;
                }

                SDL_FRect styledCaret = computeStyledCaret(caret, token.style, rotCos, rotSin, bnds.width, bnds.height);

                renderGlyph(text.font, globX, globY, token, styledCaret, rotation);

                caret.x += caret.w * token.style.scale * (token.type == TokenType::Glyph
                    ? text.letterSpacing : text.wordSpacing);
            }
        }
    }

    float getLineStartX(const TextLine& line, TextAlign alignment, float boxWidth, float scaleX) const
    {
        switch (alignment)
        {
        case TextAlign::Left:   return 0.f;
        case TextAlign::Center: return (boxWidth - line.width * scaleX) * 0.5f;
        case TextAlign::Right:  return  (boxWidth - line.width * scaleX);
        }
        return 0.f;
    }

    float getLineStartY(const TextLayout& layout, size_t lineIndex, TextAlign alignment,
        float boxHeight, float scaleY, float lineSpacing, Font* font) const
    {
        float startY = 0.f;
        if (alignment == TextAlign::Center)
            startY = (boxHeight - layout.totalHeight * scaleY) * 0.5f;

        for (size_t i = 0; i < lineIndex; ++i)
        {
            float lineHeight = 0.f;
            for (const TextToken& token : layout.lines[i].tokens)
                if (token.type == TokenType::Glyph)
                    lineHeight = std::max(lineHeight, font->getGlyph(token.glyph)->h * scaleY * token.style.scale);

            if (lineHeight == 0.f) lineHeight = font->getGlyph(' ')->h * scaleY;
            startY += lineHeight * lineSpacing;
        }
        return startY;
    }

    SDL_FRect computeStyledCaret(const SDL_FRect& caret, const TextStyle& style,
        float rotCos, float rotSin, float boxWidth, float boxHeight) const
    {
        SDL_FRect sc = caret;
        sc.x += caret.w * style.scale / 2.f - boxWidth / 2.f;
        sc.y += caret.h / 2.f - boxHeight / 2.f;
        sc.w *= style.scale;
        sc.h *= style.scale;
        sc.y -= caret.h * (style.scale - 1.f) / 2.f;

        float oldX = sc.x;
        sc.x = sc.x * rotCos - sc.y * rotSin;
        sc.y = oldX * rotSin + sc.y * rotCos;
        return sc;
    }

    void renderGlyph(Font* font, float x, float y, const TextToken& token,
        const SDL_FRect& styledCaret, float rotation) const
    {
        SDL_FlipMode flip = token.style.isFlipped ? SDL_FLIP_HORIZONTAL : SDL_FLIP_NONE;
        font->renderCharacter(
            token.glyph,
            { styledCaret.x - styledCaret.w / 2.f + x, styledCaret.y - styledCaret.h / 2.f + y,
              styledCaret.w, styledCaret.h },
            flip, token.style.rotation + rotation, token.style.color);
    }
};