/* color.hpp
 * Copyright © 2026 Saul D. Beniquez
 * License: Mozilla Public License v. 2.0
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v.2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/.
 */

#pragma once

#include "IOCore/util/serialization.hpp"

#include <cstdint>

namespace elemental {

struct Color
{
	uint8_t r{ 255 };
	uint8_t g{ 255 };
	uint8_t b{ 255 };
	uint8_t a{ 255 };

	Color() = default;
	Color(uint8_t r, uint8_t g, uint8_t b, uint8_t a = 255)
	    : r(r), g(g), b(b), a(a)
	{
	}

	JSON_SERIALIZABLE(Color, r, g, b, a)
};

}  // namespace elemental

// clang-format off
// vim: set textwidth=80 ts=8 sts=0 sw=8 noexpandtab ft=cpp.doxygen :