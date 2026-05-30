#pragma once

#include <unordered_map>
#include <memory>
#include <typeindex>
#include <stdexcept>

// A simple service locator implementation that allows registering and retrieving services by type
// Services are stored as void pointers and cast to the appropriate type when retrieved
// Note: This implementation does not manage the lifetime of the services, it is the responsibility of the caller to ensure that the services are properly deleted when no longer needed
class ServiceLocator
{
public:
	// Register a service of type T, the service must be a pointer and must not be null
    template<typename T>
    static void registerService(T* service)
    {
        if (!service) throw std::runtime_error("Cannot register null service");

        auto const key = std::type_index(typeid(T));

        if (m_services.contains(key))
            throw std::runtime_error("Service already registered");
        
        m_services[key] = service;
    }

	// Get a reference to the service of type T, throws an exception if the service is not found
    template<typename T>
    static T& get()
    {
        auto key = std::type_index(typeid(T));
        auto it  = m_services.find(key);

        if (it == m_services.end())
            throw std::runtime_error("Service not found");

        return *static_cast<T*>(it->second);
    }

	// Get a pointer to the service of type T, returns nullptr if the service is not found
    template<typename T>
    static T* tryGet() noexcept
    {
        auto key = std::type_index(typeid(T));
        auto it  = m_services.find(key);

        if (it == m_services.end())
            return nullptr;

        return static_cast<T*>(it->second);
    }

	// Get a pointer to the service of type T, returns nullptr if the service is not found
    template<typename T>
    static T* tryGetPtr() noexcept
    {
        if (auto sp = tryGet<T>()) return sp.get();
        return nullptr;
    }

	// Unregister a service of type T, deletes the service if found
    template<typename T>
    static void unregister() noexcept
    {
        // Find service
        auto key = std::type_index(typeid(T));
        auto it = m_services.find(key);

        // Erase service if found
        if (it != m_services.end())
        {
            delete static_cast<T*>(it->second);
            m_services.erase(key);
        }
    }

	// Clear all registered services, deletes all services and clears the services map
    static void clear() noexcept
    {
        // Clear services map
        m_services.clear();
    }

private:
	// The map that holds the registered services,
    // the key is the type index of the service and the value is a void pointer to the service
    inline static std::unordered_map<std::type_index, void*> m_services;
};