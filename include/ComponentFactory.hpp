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

#include <typeindex>
#include <unordered_map>
#include <vector>

namespace elemental {
class ComponentFactory
{
	using TypeInfo = Component::TypeInfo;
	using ComponentPtr = std::shared_ptr<Component>;
	using ComponentVector = std::vector<std::shared_ptr<Component>>;
	using ComponentPool = std::unordered_map<TypeInfo, ComponentVector>;
  public:
	template<typename T, typename... Args>
	std::shared_ptr<T> CreateComponent(const Component::InstanceID&,
	                                   Args&&...);

	template<typename T>
	std::shared_ptr<T> GetComponent(const Component::InstanceID&);

	template<typename T>
	ComponentVector& GetComponentVector(const TypeInfo&);

  private:
	ComponentPool component_pool;
};
} // namespace elemental

#define COMP_FACTORY_DECL
#include "details/ComponentFactory.impl.hpp"
#undef COMP_FACTORY_DECL
  // clang-format off
// vim: set foldmethod=syntax textwidth=80 ts=8 sts=0 sw=8 foldlevel=99 noexpandtab ft=cpp.doxygen :
