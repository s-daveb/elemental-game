/* ComponentFactory.hpp
 * Copyright © 2024 Saul D. Beniquez
 * License:  Mozilla Public License v. 2.0
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v.2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/.
 */

#pragma once

#include "Component.hpp"
#include "IOCore/Exception.hpp"

#include <memory>
#include <typeindex>
#include <unordered_map>
#include <vector>

namespace elemental {
struct ComponentFactory {
	using TypeInfo = Component::TypeInfo;
	using ComponentPtr = std::shared_ptr<Component>;
	using ComponentVector = std::vector<std::shared_ptr<Component>>;
	using ComponentPool = std::unordered_map<TypeInfo, ComponentVector>;

	virtual ~ComponentFactory();

	template<typename TComponent, typename... TArgs>
	auto createComponent(TArgs&&...) -> std::shared_ptr<TComponent>;

	template<typename TComponent>
	auto getComponentVector() -> ComponentVector&;

	template<typename TComponent>
	auto getComponent(const Component::InstanceID&)
	    -> std::shared_ptr<TComponent>;

    private:
	ComponentPool component_pool;
};
} // namespace elemental

#define COMP_FACTORY_DECL
#include "ComponentFactory.impl.hpp"
#undef COMP_FACTORY_DECL
  // clang-format off
// vim: set textwidth=80 ts=8 sts=0 sw=8 foldlevel=99 noexpandtab ft=cpp.doxygen :
