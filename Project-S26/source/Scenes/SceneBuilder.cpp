#include "SceneBuilder.h"
#include <fstream>
#include "../Debug.h"
#include "../UI/BasicUI/Sprite.h"
#include "../UI/BasicUI/TextLabel.h"
#include "../Math/Vec2.h"
#include "../Math/Mat3.h"

SceneBuilder::SceneBuilder(const std::string& tiledAssetsJSONPath)
	: m_TiledTileset(std::move(nlohmann::json::parse(std::fstream(tiledAssetsJSONPath))["tiles"])), m_iFirstGID(1)
{
	// Default factories
	m_Factories = {
		{ "Image", [this](nlohmann::json& o) { return createImage(o); } },
		{ "Nineslice", [this](nlohmann::json& o) { return createNineslice(o); } },
		{ "TextLabel", [this](nlohmann::json& o) { return createTextLabel(o); } },
		{ "SimpleButton", [this](nlohmann::json& o) { return createSimpleButton(o); } },
	};
}

void SceneBuilder::loadScene(UIElement* root, const std::string& sceneJSONPath)
{
	nlohmann::json sceneJSON = nlohmann::json::parse(std::fstream(sceneJSONPath));

	m_iFirstGID = sceneJSON["tilesets"][0]["firstgid"];

	auto& layers = sceneJSON["layers"];
	for (auto& layer : layers) loadLayer(layer, root);
}

void SceneBuilder::registerFactory(const std::string& factoryKey, UIElementFactory factory)
{
	m_Factories.insert(std::make_pair(factoryKey, factory));
}

void SceneBuilder::loadLayer(nlohmann::json_abi_v3_12_0::json& layer, UIElement* root)
{
	UIElement* container = new UIElement(0, 0);

	auto& offX = layer["offsetx"];
	auto& offY = layer["offsety"];
	if (!offX.is_null())
		container->setX(offX);
	if (!offY.is_null())
		container->setY(offY);

	// Register Container
	std::string name = layer["name"];
	auto& layerId = layer["id"];
	if (name == "" && layerId)
		name = "layer" + layerId;
	m_UIElements.insert(std::make_pair(name, container));

	root->addChild(container);

	// Check if there are more inner layers nested
	auto& innerLayers = layer["layers"];
	if (innerLayers.is_null())
	{
		loadLayerObjects(container, layer["objects"]);
	}
	else
	{
		for (auto& layer : innerLayers) loadLayer(layer, container);
	}

}

SDL_Color SceneBuilder::HexToSDLColor(const std::string& hex)
{
	SDL_Color color;
	// Only read alpha if hex has enough characters for it
	if (hex.size() >= 9)
	{
		color.a = std::stoi(hex.substr(1, 2), nullptr, 16);
		color.r = std::stoi(hex.substr(3, 2), nullptr, 16);
		color.g = std::stoi(hex.substr(5, 2), nullptr, 16);
		color.b = std::stoi(hex.substr(7, 2), nullptr, 16);
	}
	else
	{
		color.a = 255;
		color.r = std::stoi(hex.substr(1, 2), nullptr, 16);
		color.g = std::stoi(hex.substr(3, 2), nullptr, 16);
		color.b = std::stoi(hex.substr(5, 2), nullptr, 16);
	}

	return color;
}

Vec2f SceneBuilder::tiledTransformToGame(float x, float y, float width, float height, float degrees) const
{
	Vec2f rotatedOffset = Mat3f::rotation(degrees * std::numbers::pi_v<float> / 180.f) * Vec2f(width / 2.f, -height / 2.f);
	return Vec2f(x + rotatedOffset.getX(), y + rotatedOffset.getY());
}

void SceneBuilder::loadLayerObjects(UIElement* container, nlohmann::json& layerObjects)
{
	for (nlohmann::json& object : layerObjects)
	{
		createObject(container, object);
	}
}

void SceneBuilder::createObject(UIElement* container, nlohmann::json& object)
{
	auto& type = object["type"];
	auto itFactory = m_Factories.find(type);

	if (itFactory == m_Factories.end())
		ERROR("Unkown type of object in scene: " + type + '\n');
	else
	{
		// Create Object
		UIElement* obj = itFactory->second(object);
		container->addChild(obj);

		// Register Object
		std::string name = object["name"];
		if (name == "") name = std::to_string(int(object["id"]));
		m_UIElements.insert(std::make_pair(name, obj));
		LOG(name + " : " + std::string(type) + '\n');
	}
}

