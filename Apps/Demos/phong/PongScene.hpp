/* PongScene.hpp
 * Copyright © 2026 Saul D. Beniquez
 * License: Mozilla Public License v. 2.0
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v.2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/.
 */

#pragma once

#include "BallPositionComponent.hpp"
#include "ComponentFactory.hpp"
#include "GameScene.hpp"
#include "PaddlePositionComponent.hpp"
#include "RectangleViewComponent.hpp"

#include <SDL_events.h>

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
	ComponentFactory<BallPositionComponent>   ball_pos_factory_;
	ComponentFactory<PaddlePositionComponent> paddle_pos_factory_;

	BallPositionComponent*   ball_pos_{ nullptr };
	PaddlePositionComponent* player_pos_{ nullptr };
	PaddlePositionComponent* enemy_pos_{ nullptr };

	CircleViewComponent*    ball_view_{ nullptr };
	RectangleViewComponent* player_view_{ nullptr };
	RectangleViewComponent* enemy_view_{ nullptr };

	int player_score_{ 0 };
	int enemy_score_{ 0 };

	uint32_t court_width_{ 1280 };
	uint32_t court_height_{ 720 };

	static constexpr uint32_t kPaddleWidth_{ 16 };
	static constexpr uint32_t kPaddleHeight_{ 96 };
	static constexpr uint32_t kBallRadius_{ 8 };

	auto resetBall() -> void;
	auto updateBall(float dt) -> void;
	auto updatePlayer(float dt) -> void;
	auto updateEnemy(float dt) -> void;
	auto checkCollisions() -> void;
	auto syncViewComponents() -> void;

	static constexpr float kPlayerX_{ 20.0f };
	float                  enemy_x_{ 0.0f };
};

}  // namespace elemental

// clang-format off
// vim: set textwidth=80 ts=8 sts=0 sw=8 noexpandtab ft=cpp.doxygen :