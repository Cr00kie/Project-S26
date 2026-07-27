#pragma once

#include "../LuaBindingTools.h"
#include "BaseComponentHandle.h"
#include "../../Components/Text.h"

class TextHandle : public BaseComponentHandle<Text>
{
	LUA_EXPOSE_TYPE(TextHandle, TextHandle, NO_CONSTRUCTOR, NO_BASE)
public:
	TextHandle(Registry& reg, Entity e) : BaseComponentHandle(reg, e) {}

	void setText(const std::string& text);
	std::string getText();
	LUA_EXPOSE_MEMBER(TextHandle, setText, set_text)
	LUA_EXPOSE_MEMBER(TextHandle, getText, get_text)

	void setFont(const std::string& id);
	LUA_EXPOSE_MEMBER(TextHandle, setFont, set_font)

	void setAlignment(TextAlign alignment);
	TextAlign getAlignment();
	LUA_EXPOSE_MEMBER(TextHandle, setAlignment, set_alignment)
	LUA_EXPOSE_MEMBER(TextHandle, getAlignment, get_alignment)

	void setLetterSpacing(float space);
	float getLetterSpacing();
	LUA_EXPOSE_MEMBER(TextHandle, setLetterSpacing, set_letter_spacing)
	LUA_EXPOSE_MEMBER(TextHandle, getLetterSpacing, get_letter_spacing)

	void setLineSpacing(float space);
	float getLineSpacing();
	LUA_EXPOSE_MEMBER(TextHandle, setLineSpacing, set_line_spacing)
	LUA_EXPOSE_MEMBER(TextHandle, getLineSpacing, get_line_spacing)

	void setWordSpacing(float space);
	float getWordSpacing();
	LUA_EXPOSE_MEMBER(TextHandle, setWordSpacing, set_word_spacing)
	LUA_EXPOSE_MEMBER(TextHandle, getWordSpacing, get_word_spacing)

	void setVisibleGlyphCount(int count);
	int getVisibleGlyphCount();
	void showAllGlyphs();
	LUA_EXPOSE_MEMBER(TextHandle, setVisibleGlyphCount, set_visible_glyph_count)
	LUA_EXPOSE_MEMBER(TextHandle, getVisibleGlyphCount, get_visible_glyph_count)
	LUA_EXPOSE_MEMBER(TextHandle, showAllGlyphs, show_all_glyphs)

};
LUA_EXPOSE_COMPONENT_GETTER(TextHandle, get_text)