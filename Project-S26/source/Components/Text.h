#pragma once

#include <string>
#include "../Tools/ServiceLocator.h"
#include "../Resources/ResourceManager.h"
#include "../Resources/Font.h"
#include "../Lua/LuaBindingTools.h"

enum class TextAlign
{
	Left,
	Center,
	Right
};

LUA_EXPOSE_ENUM(TextAlign,
	"Left", TextAlign::Left, 
	"Center", TextAlign::Center,
	"Right", TextAlign::Right)
	
struct Text
{
	Text(const std::string& text,
		const std::string& fontId,
		TextAlign alignment = TextAlign::Left,
		float letterSpacing = 1.f,
		float wordSpacing = 1.f,
		float lineSpacing = 1.f
	) : 
		textAlignment(alignment),
		letterSpacing(letterSpacing),
		wordSpacing(wordSpacing),
		lineSpacing(lineSpacing)
	{
		setFont(fontId);
		setText(text);
	}

	Text(const Text&) = delete;
	Text& operator= (const Text&) = delete;

	Text(Text&& other) noexcept :
		rawText(std::move(other.rawText)),
		textAlignment(other.textAlignment),
		letterSpacing(other.letterSpacing),
		wordSpacing(other.wordSpacing),
		lineSpacing(other.lineSpacing),
		visibleGlyphCount(other.visibleGlyphCount),
		version(other.version)
	{
		font = other.font;
		fontId = std::move(other.fontId);

		other.font = nullptr;
		other.fontId.clear();
	}

	Text& operator=(Text&& other) noexcept
	{
		if (this == &other) return *this;

		if(!fontId.empty()) 
			ServiceLocator::get<ResourceManager>().ReleaseResource<Font>(fontId);

		font = other.font;
		fontId = std::move(other.fontId);

		rawText = std::move(other.rawText);
		textAlignment = other.textAlignment;
		letterSpacing = other.letterSpacing;
		wordSpacing = other.wordSpacing;
		lineSpacing = other.lineSpacing;
		visibleGlyphCount = other.visibleGlyphCount;
		version = other.version;

		other.font = nullptr;
		other.fontId.clear();

		return *this;
	}

	~Text()
	{
		// Release font
		if (!fontId.empty())
			ServiceLocator::get<ResourceManager>().ReleaseResource<Font>(fontId);
	}

	void setFont(const std::string& fontId)
	{
		if (this->fontId == fontId) return;

		if (!fontId.empty())
			ServiceLocator::get<ResourceManager>().ReleaseResource<Font>(fontId);

		this->fontId = fontId;
		// Request font
		font = ServiceLocator::get<ResourceManager>().GetResource<Font>(fontId);

		++version;
	}

	void setText(const std::string& text)
	{
		rawText = text;
		++version;
	}
	std::string getText() { return rawText; }

	void setAlignment(TextAlign alignment) { textAlignment = alignment; ++version; }
	TextAlign getAlignment() { return textAlignment; }

	void setLetterSpacing(float space) { letterSpacing = space; ++version;}
	float getLetterSpacing() { return letterSpacing; }

	void setLineSpacing(float space) { lineSpacing = space; ++version;}
	float getLineSpacing() { return lineSpacing; }

	void setWordSpacing(float space) { wordSpacing = space; ++version;}
	float getWordSpacing() { return wordSpacing; }

	void setVisibleGlyphCount(int count) { visibleGlyphCount = count; }
	int getVisibleGlyphCount() { return visibleGlyphCount; }
	void showAllGlyphs() { visibleGlyphCount = (int)rawText.size(); }

	void setColor(SDL_Color color)
	{
		if (rawText.empty()) return;
		std::string tag = "<c " + std::to_string(color.r) + " " + std::to_string(color.g)
			+ " " + std::to_string(color.b) + " " + std::to_string(color.a) + ">";
		// updates version via setText
		setText(tag + rawText);
	}

	Font* font;
	std::string fontId;
	std::string rawText;
	TextAlign textAlignment;
	float letterSpacing;
	float lineSpacing;
	float wordSpacing;
	int visibleGlyphCount;
	int version = 1;
};