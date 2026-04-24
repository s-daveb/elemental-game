/* IComponent.hpp
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

class IComponentFactory;

struct IComponent
{
	using TypeInfo   = std::type_index;
	using InstanceID = unsigned int;

	virtual ~IComponent() = default;

	[[nodiscard]] virtual auto getInstanceId() const -> InstanceID      = 0;
	[[nodiscard]] virtual auto getTypeIndex() const -> TypeInfo         = 0;
	[[nodiscard]] virtual auto getFactory() const -> IComponentFactory& = 0;
};

using IComponentRef = std::reference_wrapper<IComponent>;

}  // namespace elemental
