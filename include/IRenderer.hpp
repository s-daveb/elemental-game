/* IRenderer.hpp
 * Copyright © 2023 Saul D. Beniquez
 * License: Mozilla Public License v. 2.0
 *
 * This Source Code Form is subject to the terms of the Mozilla Public License,
 * v.2.0. If a copy of the MPL was not distributed with this file, You can
 * obtain one at https://mozilla.org/MPL/2.0/ .
 */

#pragma once

#include "Singleton.template.hpp"
#include "types.hpp"

#include <type_traits>

#include <SDL.h>

namespace elemental {

struct Area
{
	uint32_t x, y, width, height;
};

struct IRenderer
{
	SINGLETON_INTERFACE(IRenderer);

	virtual ~IRenderer() {}

	bool Flip();

  protected:
	IRenderer() = default;
};

}

// clang-format off
// vim: set foldmethod=syntax textwidth=80 ts=8 sts=0 sw=8 foldlevel=99 noexpandtab ft=cpp.doxygen :
