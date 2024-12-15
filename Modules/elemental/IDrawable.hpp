/* IDrawable.hpp
 * Copyright © 2023 Saul D. Beniquez
 * License: Mozilla Public License v. 2.0
 *
 * This Source Code Form is subject to the terms of the Mozilla Public License,
 * v.2.0. If a copy of the MPL was not distributed with this file, You can
 * obtain one at https://mozilla.org/MPL/2.0/.
 */

#pragma once

#include "External/IOCore/include/Exception.hpp"
#include "types.hpp"
#include "types/rendering.hpp"

#include <any>

namespace elemental {

struct IDrawable {
	IDrawable() = default;
	virtual ~IDrawable(){};

	virtual auto rectangle() const -> Rectangle&
	{
		throw IOCore::NotImplementedException();
	}
	virtual auto imageData() const -> std::any&
	{
		throw IOCore::NotImplementedException();
	}

	virtual auto draw() -> ErrorFlag
	{
		throw IOCore::NotImplementedException();
	}
	virtual auto draw(const Rectangle& rect) -> ErrorFlag = 0;
};

} // namespace elemental

// clang-format off
// vim: set foldmethod=syntax textwidth=80 ts=8 sts=0 sw=8  noexpandtab ft=cpp.doxygen :
