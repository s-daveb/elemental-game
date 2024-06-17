/* GameSettings.hpp
 * Copyright © 2024 Saul D. Beniquez
 * License: Mozilla Public License v. 2.0
 *
 * This Source Code Form is subject to the terms of the Mozilla Public License,
 * v.2.0. If a copy of the MPL was not distributed with this file, You can
 * obtain one at https://mozilla.org/MPL/2.0/.
 */

#pragma once

#include "types/rendering.hpp"

#include "IOCore/util/toml.hpp"

#include <nlohmann/json.hpp>

namespace elemental {

struct GameSettings {
	RendererSettings renderer_settings;

	// DEFINE_TOML_FIELDS(renderer_settings);
	NLOHMANN_DEFINE_TYPE_INTRUSIVE(GameSettings, renderer_settings);
};

} // namespace elemental

// clang-format off
// vim: set foldmethod=syntax textwidth=80 ts=8 sts=0 sw=8  noexpandtab ft=cpp.doxygen :
