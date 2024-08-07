/* Component.hpp
 * Copyright © 2024 Saul D. Beniquez
 * License: Mozilla Public License v. 2.0
 *
 * This Source Code Form is subject to the terms of the Mozilla Public License,
 * v.2.0. If a copy of the MPL was not distributed with this file, You can
 * obtain one at https://mozilla.org/MPL/2.0/.
 */

#pragma once

#include <typeindex>

namespace elemental {

class ComponentFactory;

struct Component
{
  public:
	using TypeInfo = std::type_index;

	// using EntityId = unsigned int;
	using InstanceID = unsigned int;

	friend class ComponentFactory;

	virtual ~Component() = default;

	auto getInstanceId() const -> InstanceID { return instance_id; }
	virtual auto getTypeIndex() -> TypeInfo = 0;

	template<typename T_>
	static auto isChildClass() -> bool
	{
		static_assert(std::is_base_of_v<Component, T_>,
		              "T must be a derived class of Component");
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
	static unsigned int next_instance_id;
};

unsigned int Component::next_instance_id = 0;

} // namespace elemental
  // clang-format off
// vim: set foldmethod=syntax textwidth=80 ts=8 sts=0 sw=8 foldlevel=99 noexpandtab ft=cpp.doxygen :
