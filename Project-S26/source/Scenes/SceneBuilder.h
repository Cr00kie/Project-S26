#pragma once

#include <string>
#include <unordered_map>
#include <SDL3/SDL.h>
#include <json.hpp>
#include <type_traits>

#include "../Math/Vec2.h"

class UIElement;
class Sprite;

class SceneBuilder
{
public:
	using UIElementFactory = std::function<UIElement*(nlohmann::json&)>;

	SceneBuilder(const std::string& tiledAssetsJSONPath);
	void loadScene(UIElement* scene, const std::string& sceneJSONPath);
	void registerFactory(const std::string& factoryKey, UIElementFactory factory);
	
	template<std::derived_from<UIElement> T>
	T* Q(const std::string& id) const
	{
		auto it = m_UIElements.find(id);

		if (it == m_UIElements.end()) return nullptr;

		return dynamic_cast<T*>(it->second);
	}
private:
    SDL_Color HexToSDLColor(const std::string& hex);
	Vec2f tiledTransformToGame(float x, float y, float width, float height, float degrees)const;

	void loadLayerObjects(UIElement* container, nlohmann::json&);
	void loadLayer(nlohmann::json_abi_v3_12_0::json& layer, UIElement * root);
	void createObject(UIElement*, nlohmann::json&);

	Sprite* createSprite(nlohmann::json& object);
	UIElement * createImage(nlohmann::json& object);
	UIElement * createNineslice(nlohmann::json& object);
	UIElement * createTextLabel(nlohmann::json & object);
	UIElement* createSimpleButton(nlohmann::json& object);

	nlohmann::json getProperty(nlohmann::json& object, const std::string& propName);

	nlohmann::json m_TiledTileset;

	std::unordered_map<std::string, UIElementFactory> m_Factories;
	int m_iFirstGID;

	std::unordered_map<std::string, UIElement*> m_UIElements;
};

