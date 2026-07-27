#pragma once

#include "SDL3/SDL.h"
#include <vector>

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
    Space,
    NewLine
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
    float width = 0.f;
};

struct TextLayout
{
    std::vector<TextLine> lines;
    float totalHeight = 0.f;
    int glyphCount = 0;

    int builtFromVersion = 0;
    float builtFromWidth = -1.f;
};