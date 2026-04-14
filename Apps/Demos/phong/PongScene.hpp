/* PongScene.hpp
 * Copyright © 2026 Saul D. Beniquez
 * License: Mozilla Public License v. 2.0
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v.2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/.
 */

#pragma once

#include "CircleViewComponent.hpp"
#include "GameScene.hpp"
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
	CircleViewComponent*    ball_{ nullptr };
	RectangleViewComponent* player_paddle_{ nullptr };
	RectangleViewComponent* enemy_paddle_{ nullptr };

	struct
	{
		float x{ 640.0f };
		float y{ 360.0f };
		float vx{ kBallSpeed };
		float vy{ kBallSpeed * 0.6f };
	} ball_state_;

	struct
	{
		float y{ 296.0f };
		float speed{ kPaddleSpeed };
		bool  up_pressed{ false };
		bool  down_pressed{ false };
	} player_;

	struct
	{
		float y{ 296.0f };
		float speed{ kEnemySpeed };
	} enemy_;

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