/* IDrawable.hpp
 * Copyright © 2023 Saul D. Beniquez
 * License: Mozilla Public License v. 2.0
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v.2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/.
 */

#pragma once

#include "IOCore/Exception.hpp"
#include "IOCore/types/errors.hpp"

#include "IRenderer.hpp"

#include "types.hpp"
#include "types/rendering.hpp"

#include <any>

namespace elemental {

using IOCore::ErrorFlag;

class IRenderer;

/// @todo move this to its own file
struct IDrawable {
	virtual ~IDrawable() {};

	virtual auto draw(const Rectangle& location) -> ErrorFlag = 0;

    protected:
	IDrawable(IRenderer& renderer) {}
};

} // namespace elemental

// clang-format off
// vim: set textwidth=80 ts=8 sts=0 sw=8  noexpandtab ft=cpp.doxygen :
