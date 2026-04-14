/* ShapeDrawCommand.hpp
 * Copyright © 2026 Saul D. Beniquez
 * License: Mozilla Public License v. 2.0
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v.2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/.
 */

#pragma once

#include "IDrawCommand.hpp"
#include "IRenderer.hpp"
#include "types/color.hpp"
#include "types/rendering.hpp"

#include <cstdint>
#include <memory>

namespace elemental {

enum class ShapeType : uint8_t
{
	FilledCircle,
	FilledRectangle
};

struct ShapeDrawCommand : public IDrawCommand
{
	ShapeType  shape_type;
	Rectangle  bounds;
	Color      color;
	IRenderer& renderer;

	ShapeDrawCommand(
	    ShapeType  shape_type,
	    Rectangle  bounds,
	    Color      color,
	    IRenderer& renderer)
	    : IDrawCommand(bounds, null_data), shape_type(shape_type),
	      bounds(bounds), color(color), renderer(renderer)
	{
	}

	auto rectangle() -> Rectangle& override { return bounds; }
	auto imageData() -> std::shared_ptr<void>& override
	{ return null_data; }

	auto draw() -> ErrorFlag override;

    private:
	static std::shared_ptr<void> null_data;
};

}  // namespace elemental

// clang-format off
// vim: set textwidth=80 ts=8 sts=0 sw=8 noexpandtab ft=cpp.doxygen :