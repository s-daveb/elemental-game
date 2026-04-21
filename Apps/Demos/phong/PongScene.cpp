/* PongScene.cpp
 * Copyright © 2026 Saul D. Beniquez
 * License: Mozilla Public License v. 2.0
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v.2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/.
 */

#include "PongScene.hpp"

#include "CircleViewComponent.hpp"
#include "ComponentPool.hpp"
#include "Observable.hpp"
#include "RectangleViewComponent.hpp"

#include <SDL_events.h>

#include <cmath>

namespace elemental {

PongScene::PongScene(const SceneConfig& config) : GameScene(config)
{
	auto& pool = ComponentPool::getInstance();
	pool.registerFactory<BallPositionComponent>(ball_pos_factory);
	pool.registerFactory<PaddlePositionComponent>(paddle_pos_factory);

	constexpr float half_paddle = static_cast<float>(kPaddleHeight) / 2.0f;

	ball_pos_ptr = &ball_pos_factory.create(
	    static_cast<float>(court_width) / 2.0f,
	    static_cast<float>(court_height) / 2.0f,
	    kBallSpeed,
	    kBallSpeed * 0.6f,
	    static_cast<float>(kBallRadius));

	player_pos_ptr = &paddle_pos_factory.create(
	    static_cast<float>(court_height) / 2.0f - half_paddle,
	    kPaddleSpeed);

	enemy_pos_ptr = &paddle_pos_factory.create(
	    static_cast<float>(court_height) / 2.0f - half_paddle, kEnemySpeed);

	auto& ball_entity   = getEntity(kBallId);
	auto& player_entity = getEntity(kPlayerId);
	auto& enemy_entity  = getEntity(kEnemyId);

	if (!ball_entity.views_ptr.empty()) {
		ball_view_ptr = dynamic_cast<const CircleViewComponent*>(
		    ball_entity.views_ptr[0]);
	}
	if (!player_entity.views_ptr.empty()) {
		player_view_ptr = dynamic_cast<const RectangleViewComponent*>(
		    player_entity.views_ptr[0]);
	}
	if (!enemy_entity.views_ptr.empty()) {
		enemy_view_ptr = dynamic_cast<const RectangleViewComponent*>(
		    enemy_entity.views_ptr[0]);
	}

	auto resolution = renderer.getResolution();
	court_width     = resolution.width;
	court_height    = resolution.height;
	enemy_x         = static_cast<float>(court_width) - 36.0f;

	// TODO: Refactor position components to use references instead of
	// const pointers. The position components need mutable access for
	// state updates, which is incompatible with const pointer observation.
	// Using dynamic_cast and non-const raw pointers violates coding
	// standards. Components should be accessed via factory references.
}

auto PongScene::onUpdate() -> void
{
	static constexpr float kDt = 1.0f / 60.0f;

	// TODO: Refactor - position update methods need mutable component
	// access updateBall(kDt); updatePlayer(kDt); updateEnemy(kDt);
	// checkCollisions();
	// syncViewComponents();
}

auto PongScene::onMessage(const Observable& sender, std::any message) -> void
{
	// TODO: Refactor - needs mutable paddle_pos_ptr for setter calls
	if (!message.has_value()) { return; }

	auto event = std::any_cast<SDL_Event>(message);
	if (event.type == SDL_KEYDOWN) {
		if (event.key.keysym.scancode == SDL_SCANCODE_UP) {
			// TODO: player_pos_ptr->setUpPressed(true);
		}
		if (event.key.keysym.scancode == SDL_SCANCODE_DOWN) {
			// TODO: player_pos_ptr->setDownPressed(true);
		}
	}
	if (event.type == SDL_KEYUP) {
		if (event.key.keysym.scancode == SDL_SCANCODE_UP) {
			// TODO: player_pos_ptr->setUpPressed(false);
		}
		if (event.key.keysym.scancode == SDL_SCANCODE_DOWN) {
			// TODO: player_pos_ptr->setDownPressed(false);
		}
	}
}

auto PongScene::resetBall() -> void
{
	// TODO: Refactor - needs mutable ball_pos_ptr for setter calls
	// ball_pos_ptr->setX(static_cast<float>(court_width) / 2.0f);
	// ball_pos_ptr->setY(static_cast<float>(court_height) / 2.0f);
	// ball_pos_ptr->setVx(kBallSpeed);
	// ball_pos_ptr->setVy(kBallSpeed * 0.6f);
}

auto PongScene::updateBall(float dt) -> void
{
	// TODO: Refactor - needs mutable ball_pos_ptr for setter calls
	// ball_pos_ptr->setX(ball_pos_ptr->x() + ball_pos_ptr->vx() * dt);
	// ball_pos_ptr->setY(ball_pos_ptr->y() + ball_pos_ptr->vy() * dt);
	//
	// auto half_r = ball_pos_ptr->radius();
	// if (ball_pos_ptr->y() - half_r <= 0.0f) {
	// 	ball_pos_ptr->setVy(std::abs(ball_pos_ptr->vy()));
	// 	ball_pos_ptr->setY(half_r);
	// }
	// if (ball_pos_ptr->y() + half_r >= static_cast<float>(court_height))
	// {
	// 	ball_pos_ptr->setVy(-std::abs(ball_pos_ptr->vy()));
	// 	ball_pos_ptr->setY(static_cast<float>(court_height) -
	// half_r);
	// }
	//
	// if (ball_pos_ptr->x() <= 0.0f) {
	// 	enemy_score++;
	// 	resetBall();
	// }
	// if (ball_pos_ptr->x() >= static_cast<float>(court_width)) {
	// 	player_score++;
	// 	resetBall();
	// }
}

auto PongScene::updatePlayer(float dt) -> void
{
	// TODO: Refactor - needs mutable player_pos_ptr for setter calls
	// if (player_pos_ptr->upPressed()) {
	// 	player_pos_ptr->setY(player_pos_ptr->y() -
	// player_pos_ptr->speed() * dt);
	// }
	// if (player_pos_ptr->downPressed()) {
	// 	player_pos_ptr->setY(player_pos_ptr->y() +
	// player_pos_ptr->speed() * dt);
	// }
	//
	// if (player_pos_ptr->y() < 0.0f) { player_pos_ptr->setY(0.0f); }
	// if (player_pos_ptr->y() + static_cast<float>(kPaddleHeight) >
	//     static_cast<float>(court_height)) {
	// 	player_pos_ptr->setY(
	// 	    static_cast<float>(court_height) -
	// 	    static_cast<float>(kPaddleHeight));
	// }
}

auto PongScene::updateEnemy(float dt) -> void
{
	// TODO: Refactor - needs mutable enemy_pos_ptr for setter calls
	// float center =
	//     enemy_pos_ptr->y() + static_cast<float>(kPaddleHeight) / 2.0f;
	// if (center < ball_pos_ptr->y()) {
	// 	enemy_pos_ptr->setY(enemy_pos_ptr->y() + enemy_pos_ptr->speed()
	// * dt);
	// }
	// if (center > ball_pos_ptr->y()) {
	// 	enemy_pos_ptr->setY(enemy_pos_ptr->y() - enemy_pos_ptr->speed()
	// * dt);
	// }
	//
	// if (enemy_pos_ptr->y() < 0.0f) { enemy_pos_ptr->setY(0.0f); }
	// if (enemy_pos_ptr->y() + static_cast<float>(kPaddleHeight) >
	//     static_cast<float>(court_height)) {
	// 	enemy_pos_ptr->setY(
	// 	    static_cast<float>(court_height) -
	// 	    static_cast<float>(kPaddleHeight));
	// }
}

auto PongScene::checkCollisions() -> void
{
	// TODO: Refactor - needs mutable ball_pos_ptr for setter calls
	// float bx = ball_pos_ptr->x();
	// float by = ball_pos_ptr->y();
	//
	// float player_top = player_pos_ptr->y();
	// float player_bottom =
	//     player_pos_ptr->y() + static_cast<float>(kPaddleHeight);
	//
	// if (bx - static_cast<float>(kBallRadius) <=
	//         kPlayerX + static_cast<float>(kPaddleWidth) &&
	//     by >= player_top && by <= player_bottom) {
	// 	ball_pos_ptr->setVx(std::abs(ball_pos_ptr->vx()));
	// }
	//
	// float enemy_top = enemy_pos_ptr->y();
	// float enemy_bottom =
	//     enemy_pos_ptr->y() + static_cast<float>(kPaddleHeight);
	//
	// if (bx + static_cast<float>(kBallRadius) >= enemy_x &&
	//     by >= enemy_top && by <= enemy_bottom) {
	// 	ball_pos_ptr->setVx(-std::abs(ball_pos_ptr->vx()));
	// }
}

auto PongScene::syncViewComponents() -> void
{
	// TODO: Refactor - const view pointers cannot call non-const
	// setPosition(). View components need a different mutation pattern
	// that doesn't require bypassing const-correctness.
}

}  // namespace elemental

// clang-format off
// vim: set textwidth=80 ts=8 sts=0 sw=8 noexpandtab ft=cpp.doxygen :