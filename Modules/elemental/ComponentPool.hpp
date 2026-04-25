/* ComponentPool.hpp
 * Copyright © 2026 Saul D. Beniquez
 * License: Mozilla Public License v. 2.0
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v.2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/.
 */

#pragma once

#include "IComponentFactory.hpp"

#include <typeindex>
#include <unordered_map>

namespace elemental {

class ComponentPool
{
	using FactoryRef = std::reference_wrapper<IComponentFactory>;

	std::unordered_map<std::type_index, FactoryRef> factories;

	ComponentPool() = default;

    public:
	static auto getInstance() noexcept -> ComponentPool&
	{
		static ComponentPool instance;
		return instance;
	}

	template<typename T>
	void registerFactory(IComponentFactoryRef factory) noexcept
	{ factories.emplace(std::type_index(typeid(T)), factory); }

	template<typename T>
	[[nodiscard]] auto getFactory() const -> IComponentFactory&
	{ return factories.at(std::type_index(typeid(T))); }

	template<typename T>
	[[nodiscard]] auto hasFactory() const -> bool
	{ return factories.count(std::type_index(typeid(T))) > 0; }
};

}  // namespace elemental

// clang-format off
// vim: set textwidth=80 ts=8 sts=0 sw=8 noexpandtab ft=cpp.doxygen :
