/* SceneConfig.hpp
 * Copyright © 2026 Saul D. Beniquez
 * License: Mozilla Public License v. 2.0
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v.2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/.
 */

#pragma once

#include "IOCore/util/serialization.hpp"

#include "types/color.hpp"
#include "types/id.hpp"
#include "types/rendering.hpp"

#include <nlohmann/json.hpp>
#include <string>
#include <vector>

namespace elemental {

struct ViewConfig
{
	std::string shape;  // "circle" | "rectangle"
	uint32_t    radius{ 0 };
	uint32_t    width{ 0 };
	uint32_t    height{ 0 };
	Color       color{ 255, 255, 255, 255 };

	JSON_SERIALIZABLE(ViewConfig, shape, radius, width, height, color)
};

struct VelocityConfig
{
	float vx{ 0.0f };
	float vy{ 0.0f };

	JSON_SERIALIZABLE(VelocityConfig, vx, vy)
};

struct EntityConfig
{
	idtype_t                 entity_id{ 0 };
	std::string              name;
	std::string              type;
	uint32_t                 x{ 0 };
	uint32_t                 y{ 0 };
	std::string              layer;
	ViewConfig               view;
	VelocityConfig           velocity;
	std::vector<std::string> scripts;

	JSON_SERIALIZABLE(
	    EntityConfig,
	    entity_id,
	    name,
	    type,
	    x,
	    y,
	    layer,
	    view,
	    velocity,
	    scripts)
};

struct SceneConfig
{
	std::string               type;
	std::string               kind;
	Area                      dimensions{ 1280, 720 };
	std::vector<std::string>  layers;
	std::vector<EntityConfig> entities;

	JSON_SERIALIZABLE(SceneConfig, type, kind, dimensions, layers, entities)
};

}  // namespace elemental

// clang-format off
// vim: set textwidth=80 ts=8 sts=0 sw=8 noexpandtab ft=cpp.doxygen :