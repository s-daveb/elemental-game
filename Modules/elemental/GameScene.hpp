/* GameScene.hpp
 * Copyright © 2026 Saul D. Beniquez
 * License: Mozilla Public License v. 2.0
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v.2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/.
 */

#pragma once

#include "CircleViewComponent.hpp"
#include "ComponentFactory.hpp"
#include "IRenderer.hpp"
#include "IState.hpp"
#include "RectangleViewComponent.hpp"
#include "SceneConfig.hpp"
#include "SdlRenderer.hpp"
#include "Singleton.hpp"
#include "types/id.hpp"

#include <any>
#include <functional>
#include <list>
#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

namespace elemental {

class Observable;

class GameScene : public IState
{
    public:
	using EntityId = idtype_t;

	struct Entity
	{
		EntityId                                            id{ 0 };
		std::string                                         name;
		std::vector<std::reference_wrapper<IViewComponent>> views_ref;
		VelocityConfig                                      velocity;
	};

	explicit GameScene(const SceneConfig& config);
	~GameScene() override = default;

	auto step() -> void override;
	auto getDrawCommands()
	    -> std::list<std::shared_ptr<IDrawCommand>> override;
	auto receiveMessage(const Observable& sender, std::any message)
	    -> void override;

	auto getEntity(EntityId id) -> Entity&;
	auto getEntity(EntityId id) const -> const Entity&;
	auto getEntityByName(const std::string& name) -> Entity&;

	auto& getCircleFactory() { return circle_factory; }
	auto& getRectFactory() { return rect_factory; }

    protected:
	virtual auto onUpdate() -> void {}
	virtual auto onMessage(const Observable& sender, std::any message)
	    -> void
	{
	}

	SceneConfig config;

	ComponentFactory<CircleViewComponent>    circle_factory;
	ComponentFactory<RectangleViewComponent> rect_factory;

	std::unordered_map<EntityId, Entity> entities;
	EntityId                             next_entity_id{ 1 };

	IRenderer& renderer;
};

}  // namespace elemental

// clang-format off
// vim: set textwidth=80 ts=8 sts=0 sw=8 noexpandtab ft=cpp.doxygen :