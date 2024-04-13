/* IDrawable.hpp
 * Copyright © 2023 Saul D. Beniquez
 * License: Mozilla Public License v. 2.0
 *
 * This Source Code Form is subject to the terms of the Mozilla Public License,
 * v.2.0. If a copy of the MPL was not distributed with this file, You can
 * obtain one at https://mozilla.org/MPL/2.0/.
 */

#pragma once

#include "types.hpp"
#include "types/rendering.hpp"

namespace elemental {

struct IDrawable
{
	virtual ~IDrawable(){};

	virtual void draw(const Rectangle& rect) = 0;

	void draw(Position2D& pos) { this->draw(Rectangle{ pos.x, pos.y }); }
};

} // namespace elemental

// clang-format off
// vim: set foldmethod=syntax textwidth=80 ts=8 sts=0 sw=8  noexpandtab ft=cpp.doxygen :
