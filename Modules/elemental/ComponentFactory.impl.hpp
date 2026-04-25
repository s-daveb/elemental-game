/* ComponentFactory.impl.hpp
 * Copyright © 2024 Saul D. Beniquez
 * License: Mozilla Public License v. 2.0
 */

#pragma once

#include "IOCore/Exception.hpp"

#include "ComponentFactory.hpp"

#include <string>

namespace elemental {

template<typename TComponent, std::size_t Capacity>
void ComponentFactory<TComponent, Capacity>::reset() noexcept
{
	for (auto& opt: components) {
		if (opt.has_value()) { opt.reset(); }
	}
	size = 0;
}

template<typename TComponent, std::size_t Capacity>
template<typename... TArgs>
auto ComponentFactory<TComponent, Capacity>::create(TArgs&&... args)
    -> TComponent&
{
	if (size >= Capacity) {
		throw IOCore::Exception(
		    "Component capacity exceeded for type " +
		    std::string(typeid(TComponent).name()) +
		    "(Capacity: " + std::to_string(Capacity) + ")");
	}

	auto& slot = components[size++];
	slot.emplace(*this, std::forward<TArgs>(args)...);

	return slot.value();
}

template<typename TComponent, std::size_t Capacity>
auto ComponentFactory<TComponent, Capacity>::get(size_t index) -> TComponent&
{
	if (index >= size || !components[index].has_value()) {
		throw std::out_of_range("ComponentFactory::get: invalid index");
	}

	return components[index].value();
}

template<typename TComponent, std::size_t Capacity>
auto ComponentFactory<TComponent, Capacity>::get(size_t index) const
    -> const TComponent&
{
	if (index >= size || !components[index].has_value()) {
		throw std::out_of_range("ComponentFactory::get: invalid index");
	}

	return components[index].value();
}

template<typename TComponent, std::size_t Capacity>
auto ComponentFactory<TComponent, Capacity>::getUnchecked(size_t index)
    -> TComponent&
{ return components[index].value(); }

template<typename TComponent, std::size_t Capacity>
auto ComponentFactory<TComponent, Capacity>::getUnchecked(size_t index) const
    -> const TComponent&
{ return components[index].value(); }

}  // namespace elemental
