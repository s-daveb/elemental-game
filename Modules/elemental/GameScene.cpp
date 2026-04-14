/* GameScene.cpp
 * Copyright © 2026 Saul D. Beniquez
 * License: Mozilla Public License v. 2.0
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v.2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/.
 */

#include "GameScene.hpp"

#include "IOCore/Exception.hpp"

#include "CircleViewComponent.hpp"
#include "ComponentPool.hpp"
#include "Observable.hpp"
#include "RectangleViewComponent.hpp"

#include <stdexcept>

namespace elemental {

GameScene::GameScene(const SceneConfig& config)
    : config_(config), renderer_(IRenderer::GetInstance<SdlRenderer>())
{
	auto& pool = ComponentPool::getInstance();
	pool.registerFactory<CircleViewComponent>(circle_factory_);
	pool.registerFactory<RectangleViewComponent>(rect_factory_);

	for (const auto& entity_cfg: config_.entities) {
		Entity entity;
		entity.id   = entity_cfg.entity_id != 0 ? entity_cfg.entity_id
		                                        : next_entity_id_++;
		entity.name = entity_cfg.name;
		entity.velocity = entity_cfg.velocity;

		const auto& view = entity_cfg.view;
		if (view.shape == "circle") {
			auto& comp = circle_factory_.create(
			    Point{ entity_cfg.x, entity_cfg.y },
			    view.radius,
			    view.color);
			entity.views.push_back(&comp);
		} else if (view.shape == "rectangle") {
			auto& comp = rect_factory_.create(
			    Point{ entity_cfg.x, entity_cfg.y },
			    view.width,
			    view.height,
			    view.color);
			entity.views.push_back(&comp);
		}

		entities_[entity.id] = std::move(entity);
	}
}

auto GameScene::step() -> void
{ onUpdate(); }

auto GameScene::getDrawCommands() -> std::list<std::shared_ptr<IDrawCommand>>
{
	std::list<std::shared_ptr<IDrawCommand>> commands;
	for (auto& [id, entity]: entities_) {
		for (auto* view: entity.views) {
			commands.push_back(view->produceDrawCommand(renderer_));
		}
	}
	return commands;
}

void GameScene::recieveMessage(const Observable& sender, std::any message)
{ onMessage(sender, std::move(message)); }

auto GameScene::getEntity(EntityId id) -> Entity&
{ return entities_.at(id); }

auto GameScene::getEntity(EntityId id) const -> const Entity&
{ return entities_.at(id); }

auto GameScene::getEntityByName(const std::string& name) -> Entity&
{
	for (auto& [id, entity]: entities_) {
		if (entity.name == name) { return entity; }
	}
	throw std::runtime_error("Entity not found: " + name);
}

}  // namespace elemental

// clang-format off
// vim: set textwidth=80 ts=8 sts=0 sw=8 noexpandtab ft=cpp.doxygen :