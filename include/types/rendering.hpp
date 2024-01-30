/* renderers.hpp
 * Copyright © 2024 Saul D. Beniquez
 * License: Mozilla Public License v. 2.0
 *
 * This Source Code Form is subject to the terms of the Mozilla Public License,
 * v.2.0. If a copy of the MPL was not distributed with this file, You can
 * obtain one at https://mozilla.org/MPL/2.0/.
 */

#pragma once

#include <cstdint>
#include <nlohmann/json.hpp>
#include <string>

namespace elemental {

struct Point
{
	uint32_t x, y;
	NLOHMANN_DEFINE_TYPE_INTRUSIVE(Point, x, y);
};
using Position = Point;

struct Area
{
	uint32_t width, height;

	NLOHMANN_DEFINE_TYPE_INTRUSIVE(Area, width, height);
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
	NLOHMANN_DEFINE_TYPE_INTRUSIVE(Rectangle, position, size);
};

enum class WindowMode
{
	Windowed = 0x00,
	Borderless = 0x01,
	Fullscreen = 0x11,
};
NLOHMANN_JSON_SERIALIZE_ENUM(WindowMode,
                             { { WindowMode::Windowed, "windowed" },
                               { WindowMode::Borderless, "borderless" },
                               { WindowMode::Fullscreen, "fullscreen" } });
enum class WindowPlacement

{
	Manual = 0x00,
	Centered = 0x01
};
NLOHMANN_JSON_SERIALIZE_ENUM(WindowPlacement,
                             { { WindowPlacement::Manual, "manual" },
                               { WindowPlacement::Centered, "centered" } });

struct WindowParameters
{
	std::string title;
	WindowMode mode;
	WindowPlacement placement;
	Position position;
	Area size;

	NLOHMANN_DEFINE_TYPE_INTRUSIVE(WindowParameters, title, mode, placement,
	                               position, size);
};

struct RendererSettings
{
	WindowParameters window;
	Resolution resolution;

	NLOHMANN_DEFINE_TYPE_INTRUSIVE(RendererSettings, window, resolution);
};

} // namespace elemental
  // clang-format off
// vim: set foldmethod=syntax textwidth=80 ts=8 sts=0 sw=8 foldlevel=99 noexpandtab ft=cpp.doxygen :
