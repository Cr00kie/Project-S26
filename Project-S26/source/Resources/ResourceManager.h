#pragma once

#include "ResourceLoaders/ResourceLoader.h"

#include <SDL3/SDL.h>
#include <SDL3_mixer/SDL_mixer.h>
#include <unordered_map>
#include <string>
#include <stdexcept>
#include <filesystem>
#include <fstream>
#include <typeindex>
#include <cassert>

// A resource manager that manages the loading, caching and releasing of resources in the game engine
class ResourceManager
{
private:
	// A struct that represents a managed resource,
	// it holds the reference count and the resource pointer
	struct ManagedResource
	{
		int iRefCount;
		BasicResource* pResource;
	};

	//vector where all the resource loaders exist
	std::vector<ResourceLoader*> m_LoaderVector;

	//map that index each file extension to its corresponding resource loader
	std::unordered_map<std::string, ResourceLoader*> m_LoaderMap;

	//map that holds the cached resources, it is indexed by the type of resource and then by the resource id
	// TODO: Make possible to make some assets not expire
	std::unordered_map<std::type_index, 
		std::unordered_map<std::string, ManagedResource>> m_ResourceMap;

	//map that holds the file paths for each resource id, it is indexed by the resource id
	//Key: id, value: path
	std::unordered_map<std::string, std::string> m_PathMap;

	// Read the resources file and populate the path map with the resource ids and their corresponding file paths
	void ReadResourcesFile(const std::string& filePath);

	// Initialize the loader map with the appropiate resource loaders for each file extension
	void InitLoaderMap(SDL_Renderer* renderer, MIX_Mixer* sfxMixer, MIX_Mixer* musicMixer);

	// Music mixers
	MIX_Mixer* m_sfxMixer;
	MIX_Mixer* m_musicMixer;

public: 
	// Creates a ResourceManager to manage the resources of the game
	// Arguments:
	//		- filePath: the file with the traductions from asset ID to relative file path
	//		- renderer: the SDL renderer being used
	//		- mixer: mixer object being used
	ResourceManager(const std::string& filePath, SDL_Renderer* renderer, MIX_Mixer* sfxMixer, MIX_Mixer* musicMixer);
	~ResourceManager();

	// Delete copy constructor and assignment operator to prevent copying of the resource manager
	ResourceManager(const ResourceManager&) = delete;
	ResourceManager& operator=(const ResourceManager&) = delete;

	// Get a resource of type ResourceT with the given id,
	// it will load the resource from disk if it is not already cached
	// The resource type must be derived from BasicResource,
	// and the id must be defined in the resources file
	template<std::derived_from<BasicResource> ResourceT>
	ResourceT* GetResource(const std::string& id)
	{
		// Get typeid of resource to get hashmap of those resources
		std::type_index resourceType = typeid(ResourceT);
		auto& resourceMap = m_ResourceMap[resourceType];

		// find the requested resource in the map
		auto it = resourceMap.find(id);
		if (it != resourceMap.end())
		{
			ManagedResource& managedResource = it->second;
			// Return resource if found
			++managedResource.iRefCount;
			return static_cast<ResourceT*>(managedResource.pResource);
		}

		// try to translate from id to path to get the file from disk
		auto pathIt = m_PathMap.find(id);
		if (pathIt == m_PathMap.end())
			throw std::out_of_range("Resource id not found");

		// Get file extension to use the correct loader
		std::string extension = std::filesystem::path(pathIt->second).extension().string();

		// Find loader
		auto loaderIt = m_LoaderMap.find(extension);
		if (loaderIt == m_LoaderMap.end())
			throw std::out_of_range("Incompatible resource type");
		// Get the appropiate loader to load the file
		ResourceLoader* loader = loaderIt->second;

		// Make sure the asset being loaded is the same type as the asset requested
		assert(loader->ResourceType() == resourceType);

		// Load the file
		ManagedResource resource = { 1, loader->Load(pathIt->second) };

		resourceMap[id] = resource;

		// Return the asset and cache it
		return static_cast<ResourceT*>(resource.pResource);
	}

	// Release a resource of type ResourceT with the given id
	template<std::derived_from<BasicResource> ResourceT>
	void ReleaseResource(const std::string& id)
	{
		// Get typeid of resource to get hashmap of those resources
		std::type_index resourceType = typeid(ResourceT);
		auto& resourceMap = m_ResourceMap[resourceType];

		// find the requested resource in the map
		auto it = resourceMap.find(id);
		if (it != resourceMap.end())
		{
			ManagedResource& managedResource = it->second;
			
			//Destroy resource if it is no longer used by anyone
			if (--managedResource.iRefCount <= 0)
			{
				delete managedResource.pResource;
				resourceMap.erase(it);
			}
		}
	}

};
