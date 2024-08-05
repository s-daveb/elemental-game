/* renderers.hpp
 * Copyright © 2024 Saul D. Beniquez
 * License: Mozilla Public License v. 2.0
 *
 * This Source Code Form is subject to the terms of the Mozilla Public License,
 * v.2.0. If a copy of the MPL was not distributed with this file, You can
 * obtain one at https://mozilla.org/MPL/2.0/.
 */

#pragma once

#include "IOCore/util/serialization.hpp"

#include <cstdint>
#include <string>

namespace elemental {

struct Point {
	uint32_t x, y;
	JSON_SERIALIZABLE(Point, x, y);
	TOML_SERIALIZABLE(Point, x, y);
};
using Position2D = Point;

struct Area {
	uint32_t width, height;
	JSON_SERIALIZABLE(Area, width, height);
	TOML_SERIALIZABLE(Area, width, height);
};
using Resolution = Area;

struct Rectangle {
	Point position;
	Area size;

	uint32_t& x = position.x;
	uint32_t& y = position.y;

	uint32_t& width = size.width;
	uint32_t& height = size.height;

	JSON_SERIALIZABLE(Rectangle, position, size);
	// TOML_SERIALIZABLE(Rectangle, position);
};

enum class WindowMode {
	Windowed = 0x00,
	Borderless = 0x01,
	Fullscreen = 0x11,
};
JSON_SERIALIZABLE_ENUM( // NOLINT(readability-identifier-length)
    WindowMode, { { WindowMode::Windowed, "Windowed" },
                  { WindowMode::Borderless, "Borderless" },
                  { WindowMode::Fullscreen, "Fullscreen" } }
);
enum class WindowPlacement : int { Manual = 0x00, Centered = 0x01 };

struct WindowParameters {
	std::string title;
	WindowMode mode;
	WindowPlacement placement;
	Position2D position;
	Area size;

	JSON_SERIALIZABLE(
	    WindowParameters, title, mode, placement, position, size
	);
};

struct RendererSettings {
	WindowParameters window;
	Resolution resolution;
	int field1;

	JSON_SERIALIZABLE(RendererSettings, window, resolution, field1);
};

} // namespace elemental
  // clang-format off
// vim: set foldmethod=syntax textwidth=80 ts=8 sts=0 sw=8  noexpandtab ft=cpp.doxygen :
