#pragma once
#include <functional>
#include <vector>

// A reactive property that allows subscribing to changes in its value
// It holds a value of type T and a list of subscribers that are notified when the value changes
template<typename T>
class ReactiveProperty {
private:
	// The current value of the reactive property
    T m_value;
	// A list of subscribers that are notified when the value changes
    std::vector<std::function<void(const T&)>> m_subscribers;

public:
    // Creates an empty reactive property
    ReactiveProperty() : m_value(T()) {}
    // Creates a ReactiveProperty
    // Arguments:
    //		- initialValue: the initial value of the reactive property
    explicit ReactiveProperty(const T& initialValue) : m_value(initialValue) {}
    
	// Get the current value of the reactive property
    const T& get() const { 
        return m_value; 
    }
    
	// Set a new value for the reactive property,
    // if the new value is different from the current value, update the value and notify the subscribers
    void set(const T& newValue) {
        if (m_value != newValue) {
            m_value = newValue;
            notify();
        }
    }
    
	// Subscribe to changes in the reactive property by providing a callback function
    void subscribe(std::function<void(const T&)> callback) {
        m_subscribers.push_back(callback);
        callback(m_value);
    }
    
	// Assignment operator to set a new value for the reactive property
    ReactiveProperty& operator=(const T& newValue) {
        set(newValue);
        return *this;
    }
    
	// Implicit conversion operator to allow using the reactive property as if it were a regular value of type T
    operator const T&() const {
        return m_value;
    }
    
private:
	// Notify all subscribers of a change in the value by calling their callback functions with the new value
    void notify() {
        for (auto& subscriber : m_subscribers) {
            subscriber(m_value);
        }
    }
};