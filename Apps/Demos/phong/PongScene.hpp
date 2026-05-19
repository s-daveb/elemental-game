/* PongScene.hpp
 * Copyright © 2026 Saul D. Beniquez
 * License: Mozilla Public License v. 2.0
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v.2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/.
 */

#pragma once

#include "IOCore/types/optional_ref.hpp"

#include "BallPositionComponent.hpp"
#include "ComponentFactory.hpp"
#include "GameScene.hpp"
#include "IViewComponent.hpp"
#include "PaddlePositionComponent.hpp"
#include "types/rendering.hpp"

#include <SDL_events.h>

#include <atomic>

namespace elemental {

class PongScene : public GameScene
{
	static constexpr EntityId kBallId   = 1;
	static constexpr EntityId kPlayerId = 2;
	static constexpr EntityId kEnemyId  = 3;

	static constexpr float kBallSpeed   = 300.0f;
	static constexpr float kPaddleSpeed = 400.0f;
	static constexpr float kEnemySpeed  = 250.0f;

    public:
	explicit PongScene(const SceneConfig& config);

	auto onUpdate() -> void override;
	auto onMessage(const Observable& sender, std::any message)
	    -> void override;

    private:
	ComponentFactory<BallPositionComponent>   ball_pos_factory;
	ComponentFactory<PaddlePositionComponent> paddle_pos_factory;

	IOCore::optional_ref<BallPositionComponent>   ball_pos_ref;
	IOCore::optional_ref<PaddlePositionComponent> player_pos_ref;
	IOCore::optional_ref<PaddlePositionComponent> enemy_pos_ref;

	IOCore::optional_ref<IViewComponent> ball_view_ref;
	IOCore::optional_ref<IViewComponent> player_view_ref;
	IOCore::optional_ref<IViewComponent> enemy_view_ref;

	FontHandle        menu_font;
	int               player_score{ 0 };
	int               enemy_score{ 0 };
	int               last_player_score{ -1 };
	int               last_enemy_score{ -1 };
	std::atomic<bool> is_paused{ false };

	auto resetBall() -> void;
	auto updateBall(float dt) -> void;
	auto updatePlayer(float dt) -> void;
	auto updateEnemy(float dt) -> void;
	auto checkCollisions() -> void;
	auto syncViewComponents() -> void;

	auto getDrawCommands()
	    -> std::list<std::shared_ptr<IDrawCommand>> override;

	void setFont(FontHandle f);

	uint32_t court_width{ 1280 };
	uint32_t court_height{ 720 };

	static constexpr uint32_t kPaddleWidth  = 16;
	static constexpr uint32_t kPaddleHeight = 96;
	static constexpr uint32_t kBallRadius   = 8;

	static constexpr float kPlayerX = 20.0f;
	float                  enemy_x{ 0.0f };
};

}  // namespace elemental

// clang-format off
// vim: set textwidth=80 ts=8 sts=0 sw=8 noexpandtab ft=cpp.doxygen :