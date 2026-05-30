#pragma once

#include "../../Resources/BasicResource.h"

#include <string>
#include <memory>
#include <typeindex>

// An abstract base class for resource loaders,
// it defines the interface for loading resources from file paths
class ResourceLoader
{
public:
	ResourceLoader() = default;
	virtual ~ResourceLoader() = default;

	// Get the type of resource that this loader can load,
	// it will be used to determine which loader to use for a given resource type
	virtual std::type_index ResourceType() const = 0;
	// Load a resource from the given file path and return it as a BasicResource pointer,
	virtual BasicResource* Load(const std::string& path) = 0;

	// Delete copy constructor and assignment operator to prevent copying of resource loaders
	ResourceLoader& operator=(const ResourceLoader&) = delete;
	ResourceLoader(const ResourceLoader&) = delete;
};

