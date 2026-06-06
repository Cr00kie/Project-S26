#pragma once
#include "Tools/ServiceLocator.h"
#include "SDL3/SDL.h"
#include "SDL3_ttf/SDL_ttf.h"
#include <iostream>
#include <source_location>
#include <string>
#include <format>
#include <vector>

#ifdef _DEBUG
class Debug
{
private:
	struct DebugDrawCommand
	{
		enum Type { Line, Rect, Point, Text};
		Type type;
		SDL_Color color;
		float x1, y1, x2, y2;
		std::string text;
		float scale;
	};

	SDL_Renderer* m_debugRenderer;
	std::ostream* m_logOutput;
	TTF_Font* m_debugFont;
	std::vector<DebugDrawCommand> m_drawCommands;

	std::string_view shortFileName(std::string_view filePath)
	{
		std::size_t pos = filePath.find_last_of("/\\");
		return ((pos == std::string_view::npos) ? filePath : filePath.substr(pos + 1));
	}

	std::string shortFunctionName(std::string_view func)
	{
		auto paren = func.find('(');
		auto sig = (paren != std::string_view::npos) ? func.substr(0, paren) : func;

		auto space = sig.rfind(' ');
		auto name = (space != std::string_view::npos) ? sig.substr(space + 1) : sig;

		return std::string(name);
	}

	std::string getHHMMSS(Uint64 ms)
	{
		Uint64 totalSeconds = ms / 1000;
		return std::format("{:02}:{:02}:{:02}",
			totalSeconds / 3600,
			(totalSeconds % 3600) / 60,
			totalSeconds % 60);
	}

public:
	Debug(SDL_Renderer* renderer, std::ostream* logOutput = &std::cout) : m_debugRenderer(renderer), m_logOutput(logOutput), m_debugFont(nullptr)
	{
		m_debugFont = TTF_OpenFont("assets/Fonts/NunitoSans.ttf",64);
		if (!m_debugFont)
		{
			error("Debug font failed to load: " + std::string(SDL_GetError()));
		}
	}
	~Debug()
	{
		if (m_debugRenderer)
		{
			SDL_DestroyRenderer(m_debugRenderer);
			m_debugRenderer = nullptr;
		}

		if (m_debugFont)
		{
			TTF_CloseFont(m_debugFont);
			m_debugFont = nullptr;
		}
	}

	void log(const std::string& message, const char* file, int line, const char* func)
	{
		(*m_logOutput) << "[" << shortFileName(file) << ":" << line << " | " << shortFunctionName(func) << "] " << "[" << getHHMMSS(SDL_GetTicks()) << "] > " << message;
	}

	void log(const std::string& message, std::source_location loc = std::source_location::current())
	{
		log(message, loc.file_name(), loc.line(), loc.function_name());
	}

	void warn(const std::string& message, const char* file, int line, const char* func)
	{
		(*m_logOutput) << "\033[33m" << "WARNING: ";
		log(message, file, line, func);
		(*m_logOutput) << "\033[0m";
	}
	void warn(const std::string& message, std::source_location loc = std::source_location::current())
	{
		warn(message, loc.file_name(), loc.line(), loc.function_name());
	}

	void error(const std::string& message, const char* file, int line, const char* func)
	{
		(*m_logOutput) << "\033[31m" << "ERROR: ";
		log(message, file, line, func);
		(*m_logOutput) << "\033[0m";
	}
	void error(const std::string& message, std::source_location loc = std::source_location::current())
	{
		error(message, loc.file_name(), loc.line(), loc.function_name());
	}

	void info(const std::string& message, const char* file, int line, const char* func)
	{
		(*m_logOutput) << "\033[36m" << "INFO: ";
		log(message, file, line, func);
		(*m_logOutput) << "\033[0m";
	}
	void info(const std::string& message, std::source_location loc = std::source_location::current())
	{
		info(message, loc.file_name(), loc.line(), loc.function_name());
	}

