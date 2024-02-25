/* ComponentFactory.hpp
 * Copyright © 2024 Saul D. Beniquez
 * License:  Mozilla Public License v. 2.0
 *
 * This Source Code Form is subject to the terms of the Mozilla Public License,
 * v.2.0. If a copy of the MPL was not distributed with this file, You can
 * obtain one at https://mozilla.org/MPL/2.0/.
 */

#pragma once


#include "Exception.hpp"
#include "Component.hpp"

#ifndef  COMP_FACTORY_DECL
#include "ComponentFactory.hpp"
#endif

#include <unordered_map>
#include <vector>



namespace elemental {

	template<typename T, typename... Args>
	std::shared_ptr<T> ComponentFactory::CreateComponent(const Component::InstanceID& id,
	                                   Args&&... args)
	{

		auto new_object =
		    std::make_shared<T>(std::forward<Args>(args)...);

		auto component_type = std::type_index(typeid(T));
		auto& component_vector = component_pool[component_type];

		component_vector.push_back(new_object);

		return new_object;
	}

	template<typename T>
	std::shared_ptr<T> ComponentFactory::GetComponent(const Component::InstanceID& id) {  return nullptr; }


	ComponentVector& ComoponentFactory::GetComponentVector(const TypeInfo& type) {
		auto& pool = this->component_pool;

		return pool.at(type);
	}


} // namespace elemental
// clang-format off
// vim: set foldmethod=syntax textwidth=80 ts=8 sts=0 sw=8  noexpandtab ft=cpp.doxygen :
//
