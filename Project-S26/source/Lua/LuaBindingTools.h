#pragma once

#include "LuaManager.h"
#include "../Tools/ServiceLocator.h"
#include <string>

#define NO_CONSTRUCTOR sol::no_constructor

#define CONSTRUCTORS(...) sol::constructors<__VA_ARGS__>()

#define BASE_CLASSES(...) , sol::base_classes, sol::bases<__VA_ARGS__>()

#define NO_BASE 

#define LUA_EXPOSE_TYPE(TypeName, ExposedName, Constructors, BaseClasses) \
inline static bool _lua_type_reg_##TypeName = \
    (LuaManager::lua().new_usertype<TypeName>(\
		#ExposedName, Constructors\
        BaseClasses \
), true); \
static const char* getExposedLuaClassName() {return #ExposedName;}\

#define LUA_EXPOSE_MEMBER(Class, MemberName, ExposedName) \
inline static bool _lua_member_reg_##Class##_##MemberName = \
    (LuaManager::lua()[getExposedLuaClassName()][#ExposedName] = &Class::MemberName, true);

#define LUA_EXPOSE_ENUM(EnumName, ...) \
inline static bool _lua_enum_reg_##EnumName = \
    (LuaManager::lua().new_enum(#EnumName, __VA_ARGS__), true);

#define LUA_EXPOSE_COMPONENT_GETTER(HandleType, GetterName) \
inline static bool _lua_getter_reg_##HandleType = \
    (LuaManager::lua()["Entity"][#GetterName] = [](Entity e) { \
        return HandleType(ServiceLocator::get<Registry>(), e); \
    }, true);