	void render()
	{
		for (DebugDrawCommand& drawCom : m_drawCommands)
		{
			SDL_SetRenderDrawColor(m_debugRenderer, drawCom.color.r, drawCom.color.g, drawCom.color.b, drawCom.color.a);
			
			switch (drawCom.type)
			{
			case DebugDrawCommand::Line: {
				SDL_RenderLine(m_debugRenderer, drawCom.x1, drawCom.y1, drawCom.x2, drawCom.y2);
			} break;
			case DebugDrawCommand::Rect: {
				SDL_FRect rect = { drawCom.x1, drawCom.y1, drawCom.x2, drawCom.y2 };
				SDL_RenderRect(m_debugRenderer, &rect);
			} break;
			case DebugDrawCommand::Point: {
				SDL_RenderPoint(m_debugRenderer, drawCom.x1, drawCom.y1);
			} break;
			case DebugDrawCommand::Text: {
				SDL_Surface* surf = TTF_RenderText_Solid(m_debugFont, drawCom.text.c_str(), 0, drawCom.color);
				if (!surf)
				{
					error("TTF_RenderText_Solid failed: " + std::string(SDL_GetError()));
					return;
				}
				SDL_Texture* tex = SDL_CreateTextureFromSurface(m_debugRenderer, surf);
				

				float w, h;
				SDL_GetTextureSize(tex, &w, &h);

				SDL_FRect src = { 0,0,w,h };
				SDL_FRect target = { drawCom.x1, drawCom.y1, w*drawCom.scale, h*drawCom.scale };
				SDL_RenderTexture(m_debugRenderer, tex, &src, &target );

				SDL_DestroyTexture(tex);
				SDL_DestroySurface(surf);
			} break;
			}
		}

		m_drawCommands.clear();
	}

	void DrawPoint(float x, float y, SDL_Color color = {255, 255, 255, 255})
	{
		DebugDrawCommand com;
		com.type = DebugDrawCommand::Point;
		com.color = color;
		com.x1 = x;
		com.y1 = y;
		m_drawCommands.push_back(com);
	}

	void DrawLine(float x1, float y1, float x2, float y2, SDL_Color color = { 255, 255, 255, 255 })
	{
		DebugDrawCommand com;
		com.type = DebugDrawCommand::Line;
		com.color = color;
		com.x1 = x1;
		com.y1 = y1;
		com.x2 = x2;
		com.y2 = y2;
		m_drawCommands.push_back(com);
	}

	void DrawRect(float x1, float y1, float x2, float y2, SDL_Color color = { 255, 255, 255, 255 })
	{
		DebugDrawCommand com;
		com.type = DebugDrawCommand::Rect;
		com.color = color;
		com.x1 = x1;
		com.y1 = y1;
		com.x2 = x2;
		com.y2 = y2;
		m_drawCommands.push_back(com);
	}

	void DrawText(float x, float y, const std::string& text, float scale = 1, SDL_Color color = { 255, 255, 255, 255 })
	{
		DebugDrawCommand com;
		com.type = DebugDrawCommand::Text;
		com.color = color;
		com.x1 = x;
		com.y1 = y;
		com.text = text;
		com.scale = scale;
		m_drawCommands.push_back(com);
	}
};

#define DEBUG ServiceLocator::get<Debug>()
#define LOG(message) DEBUG.log(message, __FILE__, __LINE__, __func__)
#define WARN(message) DEBUG.warn(message, __FILE__, __LINE__, __func__)
#define ERROR(message) DEBUG.error(message, __FILE__, __LINE__, __func__)
#define INFO(message) DEBUG.info(message, __FILE__, __LINE__, __func__)

#else
class Debug
{
public:
	Debug(SDL_Renderer* renderer, std::ostream* logOutput = &std::cout){}

	void log(const std::string& message, const char* file, int line, const char* func){}

	void log(const std::string& message, std::source_location loc = std::source_location::current()){}

	void warn(const std::string& message, const char* file, int line, const char* func){}
	void warn(const std::string& message, std::source_location loc = std::source_location::current()){}

	void error(const std::string& message, const char* file, int line, const char* func){}
	void error(const std::string& message, std::source_location loc = std::source_location::current()){}

	void info(const std::string& message, const char* file, int line, const char* func){}
	void info(const std::string& message, std::source_location loc = std::source_location::current()){}

	void render() {}

	void DrawPoint(float x, float y, SDL_Color color = { 255, 255, 255, 255 }){}
	void DrawLine(float x1, float y1, float x2, float y2, SDL_Color color = { 255, 255, 255, 255 }) {}
	void DrawRect(float x1, float y1, float x2, float y2, SDL_Color color = { 255, 255, 255, 255 }) {}
	void DrawText(float x, float y, std::string text, float scale = 1, SDL_Color color = { 255, 255, 255, 255 }) {}
};

#define DEBUG ServiceLocator::get<Debug>()
#define LOG(message) ((void)0)
#define WARN(message) ((void)0)
#define ERROR(message) ((void)0)
#define INFO(message) ((void)0)

#endif