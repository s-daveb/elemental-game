/* TextDrawCommand.hpp
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
#include <string>

namespace elemental {

struct TextDrawCommand : public IDrawCommand
{
	std::string                  text;
	Rectangle                    bounds;
	Color                        color;
	IRenderer&                   renderer;
	static std::shared_ptr<void> null_data;

	TextDrawCommand(
	    const std::string& text_arg,
	    Rectangle          bounds_arg,
	    Color              color_arg,
	    IRenderer&         renderer_arg)
	    : IDrawCommand(bounds_arg, null_data), text(text_arg),
	      bounds(bounds_arg), color(color_arg), renderer(renderer_arg)
	{
	}

	auto rectangle() -> Rectangle& override { return bounds; }
	auto imageData() -> std::shared_ptr<void>& override
	{ return null_data; }

	auto draw() -> ErrorFlag override;
	void prepare(FontHandle font);
};

}  // namespace elemental

// clang-format off
// vim: set textwidth=80 ts=8 sts=0 sw=8 noexpandtab ft=cpp.doxygen :
