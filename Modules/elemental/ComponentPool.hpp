/* ComponentPool.hpp
 * Copyright © 2024 Saul D. Beniquez
 * License: Mozilla Public License v. 2.0
 */

#pragma once

#include "IComponentFactory.hpp"

#include <memory>
#include <stdexcept>
#include <unordered_map>

namespace elemental {

using IComponentFactoryRef = std::reference_wrapper<IComponentFactory>;

class ComponentPool
{
    private:
	std::unordered_map<std::type_index, IComponentFactoryRef> factories_{};

    public:
	static auto getInstance() noexcept -> ComponentPool&
	{
		static ComponentPool instance{};
		return instance;
	}

	template<typename T>
	bool isRegistered() const noexcept
	{
		using TypeID = std::type_index;
		return getInstance().factories_.contains(typeid(T));
	}

	template<typename T>
	void registerFactory(IComponentFactoryRef factory) noexcept
	{
		getInstance().factories_.insert_or_assign(
		    typeid(T), std::move(factory));
	}

	template<typename T>
	IComponentFactoryRef getFactory() const&
	{
		auto& fact = getInstance().factories_;
		auto  it   = fact.find(typeid(T));
		if (it == fact.end()) {
			throw std::runtime_error(
			    "ComponentFactory not registered for type " +
			    std::string(typeid(T).name()));
		}
		return it->second;
	}

    private:
	ComponentPool() = default;
};

}  // namespace elemental
