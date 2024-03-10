/* renderers.hpp
 * Copyright © 2024 Saul D. Beniquez
 * License: Mozilla Public License v. 2.0
 *
 * This Source Code Form is subject to the terms of the Mozilla Public License,
 * v.2.0. If a copy of the MPL was not distributed with this file, You can
 * obtain one at https://mozilla.org/MPL/2.0/.
 */

#pragma once

#include "util/serialization.hpp"

#include <cstdint>
#include <string>

namespace elemental {

struct Point
{
	uint32_t x, y;
	SERIALIZABLE(Point, x, y);
};
using Position2D = Point;

struct Area
{
	uint32_t width, height;
	SERIALIZABLE(Area, width, height);
};
using Resolution = Area;

struct Rectangle
{
	union
	{
		Point position;
		struct
		{
			uint32_t x, y;
		};
	};
	union
	{
		Area size;
		struct
		{
			uint32_t width, height;
		};
	};
	SERIALIZABLE(Rectangle, position, size);
};

enum class WindowMode
{
	Windowed = 0x00,
	Borderless = 0x01,
	Fullscreen = 0x11,
};
// NOLINTNEXTLINE(readability-identifier-length)
SERIALIZABLE_ENUM(WindowMode, { { WindowMode::Windowed, "windowed" },
                                { WindowMode::Borderless, "borderless" },
                                { WindowMode::Fullscreen, "fullscreen" } });
enum class WindowPlacement : int

{
	Manual = 0x00,
	Centered = 0x01
};
// NOLINTNEXTLINE(readability-identifier-length)
SERIALIZABLE_ENUM(WindowPlacement,
                  { { WindowPlacement::Manual, "manual" },
                    { WindowPlacement::Centered, "centered" } });

struct WindowParameters
{
	std::string title;
	WindowMode mode;
	WindowPlacement placement;
	Position2D position;
	Area size;

	SERIALIZABLE(WindowParameters, title, mode, placement, position, size);
};

struct RendererSettings
{
	WindowParameters window;
	Resolution resolution;

	SERIALIZABLE(RendererSettings, window, resolution);
};

} // namespace elemental
  // clang-format off
// vim: set foldmethod=syntax textwidth=80 ts=8 sts=0 sw=8  noexpandtab ft=cpp.doxygen :
