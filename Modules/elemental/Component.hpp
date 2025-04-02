/* Component.hpp
 * Copyright © 2024 Saul D. Beniquez
 * License: Mozilla Public License v. 2.0
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v.2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/.
 */

#pragma once

#include <typeindex>

namespace elemental {

class ComponentFactory;

struct Component {
	using TypeInfo = std::type_index;
	using InstanceID = unsigned int;
	friend class ComponentFactory;

	virtual ~Component() = default;

	auto getInstanceId() const -> InstanceID { return instance_id; }
	virtual auto getTypeIndex() -> TypeInfo = 0;

	template<typename TComponent>
	static constexpr auto is_child_class() -> bool
	{
		static_assert(
		    (std::is_base_of_v<Component, TComponent> ||
		     std::is_same_v<Component, TComponent>),
		    "T must be a derived class of Component"
		);
		return true;
	}

    protected:
	InstanceID instance_id;
	// unsigned int entity_id;
	ComponentFactory& factory;

	Component(ComponentFactory& owner)
	    : instance_id(next_instance_id++)
	    //	    , entity_id(0)
	    , factory(owner)
	{
	}

    private:
	static inline unsigned int next_instance_id = 0;
};

} // namespace elemental

// clang-format off
// vim: set foldmethod=syntax textwidth=80 ts=8 sts=0 sw=8 foldlevel=99 noexpandtab ft=cpp.doxygen :
