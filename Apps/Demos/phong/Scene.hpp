/* Scene.hpp
 * Copyright © 2024 Saul D. Beniquez
 * License: Mozilla Public License v2.0 (MPL2)
 *
 * This Source Code Form is subject to the terms of the Mozilla Public License,
 * v.2.0. If a copy of the MPL was not distributed with this file, You can
 * obtain one at https://mozilla.org/MPL/2.0/.
 */

#pragma once

#include "JsonConfigFile.hpp"
#include "types/rendering.hpp"

#include <SDL.h>
#include <nlohmann/json.hpp>

#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

namespace elemental {

namespace {
using configuration::JsonConfigFile;
}

class Entity;

class Scene {
	using Dimensions = Area;

    public:
	// Constructor from JSON
	Scene(SDL_Renderer* renderer, JsonConfigFile& config);
	virtual ~Scene();

	// Resource loading and entity setup methods
	void loadResources();
	void setupEntities();

    private:
	SDL_Renderer* renderer;
	Dimensions dimensions;
	std::vector<std::string> layers;
	std::unordered_map<std::string, std::shared_ptr<Entity>> entities;
	std::unordered_map<std::string, std::string> scripts;

	// Placeholder for script handling
	void load_script(const std::string& script);
};
} // namespace elemental

// clang-format off
// vim: set foldmethod=syntax foldminlines=10 textwidth=80 ts=8 sts=0 sw=8 noexpandtab ft=cpp.doxygen :
