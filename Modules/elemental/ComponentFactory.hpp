/* ComponentFactory.hpp
 * Copyright © 2024 Saul D. Beniquez
 * License: Mozilla Public License v. 2.0
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v.2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/.
 */

#pragma once

#include "IComponent.hpp"
#include "IComponentFactory.hpp"

#include <array>
#include <memory>
#include <optional>
#include <type_traits>

namespace elemental {

/* Reference wrapper type for IComponentFactory */
using IComponentFactoryRef = std::reference_wrapper<IComponentFactory>;

/* ComponentVector alias using fixed-size array */
template<std::size_t Capacity = 256>
using ComponentVector =
    std::array<std::optional<IComponentFactoryRef>, Capacity>;

/* Forward declaration for friend access */
class ComponentPool;

/**
 * Template component factory implementing IComponentFactory
 *
 * Provides compile-time typed interface for managing TComponent instances.
 * Stores up to `Capacity` components in a fixed-size array.
 */
template<typename TComponent, std::size_t Capacity = 256>
class ComponentFactory : public IComponentFactory
{
	static_assert(
	    std::is_base_of_v<IComponent, TComponent>,
	    "TComponent must derive from IComponent");

    public:
	using TypeInfo   = typename IComponentFactory::TypeInfo;
	using SizeType   = typename IComponentFactory::SizeType;
	using Components = std::array<std::optional<TComponent>, Capacity>;

    private:
	Components components_{};
	TypeInfo   type_id_;
	size_t     size_{ 0 };

    public:
	constexpr ComponentFactory() noexcept : type_id_(typeid(TComponent)) {}
	~ComponentFactory() override = default;

	[[nodiscard]] auto getTypeID() const -> TypeInfo override
	{ return type_id_; }
	[[nodiscard]] auto size() const -> SizeType override { return size_; }
	[[nodiscard]] auto capacity() const -> SizeType override
	{ return Capacity; }

	[[nodiscard]] auto empty() const -> bool override { return size_ == 0; }

	auto clear() -> void override { reset(); }

	/* Create a new component with forwarded arguments */
	template<typename... TArgs>
	auto create(TArgs&&... args) -> TComponent&;

	[[nodiscard]] auto get(size_t index) -> TComponent&;
	[[nodiscard]] auto get(size_t index) const -> const TComponent&;

	[[nodiscard]] auto getUnchecked(size_t index) -> TComponent&;
	[[nodiscard]] auto getUnchecked(size_t index) const
	    -> const TComponent&;

	/* Iterator support */
	auto begin() -> decltype(components_.begin())
	{ return components_.begin(); }
	auto end() -> decltype(components_.end()) { return components_.end(); }
	auto begin() const -> decltype(components_.begin()) const
	{ return components_.begin(); }
	auto end() const -> decltype(components_.end()) const
	{ return components_.end(); }

    private:
	void reset() noexcept;

	friend class ComponentPool;
};

}  // namespace elemental

#include "ComponentFactory.impl.hpp"
