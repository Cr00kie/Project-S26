#include "ResourceManager.h"

#include "ResourceLoaders/TextureLoader.h"
#include "ResourceLoaders/FontLoader.h"
#include "ResourceLoaders/SoundLoader.h"
#include "ResourceLoaders/AnimationLoader.h"
#include "json.hpp"
#include <iostream>

ResourceManager::ResourceManager(const std::string& filePath, SDL_Renderer* renderer, MIX_Mixer* sfxMixer, MIX_Mixer* musicMixer) : 
	m_sfxMixer(sfxMixer), m_musicMixer(musicMixer) {
	ReadResourcesFile(filePath);
	InitLoaderMap(renderer, sfxMixer, musicMixer);
}

ResourceManager::~ResourceManager()
{
	// Delete all resources
	for (auto& resourceMap : m_ResourceMap)
	{
		for (auto& resource : resourceMap.second)
		{
			delete resource.second.pResource;
		}
		resourceMap.second.clear();
	}
	m_ResourceMap.clear();

	//Delete all loaders
	for (auto& loader : m_LoaderVector)
	{
		delete loader;
	}
	m_LoaderVector.clear();
}

//Reads the resources file, and loads thaat resource into the resources map
void ResourceManager::ReadResourcesFile(const std::string& filePath) {
	// Try open file
	std::ifstream file(filePath);
	if (!file.is_open())
		throw std::runtime_error("Could not open resources file");

	// Windows Notepad adds BOM to the beggining of the file because jajan't
	char checkBOM[4] = { 0 };
	file.get(checkBOM, 4);
	// If we caught Byte Order Mark at the beggining of file, ignore BOM
	if (strcmp(checkBOM, "\xEF\xBB\xBF"))
	{
		// If it wasn´t BOM, then unget from stream
		file.seekg(0);
	}

	nlohmann::json data;
	file >> data;
	// Read every asset id and path
	if (data.contains("Textures")) {
		const auto& textures = data["Textures"];
		for (const auto& tex : textures) {
			m_PathMap.insert({
				tex["id"].get<std::string>(),
				tex["path"].get<std::string>()
				});
		}
	}
	if (data.contains("Sounds")) {
		const auto& sounds = data["Sounds"];
		for (const auto& sound : sounds) {
			m_PathMap.insert({
				sound["id"].get<std::string>(),
				sound["path"].get<std::string>()
				});
		}
	}
	if (data.contains("Music")) {
		const auto& music = data["Music"];
		for (const auto& mus : music) {
			m_PathMap.insert({
				mus["id"].get<std::string>(),
				mus["path"].get<std::string>()
				});
		}
	}
	if (data.contains("Fonts")) {
		const auto& fonts = data["Fonts"];
		for (const auto& font : fonts) {
			m_PathMap.insert({
				font["id"].get<std::string>(),
				font["path"].get<std::string>()
				});
		}
	}
	if (data.contains("Scripts")) {
		const auto& scripts = data["Scripts"];
		for (const auto& scrp : scripts) {
			m_PathMap.insert({
				scrp["id"].get<std::string>(),
				scrp["path"].get<std::string>()
				});
		}
	}
	if (data.contains("Animations"))
	{
		const auto& animations = data["Animations"];
		for (const auto& anim : animations)
		{
			m_PathMap.insert({
				anim["id"].get<std::string>(),
				anim["path"].get<std::string>()
				});
		}
	}

	// Close file
	if (file.is_open())
		file.close();
}

//Creates all loaders on the m_loaderVector, and then maps every loader to a file extension
void ResourceManager::InitLoaderMap(SDL_Renderer* renderer, MIX_Mixer* sfxMixer, MIX_Mixer* musicMixer)
{
	ResourceLoader* textureLoader = m_LoaderVector.emplace_back(new TextureLoader(renderer));
	ResourceLoader* fontLoader = m_LoaderVector.emplace_back(new FontLoader(renderer, 128));
	ResourceLoader* soundLoader = m_LoaderVector.emplace_back(new SoundLoader(sfxMixer));
	ResourceLoader* animLoader = m_LoaderVector.emplace_back(new AnimationLoader());

	// Map all type of resource loaders to file extensions

	m_LoaderMap[".png"] = textureLoader;
	m_LoaderMap[".jpg"] = textureLoader;
	m_LoaderMap[".bmp"] = textureLoader;

	m_LoaderMap[".ttf"] = fontLoader;

	m_LoaderMap[".wav"] = soundLoader;
	m_LoaderMap[".mp3"] = soundLoader;

	m_LoaderMap[".anim"] = animLoader;
}