UIElement* SceneBuilder::createTextLabel(nlohmann::json& object)
{
	float tiledX = object["x"];
	float tiledY = object["y"];
	float width = object["width"];
	float height = object["height"];
	float rotation = object["rotation"];
	std::string text = object["text"]["text"];
	auto& tiledFont = object["text"]["font"];
	std::string font;
	if (tiledFont.is_null()) font = "NunitoSans";
	else font = tiledFont;

	auto& tiledColor = object["text"]["color"];

	auto& tiledAlignment = object["text"]["halign"];
	TextLabel::TextAlign alignment;
	if (tiledAlignment.is_null() || tiledAlignment == "left") alignment = TextLabel::TextAlign::Left;
	else if (tiledAlignment == "right") alignment = TextLabel::TextAlign::Right;
	else alignment = TextLabel::TextAlign::Center;

	// Tiled rotates using the bottom left corner as pivot, we want to correct out position to take that into account
	Vec2f finalPosition = tiledTransformToGame(tiledX, tiledY, width, -height, rotation);

	bool visible = object["visible"];

	TextLabel* textLabel = new TextLabel(finalPosition.getX(), finalPosition.getY(), text, font, width, height, 1, alignment, rotation);

	textLabel->setVisible(visible);
	if(!tiledColor.is_null()) textLabel->setColor(HexToSDLColor(tiledColor));

	return textLabel;
}

UIElement* SceneBuilder::createSimpleButton(nlohmann::json& object)
{
	Sprite* button = createSprite(object);

	SDL_Color defaultColor = button->getRGBAModulation();
	std::string defaultImg = button->getTexture();

	SDL_Color clickColor = HexToSDLColor(getProperty(object, "Click Color"));
	float clickScale = getProperty(object, "Click Scale");
	auto clickImg = getProperty(object, "Click Image");
	if (clickImg.is_null()) clickImg = defaultImg;

	SDL_Color hoverColor = HexToSDLColor(getProperty(object, "Hover Color"));
	float hoverScale = getProperty(object, "Hover Scale");
	auto hoverImg = getProperty(object, "Hover Image");
	if (hoverImg.is_null()) hoverImg = defaultImg;

	TextLabel* buttonText = new TextLabel(0, 0, getProperty(object, "Text"), "NunitoSans",
		-1, -1, 1, TextLabel::TextAlign::Center);
	button->addChild(buttonText);
	buttonText->setColor(HexToSDLColor(getProperty(object, "Text Color")));
	buttonText->setInteractive(false);

	button->m_userEvents.onClick.addListener([button, clickColor, clickScale, clickImg](MouseEvent&)
		{
			button->setRGBAModulation(clickColor);
			button->setScale(clickScale);
			button->setTexture(clickImg);
		});
	button->m_userEvents.onHoverEnter.addListener([button, hoverColor, hoverScale, hoverImg](MouseEvent&)
		{
			button->setRGBAModulation(hoverColor);
			button->setScale(hoverScale);
			button->setTexture(hoverImg);
		});
	button->m_userEvents.onHoverExit.addListener([button, defaultColor, defaultImg](MouseEvent&)
		{
			button->setRGBAModulation(defaultColor);
			button->setScale(1);
			button->setTexture(defaultImg);
		});

	return button;
}

UIElement* SceneBuilder::createNineslice(nlohmann::json& object)
{
	auto lb = getProperty(object, "Left Border");
	auto rb = getProperty(object, "Right Border");
	auto tb = getProperty(object, "Top Border");
	auto bb = getProperty(object, "Bottom Border");

	Sprite* nineslice = createSprite(object);

	nineslice->setType(NINESLICE);

	if (!lb.is_null()) nineslice->setLeftWidth(lb);
	if (!rb.is_null()) nineslice->setRightWidth(rb);
	if (!tb.is_null()) nineslice->setTopHeight(tb);
	if (!bb.is_null()) nineslice->setBottomHeight(bb);

	return nineslice;
}

Sprite* SceneBuilder::createSprite(nlohmann::json& object)
{
	float tiledX = object["x"];
	float tiledY = object["y"];
	float width = object["width"];
	float height = object["height"];
	float rotation = object["rotation"];
	// Tiled rotates using the bottom left corner as pivot, we want to correct out position to take that into account
	Vec2f finalPosition = tiledTransformToGame(tiledX, tiledY, width, height, rotation);

	auto modulation = getProperty(object, "Modulation");

	bool visible = object["visible"];
	Uint8 opacity = Uint8(float(object["opacity"]) * 255);
	std::string id = m_TiledTileset[object["gid"] - m_iFirstGID]["type"];

	Sprite* sprite = new Sprite(finalPosition.getX(), finalPosition.getY(), id, IMAGE, width, height, rotation);

	sprite->setVisible(visible);
	sprite->setOpacity(opacity);
	if (!modulation.is_null()) sprite->setRGBAModulation(HexToSDLColor(modulation));

	return sprite;
}

UIElement* SceneBuilder::createImage(nlohmann::json& object)
{
	Sprite* image = createSprite(object);
	image->setType(IMAGE);

	return image;
}

nlohmann::json SceneBuilder::getProperty(nlohmann::json& object, const std::string& propName)
{
	// Find property
	for (auto& prop : object["properties"])
	{
		if (prop["name"] == propName)
			return prop["value"];
	}

	// If property not found return null
	return nullptr;
}
