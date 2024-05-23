/* Scene.cpp
 * Copyright © 2024 Saul D. Beniquez
 * License: Mozilla Public License v2.0 (MPL2)
 *
 * This Source Code Form is subject to the terms of the Mozilla Public License,
 * v.2.0. If a copy of the MPL was not distributed with this file, You can
 * obtain one at https://mozilla.org/MPL/2.0/.
 */

#include "Scene.hpp"

#include "JsonConfigFile.hpp"

namespace elemental {

using configuration::JsonConfigFile;

Scene::Scene(SDL_Renderer* renderer, JsonConfigFile& configFile)
    : renderer(renderer)
{
	auto config = configFile.jsonDataRef();
	dimensions = config["dimensions"].get<std::vector<int>>();
	layers = config["layers"].get<std::vector<std::string>>();
	for (const auto& entity : config["entities"]) {
		entities[entity["name"]] = std::make_shared<Entity>(
		    entity["type"],
		    entity["position"],
		    entity["size"],
		    entity["layer"]
		);
	}
	for (const auto& script : config["scripts"]) {
		scripts[script["name"]] = script["path"];
	}
}
Scene::~Scene() = default;

void Scene::loadResources()
{
	for (const auto& script : scripts) {
		load_script(script.second);
	}
}

void Scene::setupEntities()
{
	for (const auto& entity : entities) {
		entity.second->loadBehavior(scripts[entity.first]);
	}
}
