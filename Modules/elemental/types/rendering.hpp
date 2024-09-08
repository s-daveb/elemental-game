/* renderers.hpp
 * Copyright © 2024 Saul D. Beniquez
 * License: Mozilla Public License v. 2.0
 *
 * This Source Code Form is subject to the terms of the Mozilla Public License,
 * v.2.0. If a copy of the MPL was not distributed with this file, You can
 * obtain one at https://mozilla.org/MPL/2.0/.
 */

#pragma once

#include "IOCore/util/toml.hpp"

#include <cstdint>
#include <string>

namespace elemental {

struct Point {
	uint32_t x, y;
	TOML_CLASS(Point, x, y);
};
using Position2D = Point;

struct Area {
	uint32_t width, height;
	TOML_CLASS(Area, width, height);
};
using Resolution = Area;

struct Rectangle {
	Point position;
	Area size;

	uint32_t& x = position.x;
	uint32_t& y = position.y;

	uint32_t& width = size.width;
	uint32_t& height = size.height;

	TOML_CLASS(Rectangle, position, size);
};

enum class WindowMode {
	Windowed = 0x00,
	Borderless = 0x01,
	Fullscreen = 0x11,
};
TOML_ENUM(
    WindowMode, WindowMode::Windowed, WindowMode::Borderless,
    WindowMode::Fullscreen
);

enum WindowPlacement { Manual, Centered };
TOML_ENUM(WindowPlacement, Manual, Centered);

struct WindowParameters {
	std::string title;

	WindowMode mode;
	WindowPlacement placement;

	Position2D position;
	Area size;

	TOML_CLASS(WindowParameters, title, mode, placement, position, size);
};

struct RendererSettings {
	WindowParameters window;
	Resolution resolution;

	TOML_CLASS(RendererSettings, window); // resolution);
};

} // namespace elemental

// clang-format off
// vim: set foldmethod=syntax textwidth=80 ts=8 sts=0 sw=8  noexpandtab ft=cpp.doxygen :
