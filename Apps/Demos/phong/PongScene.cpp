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
	pool.registerFactory<BallPositionComponent>(ball_pos_factory_);
	pool.registerFactory<PaddlePositionComponent>(paddle_pos_factory_);

	constexpr float half_paddle = static_cast<float>(kPaddleHeight_) / 2.0f;

	ball_pos_ = &ball_pos_factory_.create(
	    static_cast<float>(court_width_) / 2.0f,
	    static_cast<float>(court_height_) / 2.0f,
	    kBallSpeed,
	    kBallSpeed * 0.6f,
	    static_cast<float>(kBallRadius_));

	player_pos_ = &paddle_pos_factory_.create(
	    static_cast<float>(court_height_) / 2.0f - half_paddle,
	    kPaddleSpeed);

	enemy_pos_ = &paddle_pos_factory_.create(
	    static_cast<float>(court_height_) / 2.0f - half_paddle,
	    kEnemySpeed);

	auto& ball_entity   = getEntity(kBallId);
	auto& player_entity = getEntity(kPlayerId);
	auto& enemy_entity  = getEntity(kEnemyId);

	if (!ball_entity.views.empty()) {
		ball_view_ =
		    dynamic_cast<CircleViewComponent*>(ball_entity.views[0]);
	}
	if (!player_entity.views.empty()) {
		player_view_ = dynamic_cast<RectangleViewComponent*>(
		    player_entity.views[0]);
	}
	if (!enemy_entity.views.empty()) {
		enemy_view_ = dynamic_cast<RectangleViewComponent*>(
		    enemy_entity.views[0]);
	}

	auto resolution = renderer_.getResolution();
	court_width_    = resolution.width;
	court_height_   = resolution.height;
	enemy_x_        = static_cast<float>(court_width_) - 36.0f;
	ball_pos_->setX(static_cast<float>(court_width_) / 2.0f);
	ball_pos_->setY(static_cast<float>(court_height_) / 2.0f);
	player_pos_->setY(
	    static_cast<float>(court_height_) / 2.0f - half_paddle);
	enemy_pos_->setY(
	    static_cast<float>(court_height_) / 2.0f - half_paddle);
}

auto PongScene::onUpdate() -> void
{
	static constexpr float kDt = 1.0f / 60.0f;

	updateBall(kDt);
	updatePlayer(kDt);
	updateEnemy(kDt);
	checkCollisions();
	syncViewComponents();
}

auto PongScene::onMessage(const Observable& sender, std::any message) -> void
{
	if (!message.has_value()) { return; }

	auto event = std::any_cast<SDL_Event>(message);
	if (event.type == SDL_KEYDOWN) {
		if (event.key.keysym.scancode == SDL_SCANCODE_UP) {
			player_pos_->setUpPressed(true);
		}
		if (event.key.keysym.scancode == SDL_SCANCODE_DOWN) {
			player_pos_->setDownPressed(true);
		}
	}
	if (event.type == SDL_KEYUP) {
		if (event.key.keysym.scancode == SDL_SCANCODE_UP) {
			player_pos_->setUpPressed(false);
		}
		if (event.key.keysym.scancode == SDL_SCANCODE_DOWN) {
			player_pos_->setDownPressed(false);
		}
	}
}

auto PongScene::resetBall() -> void
{
	ball_pos_->setX(static_cast<float>(court_width_) / 2.0f);
	ball_pos_->setY(static_cast<float>(court_height_) / 2.0f);
	ball_pos_->setVx(kBallSpeed);
	ball_pos_->setVy(kBallSpeed * 0.6f);
}

