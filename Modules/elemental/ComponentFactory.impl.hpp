/* ComponentFactory.impl.hpp
 * Copyright © 2024 Saul D. Beniquez
 * License:  Mozilla Public License v. 2.0
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v.2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/.
 */

#pragma once

#include "IOCore/Exception.hpp"

#include "Component.hpp"

#ifndef COMP_FACTORY_DECL
#	include "ComponentFactory.hpp"
#endif

#include <type_traits>
#include <unordered_map>
#include <vector>

namespace elemental {

using ComponentVector = ComponentFactory::ComponentVector;

inline ComponentFactory::~ComponentFactory()
{
	for (auto pair: component_pool) {
		auto key   = pair.first;
		auto value = pair.second;

		value.clear();
	}
	component_pool.clear();
}

template<typename TComponent>
constexpr bool is_component_v = std::is_base_of_v<Component, TComponent>;

template<typename TComponent, typename... TArgs>
auto ComponentFactory::createComponent(TArgs&&... args)
    -> std::shared_ptr<TComponent>
{
	static_assert(
	    is_component_v<TComponent>,
	    "TComponent must be a base class of Component");

	using ComponentType = typename std::remove_reference_t<TComponent>;
	auto new_object     = std::make_shared<ComponentType>(
	    *this, std::forward<TArgs>(args)...);

	auto component_type =
	    static_cast<std::type_index>(typeid(ComponentType));
	auto& component_vector = component_pool[component_type];

	component_vector.push_back(new_object);

	return new_object;
}

template<typename TComponent>
auto ComponentFactory::getComponentVector() -> ComponentVector&
{
	static_assert(
	    is_component_v<TComponent>,
	    "TComponent must be a base class of Component");
	using ComponentType = typename std::remove_reference_t<TComponent>;

	auto type = static_cast<std::type_index>(typeid(ComponentType));

	auto& pool = this->component_pool;

	return pool.at(type);
}

template<typename TComponent>
auto ComponentFactory::getComponent(const Component::InstanceID& id_no)
    -> std::shared_ptr<TComponent>
{
	static_assert(
	    is_component_v<TComponent>,
	    "TComponent must be a base class of Component");
	using ComponentType = typename std::remove_reference_t<TComponent>;

	std::shared_ptr<ComponentType> result(nullptr);

	auto component_list = this->getComponentVector<ComponentType>();
	result =
	    std::static_pointer_cast<ComponentType>(component_list.at(id_no));

	return result;
}

}  // namespace elemental

// clang-format off
// vim: set textwidth=80 ts=8 sts=0 sw=8  noexpandtab ft=cpp.doxygen :
