#include "TextHandle.h"

void TextHandle::setText(const std::string& text) { get().setText(text); }

std::string TextHandle::getText() { return get().getText(); }

void TextHandle::setFont(const std::string& id) { get().setFont(id); }

void TextHandle::setAlignment(TextAlign alignment) { get().setAlignment(alignment); }

TextAlign TextHandle::getAlignment() { return get().getAlignment(); }

void TextHandle::setLetterSpacing(float space) { get().setLetterSpacing(space); }

float TextHandle::getLetterSpacing() { return get().getLetterSpacing(); }

void TextHandle::setLineSpacing(float space) { get().setLineSpacing(space); }

float TextHandle::getLineSpacing() { return get().getLineSpacing(); }

void TextHandle::setWordSpacing(float space) { get().setWordSpacing(space); }

float TextHandle::getWordSpacing() { return get().getWordSpacing(); }

void TextHandle::setVisibleGlyphCount(int count) { get().setVisibleGlyphCount(count); }

int TextHandle::getVisibleGlyphCount() { return get().getVisibleGlyphCount(); }

void TextHandle::showAllGlyphs() { get().showAllGlyphs(); }