auto PongScene::updateBall(float dt) -> void
{
	ball_pos_->setX(ball_pos_->x() + ball_pos_->vx() * dt);
	ball_pos_->setY(ball_pos_->y() + ball_pos_->vy() * dt);

	auto half_r = ball_pos_->radius();
	if (ball_pos_->y() - half_r <= 0.0f) {
		ball_pos_->setVy(std::abs(ball_pos_->vy()));
		ball_pos_->setY(half_r);
	}
	if (ball_pos_->y() + half_r >= static_cast<float>(court_height_)) {
		ball_pos_->setVy(-std::abs(ball_pos_->vy()));
		ball_pos_->setY(static_cast<float>(court_height_) - half_r);
	}

	if (ball_pos_->x() <= 0.0f) {
		enemy_score_++;
		resetBall();
	}
	if (ball_pos_->x() >= static_cast<float>(court_width_)) {
		player_score_++;
		resetBall();
	}
}

auto PongScene::updatePlayer(float dt) -> void
{
	if (player_pos_->upPressed()) {
		player_pos_->setY(player_pos_->y() - player_pos_->speed() * dt);
	}
	if (player_pos_->downPressed()) {
		player_pos_->setY(player_pos_->y() + player_pos_->speed() * dt);
	}

	if (player_pos_->y() < 0.0f) { player_pos_->setY(0.0f); }
	if (player_pos_->y() + static_cast<float>(kPaddleHeight_) >
	    static_cast<float>(court_height_)) {
		player_pos_->setY(
		    static_cast<float>(court_height_) -
		    static_cast<float>(kPaddleHeight_));
	}
}

auto PongScene::updateEnemy(float dt) -> void
{
	float center =
	    enemy_pos_->y() + static_cast<float>(kPaddleHeight_) / 2.0f;
	if (center < ball_pos_->y()) {
		enemy_pos_->setY(enemy_pos_->y() + enemy_pos_->speed() * dt);
	}
	if (center > ball_pos_->y()) {
		enemy_pos_->setY(enemy_pos_->y() - enemy_pos_->speed() * dt);
	}

	if (enemy_pos_->y() < 0.0f) { enemy_pos_->setY(0.0f); }
	if (enemy_pos_->y() + static_cast<float>(kPaddleHeight_) >
	    static_cast<float>(court_height_)) {
		enemy_pos_->setY(
		    static_cast<float>(court_height_) -
		    static_cast<float>(kPaddleHeight_));
	}
}

auto PongScene::checkCollisions() -> void
{
	float bx = ball_pos_->x();
	float by = ball_pos_->y();

	float player_top = player_pos_->y();
	float player_bottom =
	    player_pos_->y() + static_cast<float>(kPaddleHeight_);

	if (bx - static_cast<float>(kBallRadius_) <=
	        kPlayerX_ + static_cast<float>(kPaddleWidth_) &&
	    by >= player_top && by <= player_bottom) {
		ball_pos_->setVx(std::abs(ball_pos_->vx()));
	}

	float enemy_top = enemy_pos_->y();
	float enemy_bottom =
	    enemy_pos_->y() + static_cast<float>(kPaddleHeight_);

	if (bx + static_cast<float>(kBallRadius_) >= enemy_x_ &&
	    by >= enemy_top && by <= enemy_bottom) {
		ball_pos_->setVx(-std::abs(ball_pos_->vx()));
	}
}

auto PongScene::syncViewComponents() -> void
{
	if (ball_view_ != nullptr) {
		ball_view_->setPosition(
		    Point{ static_cast<uint32_t>(ball_pos_->x()),
		           static_cast<uint32_t>(ball_pos_->y()) });
	}
	if (player_view_ != nullptr) {
		player_view_->setPosition(
		    Point{ 20, static_cast<uint32_t>(player_pos_->y()) });
	}
	if (enemy_view_ != nullptr) {
		enemy_view_->setPosition(
		    Point{ static_cast<uint32_t>(enemy_x_),
		           static_cast<uint32_t>(enemy_pos_->y()) });
	}
}

}  // namespace elemental

// clang-format off
// vim: set textwidth=80 ts=8 sts=0 sw=8 noexpandtab ft=cpp.doxygen :