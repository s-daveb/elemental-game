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
#include "Observable.hpp"
#include "RectangleViewComponent.hpp"

#include <SDL_events.h>

#include <cmath>

namespace elemental {

PongScene::PongScene(const SceneConfig& config) : GameScene(config)
{
	auto& ball_entity   = getEntity(kBallId);
	auto& player_entity = getEntity(kPlayerId);
	auto& enemy_entity  = getEntity(kEnemyId);

	if (!ball_entity.views.empty()) {
		ball_ =
		    dynamic_cast<CircleViewComponent*>(ball_entity.views[0]);
	}
	if (!player_entity.views.empty()) {
		player_paddle_ = dynamic_cast<RectangleViewComponent*>(
		    player_entity.views[0]);
	}
	if (!enemy_entity.views.empty()) {
		enemy_paddle_ = dynamic_cast<RectangleViewComponent*>(
		    enemy_entity.views[0]);
	}

	auto resolution = renderer_.getResolution();
	court_width_    = resolution.width;
	court_height_   = resolution.height;
	enemy_x_        = static_cast<float>(court_width_) - 36.0f;
	ball_state_.x   = static_cast<float>(court_width_) / 2.0f;
	ball_state_.y   = static_cast<float>(court_height_) / 2.0f;
	player_.y       = static_cast<float>(court_height_) / 2.0f -
	                  static_cast<float>(kPaddleHeight_) / 2.0f;
	enemy_.y        = static_cast<float>(court_height_) / 2.0f -
	                  static_cast<float>(kPaddleHeight_) / 2.0f;
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
			player_.up_pressed = true;
		}
		if (event.key.keysym.scancode == SDL_SCANCODE_DOWN) {
			player_.down_pressed = true;
		}
	}
	if (event.type == SDL_KEYUP) {
		if (event.key.keysym.scancode == SDL_SCANCODE_UP) {
			player_.up_pressed = false;
		}
		if (event.key.keysym.scancode == SDL_SCANCODE_DOWN) {
			player_.down_pressed = false;
		}
	}
}

auto PongScene::resetBall() -> void
{
	ball_state_.x  = static_cast<float>(court_width_) / 2.0f;
	ball_state_.y  = static_cast<float>(court_height_) / 2.0f;
	ball_state_.vx = kBallSpeed;
	ball_state_.vy = kBallSpeed * 0.6f;
}

auto PongScene::updateBall(float dt) -> void
{
	ball_state_.x += ball_state_.vx * dt;
	ball_state_.y += ball_state_.vy * dt;

	auto half_h = static_cast<float>(kBallRadius_);
	if (ball_state_.y - half_h <= 0.0f) {
		ball_state_.vy = std::abs(ball_state_.vy);
		ball_state_.y  = half_h;
	}
	if (ball_state_.y + half_h >= static_cast<float>(court_height_)) {
		ball_state_.vy = -std::abs(ball_state_.vy);
		ball_state_.y  = static_cast<float>(court_height_) - half_h;
	}

	if (ball_state_.x <= 0.0f) {
		enemy_score_++;
		resetBall();
	}
	if (ball_state_.x >= static_cast<float>(court_width_)) {
		player_score_++;
		resetBall();
	}
}

auto PongScene::updatePlayer(float dt) -> void
{
	if (player_.up_pressed) { player_.y -= player_.speed * dt; }
	if (player_.down_pressed) { player_.y += player_.speed * dt; }

	if (player_.y < 0.0f) { player_.y = 0.0f; }
	if (player_.y + static_cast<float>(kPaddleHeight_) >
	    static_cast<float>(court_height_)) {
		player_.y = static_cast<float>(court_height_) -
		            static_cast<float>(kPaddleHeight_);
	}
}

auto PongScene::updateEnemy(float dt) -> void
{
	float center = enemy_.y + static_cast<float>(kPaddleHeight_) / 2.0f;
	if (center < ball_state_.y) { enemy_.y += enemy_.speed * dt; }
	if (center > ball_state_.y) { enemy_.y -= enemy_.speed * dt; }

	if (enemy_.y < 0.0f) { enemy_.y = 0.0f; }
	if (enemy_.y + static_cast<float>(kPaddleHeight_) >
	    static_cast<float>(court_height_)) {
		enemy_.y = static_cast<float>(court_height_) -
		           static_cast<float>(kPaddleHeight_);
	}
}

auto PongScene::checkCollisions() -> void
{
	float bx = ball_state_.x;
	float by = ball_state_.y;

	constexpr float kPlayerX   = kPlayerX_;
	float           player_top = player_.y;
	float player_bottom = player_.y + static_cast<float>(kPaddleHeight_);

	if (bx - static_cast<float>(kBallRadius_) <=
	        kPlayerX + static_cast<float>(kPaddleWidth_) &&
	    by >= player_top && by <= player_bottom) {
		ball_state_.vx = std::abs(ball_state_.vx);
	}

	float enemy_top    = enemy_.y;
	float enemy_bottom = enemy_.y + static_cast<float>(kPaddleHeight_);

	if (bx + static_cast<float>(kBallRadius_) >= enemy_x_ &&
	    by >= enemy_top && by <= enemy_bottom) {
		ball_state_.vx = -std::abs(ball_state_.vx);
	}
}

auto PongScene::syncViewComponents() -> void
{
	if (ball_ != nullptr) {
		ball_->setPosition(
		    Point{ static_cast<uint32_t>(ball_state_.x),
		           static_cast<uint32_t>(ball_state_.y) });
	}
	if (player_paddle_ != nullptr) {
		player_paddle_->setPosition(
		    Point{ 20, static_cast<uint32_t>(player_.y) });
	}
	if (enemy_paddle_ != nullptr) {
		enemy_paddle_->setPosition(
		    Point{ static_cast<uint32_t>(enemy_x_),
		           static_cast<uint32_t>(enemy_.y) });
	}
}

}  // namespace elemental

// clang-format off
// vim: set textwidth=80 ts=8 sts=0 sw=8 noexpandtab ft=cpp.doxygen :