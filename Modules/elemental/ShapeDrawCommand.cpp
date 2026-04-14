/* ShapeDrawCommand.cpp
 * Copyright © 2026 Saul D. Beniquez
 * License: Mozilla Public License v. 2.0
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v.2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/.
 */

#include "ShapeDrawCommand.hpp"

#include "IOCore/types/errors.hpp"

#include <cstdint>
#include <memory>

using namespace elemental;
using namespace IOCore;

namespace elemental {

std::shared_ptr<void> ShapeDrawCommand::null_data{ nullptr };

auto ShapeDrawCommand::draw() -> ErrorFlag
{
	try {
		switch (shape_type) {
		case ShapeType::FilledCircle: {
			auto cx = static_cast<int32_t>(
			    bounds.position.x + bounds.size.width / 2);
			auto cy = static_cast<int32_t>(
			    bounds.position.y + bounds.size.height / 2);
			auto r = static_cast<int32_t>(bounds.size.width / 2);
			renderer.drawFilledCircle(cx, cy, r, color);
			break;
		}
		case ShapeType::FilledRectangle:
			renderer.drawFilledRect(bounds, color);
			break;
		}
		return kSuccess;
	} catch (...) {
		return kError;
	}
}

}  // namespace elemental

// clang-format off
// vim: set textwidth=80 ts=8 sts=0 sw=8 noexpandtab ft=cpp.doxygen :