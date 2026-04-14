/* IComponentFactory.hpp
 * Copyright © 2024 Saul D. Beniquez
 * License: Mozilla Public License v. 2.0
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v.2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/.
 */

#pragma once

#include "IComponent.hpp"

#include <typeindex>

namespace elemental {

struct IComponentFactory
{
	using TypeInfo = std::type_index;
	using SizeType = size_t;

	virtual ~IComponentFactory() = default;

	[[nodiscard]] virtual auto getTypeID() const -> TypeInfo = 0;
	[[nodiscard]] virtual auto size() const -> SizeType      = 0;
	[[nodiscard]] virtual auto capacity() const -> SizeType  = 0;
	[[nodiscard]] virtual auto empty() const -> bool { return size() == 0; }

	virtual auto clear() -> void = 0;
};

}  // namespace elemental
