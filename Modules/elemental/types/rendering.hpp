/* renderers.hpp
 * Copyright © 2024 Saul D. Beniquez
 * License: Mozilla Public License v. 2.0
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v.2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/.
 */

#pragma once

#include "IOCore/util/serialization.hpp"

#include <cstdint>
#include <memory>
#include <string>

namespace elemental {

/// Type-safe handle for font resources (type-erased TTF_Font)
using FontHandle = std::shared_ptr<void>;

struct Point
{
	uint32_t x, y;
	TOML_CLASS(Point, x, y);
	JSON_SERIALIZABLE(Point, x, y)
};
using Position2D = Point;

struct Area
{
	uint32_t width, height;
	TOML_CLASS(Area, width, height);
	JSON_SERIALIZABLE(Area, width, height)
};
using Resolution = Area;

struct Rectangle
{
	Rectangle() = default;
	Rectangle(Position2D position, Area size = { 0, 0 })
	    : position(position), size(size)
	{
	}
	Rectangle(unsigned x, unsigned y, unsigned width, unsigned height)
	    : position({ x, y }), size({ width, height })
	{
	}

	Rectangle(const Rectangle& other)
	    : position(other.position), size(other.size)
	{
	}
	auto operator=(const Rectangle& other) -> Rectangle&
	{
		position = other.position;
		size     = other.size;
		return *this;
	}
	auto operator==(const Rectangle& other) const -> bool
	{
		return position.x == other.position.x &&
		       position.y == other.position.y &&
		       size.width == other.size.width &&
		       size.height == other.size.height;
	}

	Point position;
	Area  size;

	uint32_t& x = position.x;
	uint32_t& y = position.y;

	uint32_t& width  = size.width;
	uint32_t& height = size.height;

	TOML_CLASS(Rectangle, position, size);
};

enum WindowMode
{
	Windowed   = 0x00,
	Borderless = 0x01,
	Fullscreen = 0x11,
};

TOML_ENUM(WindowMode, Windowed, Borderless, Fullscreen);

enum WindowPlacement
{
	Manual,
	Centered
};
TOML_ENUM(WindowPlacement, Manual, Centered);

struct WindowParameters
{
	std::string title;

	WindowMode      mode;
	WindowPlacement placement;

	Position2D position;
	Area       size;

	TOML_CLASS(WindowParameters, title, mode, placement, position, size);
};

struct RendererSettings
{
	WindowParameters window;
	Resolution       resolution;

	TOML_CLASS(RendererSettings, window);  // resolution);
};

}  // namespace elemental

// clang-format off
// vim: set textwidth=80 ts=8 sts=0 sw=8  noexpandtab ft=cpp.doxygen :
