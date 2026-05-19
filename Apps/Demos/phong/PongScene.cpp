/* PongScene.cpp
 * Copyright © 2026 Saul D. Beniquez
 * License: Mozilla Public License v. 2.0
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/.
 */

#include "PongScene.hpp"

#include "ComponentPool.hpp"
#include "ShapeDrawCommand.hpp"
#include "TextDrawCommand.hpp"
#include "types/rendering.hpp"

#include <cmath>
#include <mutex>

namespace elemental {

PongScene::PongScene(const SceneConfig& config) : GameScene(config), menu_font()
{
	auto& pool = ComponentPool::getInstance();
	pool.registerFactory<BallPositionComponent>(std::ref(ball_pos_factory));
	pool.registerFactory<PaddlePositionComponent>(
	    std::ref(paddle_pos_factory));

	// Fetch actual resolution first so all positions are correct
	auto resolution = renderer.getResolution();
	court_width     = resolution.width;
	court_height    = resolution.height;
	enemy_x         = static_cast<float>(court_width) - 36.0f;

	constexpr float half_paddle = static_cast<float>(kPaddleHeight) / 2.0f;

	auto& ball = ball_pos_factory.create(
	    static_cast<float>(court_width) / 2.0f,
	    static_cast<float>(court_height) / 2.0f,
	    kBallSpeed,
	    kBallSpeed * 0.6f,
	    static_cast<float>(kBallRadius));

	ball_pos_ref = ball;

	auto& player_paddle = paddle_pos_factory.create(
	    static_cast<float>(court_height) / 2.0f - half_paddle,
	    kPaddleSpeed);

	player_pos_ref = player_paddle;

	auto& enemy_paddle = paddle_pos_factory.create(
	    static_cast<float>(court_height) / 2.0f - half_paddle, kEnemySpeed);

	enemy_pos_ref = enemy_paddle;

	auto& ball_entity   = getEntity(kBallId);
	auto& player_entity = getEntity(kPlayerId);
	auto& enemy_entity  = getEntity(kEnemyId);

	if (!ball_entity.views_ref.empty()) {
		ball_view_ref = ball_entity.views_ref[0];
	}
	if (!player_entity.views_ref.empty()) {
		player_view_ref = player_entity.views_ref[0];
	}
	if (!enemy_entity.views_ref.empty()) {
		enemy_view_ref = enemy_entity.views_ref[0];
	}

	last_player_score = 0;
	last_enemy_score  = 0;
}

auto PongScene::onUpdate() -> void
{
	static constexpr float kDt = 1.0f / 60.0f;

	if (is_paused) {
		return;  // Freeze simulation when paused
	}

	updateBall(kDt);
	updatePlayer(kDt);
	updateEnemy(kDt);
	checkCollisions();
	syncViewComponents();

	// Queue textures only when score changes (not every frame)
	if (player_score != last_player_score) {
		renderer.queueTextTexture(
		    std::to_string(player_score),
		    menu_font,
		    Color{ 255, 255, 255, 255 });
		last_player_score = player_score;
	}
	if (enemy_score != last_enemy_score) {
		renderer.queueTextTexture(
		    std::to_string(enemy_score),
		    menu_font,
		    Color{ 255, 255, 255, 255 });
		last_enemy_score = enemy_score;
	}
}

auto PongScene::onMessage(const Observable& sender, std::any message) -> void
{
	if (!message.has_value()) { return; }

	auto event = std::any_cast<SDL_Event>(message);
	if (event.type == SDL_KEYDOWN) {
		// Pause toggle -- takes priority over all other input
		if (event.key.keysym.scancode == SDL_SCANCODE_P ||
		    event.key.keysym.scancode == SDL_SCANCODE_ESCAPE) {
			is_paused = !is_paused;

			if (is_paused.load()) {
				renderer.queueTextTexture(
				    "PAUSED",
				    menu_font,
				    Color{ 255, 255, 255, 255 });
				renderer.queueTextTexture(
				    "Press P or ESC to resume",
				    menu_font,
				    Color{ 200, 200, 200, 255 });
			}

			return;  // Block all other input when pausing
		}

		if (is_paused.load()) {
			return;  // Block game input when paused
		}

		// ... existing key handling (up/down arrow paddle control) ...
		if (event.key.keysym.scancode == SDL_SCANCODE_UP) {
			player_pos_ref->setUpPressed(true);
		}
		if (event.key.keysym.scancode == SDL_SCANCODE_DOWN) {
			player_pos_ref->setDownPressed(true);
		}
	}

	if (event.type == SDL_KEYUP) {
		if (event.key.keysym.scancode == SDL_SCANCODE_UP) {
			player_pos_ref->setUpPressed(false);
		}
		if (event.key.keysym.scancode == SDL_SCANCODE_DOWN) {
			player_pos_ref->setDownPressed(false);
		}
	}
}

auto PongScene::resetBall() -> void
{
	ball_pos_ref->setX(static_cast<float>(court_width) / 2.0f);
	ball_pos_ref->setY(static_cast<float>(court_height) / 2.0f);
	ball_pos_ref->setVx(kBallSpeed);
	ball_pos_ref->setVy(kBallSpeed * 0.6f);
}

auto PongScene::updateBall(float dt) -> void
{
	ball_pos_ref->setX(ball_pos_ref->x() + ball_pos_ref->vx() * dt);
	ball_pos_ref->setY(ball_pos_ref->y() + ball_pos_ref->vy() * dt);

	auto half_r = ball_pos_ref->radius();

	if (ball_pos_ref->y() - half_r <= static_cast<float>(0)) {
		ball_pos_ref->setVy(std::abs(ball_pos_ref->vy()));
		ball_pos_ref->setY(half_r);
	}
	if (ball_pos_ref->y() + half_r >= static_cast<float>(court_height)) {
		ball_pos_ref->setVy(-std::abs(ball_pos_ref->vy()));
		ball_pos_ref->setY(static_cast<float>(court_height) - half_r);
	}

	if (ball_pos_ref->x() <= 0.0f) {
		enemy_score++;
		resetBall();
	}
	if (ball_pos_ref->x() >= court_width) {
		player_score++;
		resetBall();
	}
}

auto PongScene::updatePlayer(float dt) -> void
{
	if (player_pos_ref->upPressed()) {
		player_pos_ref->setY(
		    player_pos_ref->y() - player_pos_ref->speed() * dt);
	}
	if (player_pos_ref->downPressed()) {
		player_pos_ref->setY(
		    player_pos_ref->y() + player_pos_ref->speed() * dt);
	}

	if (player_pos_ref->y() < static_cast<float>(0)) {
		player_pos_ref->setY(static_cast<float>(0));
	}
	if (player_pos_ref->y() + kPaddleHeight >=
	    static_cast<float>(court_height)) {
		player_pos_ref->setY(
		    static_cast<float>(court_height) - kPaddleHeight);
	}
}

auto PongScene::updateEnemy(float dt) -> void
{
	float center =
	    enemy_pos_ref->y() + static_cast<float>(kPaddleHeight) / 2.0f;

	if (center < ball_pos_ref->y()) {
		enemy_pos_ref->setY(
		    enemy_pos_ref->y() + enemy_pos_ref->speed() * dt);
	}
	if (center > ball_pos_ref->y()) {
		enemy_pos_ref->setY(
		    enemy_pos_ref->y() - enemy_pos_ref->speed() * dt);
	}

	if (enemy_pos_ref->y() < static_cast<float>(0)) {
		enemy_pos_ref->setY(static_cast<float>(0));
	}
	if (enemy_pos_ref->y() + kPaddleHeight >= court_height) {
		enemy_pos_ref->setY(court_height - kPaddleHeight);
	}
}

auto PongScene::checkCollisions() -> void
{
	float bx = ball_pos_ref->x();
	float by = ball_pos_ref->y();

	float player_top = player_pos_ref->y();
	float player_bottom =
	    player_pos_ref->y() + static_cast<float>(kPaddleHeight);

	if (bx - static_cast<float>(kBallRadius) <=
	        kPlayerX + static_cast<float>(kPaddleWidth) &&
	    by >= player_top && by <= player_bottom) {
		ball_pos_ref->setVx(std::abs(ball_pos_ref->vx()));
	}

	float enemy_top = enemy_pos_ref->y();
	float enemy_bottom =
	    enemy_pos_ref->y() + static_cast<float>(kPaddleHeight);

	if (bx + kBallRadius >= enemy_x && by >= enemy_top &&
	    by <= enemy_bottom) {
		ball_pos_ref->setVx(-std::abs(ball_pos_ref->vx()));
	}
}

auto PongScene::syncViewComponents() -> void
{
	if (ball_view_ref) {
		ball_view_ref->setPosition(
		    Point{ static_cast<uint32_t>(ball_pos_ref->x()),
		           static_cast<uint32_t>(ball_pos_ref->y()) });
	}
	if (player_view_ref) {
		player_view_ref->setPosition(
		    Point{ static_cast<uint32_t>(kPlayerX),
		           static_cast<uint32_t>(player_pos_ref->y()) });
	}
	if (enemy_view_ref) {
		enemy_view_ref->setPosition(
		    Point{ static_cast<uint32_t>(enemy_x),
		           static_cast<uint32_t>(enemy_pos_ref->y()) });
	}
}

auto PongScene::getDrawCommands() -> std::list<std::shared_ptr<IDrawCommand>>
{
	auto cmds = GameScene::getDrawCommands();

	const uint32_t y = 20, w = 100, h = 50;

	cmds.push_back(
	    std::make_shared<TextDrawCommand>(
	        std::to_string(player_score),
	        Rectangle{ 50, y, w, h },
	        Color{ 255, 255, 255, 255 },
	        renderer));

	cmds.push_back(
	    std::make_shared<TextDrawCommand>(
	        std::to_string(enemy_score),
	        Rectangle{ 746, y, w, h },
	        Color{ 255, 255, 255, 255 },
	        renderer));

	if (is_paused.load()) {
		// Dark semi-transparent overlay
		Rectangle overlay{ 0, 0, court_width, court_height };
		cmds.push_back(
		    std::make_shared<ShapeDrawCommand>(
		        ShapeType::FilledRectangle,
		        overlay,
		        Color{ 0, 0, 0, 128 },
		        renderer));

		// PAUSED text centered on screen
		uint32_t cx = court_width / 2;
		uint32_t cy = court_height / 2;
		cmds.push_back(
		    std::make_shared<TextDrawCommand>(
		        "PAUSED",
		        Rectangle{ cx - 150, cy - 50, 300, 100 },
		        Color{ 255, 255, 255, 255 },
		        renderer));

		// Resume instruction
		cmds.push_back(
		    std::make_shared<TextDrawCommand>(
		        "Press P or ESC to resume",
		        Rectangle{ cx - 200, cy + 60, 400, 40 },
		        Color{ 200, 200, 200, 255 },
		        renderer));
	}

	return cmds;
}

void PongScene::setFont(FontHandle f)
{ menu_font = std::move(f); }

}  // namespace elemental
