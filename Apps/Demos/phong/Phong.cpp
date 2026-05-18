/* Phong.cpp
 * Copyright © 2023 Saul D. Beniquez
 * License: Mozilla Public License v. 2.0
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v.2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/.
 */

#include "./Phong.hpp"

#include "util/debug.hpp"

#include "IOCore/Application.hpp"
#include "IOCore/Exception.hpp"
#include "IOCore/FileResource.hpp"
#include "IOCore/types/errors.hpp"

#include "GameSettings.hpp"
#include "IObserver.hpp"
#include "IRenderer.hpp"
#include "LoopRegulator.hpp"
#include "MainMenu.hpp"
#include "Observable.hpp"
#include "PongScene.hpp"
#include "SdlEventSource.hpp"
#include "SdlRenderer.hpp"
#include "Singleton.hpp"
#include "sys/paths.hpp"
#include "types/rendering.hpp"
#include "types/units.hpp"

#include <SDL_events.h>

#include <any>
#include <chrono>
#include <exception>
#include <iostream>
#include <memory>
#include <stack>
#include <thread>
#include <utility>

using namespace elemental;
using namespace IOCore;

const GameSettings kDefaultSettings{ { { "Phong",
	                                 WindowMode::Windowed,
	                                 WindowPlacement::Centered,
	                                 Position2D({ 0, 0 }),
	                                 { 1270_px, 720_px } },
	                               { 1280_px, 720_px } } };
/// \name Helper Functions
/// \{
void print_cycle_rate(
    milliseconds&   cycle_length,
    c::const_string label = "cycle_length")
{ DBG_PRINT(label << cycle_length.count() << "ms."); }
/// \}

Phong::Phong(int argc, c::const_string args[], c::const_string env[])
    : Application(argc, args, env), IObserver(), running_threads(),
      video_renderer(IRenderer::GetInstance<SdlRenderer>()),
      event_emitter(Singleton::getReference<SdlEventSource>()),
      settings_file(
          paths::get_app_config_root() / "phong" / "settings.toml",
          CreateDirs::Enabled),
      settings(), state_stack(), is_running(false)
{
	try {
		settings_file.read();
		settings = settings_file.get<GameSettings>();
	} catch (std::exception& except) {
		settings = kDefaultSettings;
		settings_file.set(kDefaultSettings);
		settings_file.write();
	}
	if (settings.renderer_settings.resolution.width == 0 ||
	    settings.renderer_settings.resolution.height == 0) {
		settings.renderer_settings.resolution =
		    kDefaultSettings.renderer_settings.resolution;
	}
	this->video_renderer.init(settings.renderer_settings);

	// Load menu font
	auto* raw_font = TTF_OpenFont("fonts/monospace.ttf", 48);
	if (!raw_font) {
		throw IOCore::Exception(
		    fmt::format("Failed to load font: {}", TTF_GetError()));
	}
	this->menu_font = SdlPtr<TTF_Font>(raw_font);

	this->event_emitter.registerObserver(
	    std::ref(static_cast<IObserver&>(*this)));
	this->event_emitter.registerObserver(state_stack);
	this->event_emitter.pollEvents();

	try {
		auto scene_config = elemental::SceneConfig{
			"GameScene",
			"StaticViewPort",
			elemental::Area{ 1280, 720 },
			{ "background", "foreground" },
			{
			    elemental::EntityConfig{
			        1,
			        "Ball",
			        "Ball",
			        640,
			        360,
			        "foreground",
			        elemental::ViewConfig{
			            "circle",
			            8,
			            0,
			            0,
			            elemental::Color{ 255, 255, 255, 255 } },
			        elemental::VelocityConfig{ 300.0f, 180.0f },
			        {} },
			    elemental::EntityConfig{
			        2,
			        "Player",
			        "Player",
			        20,
			        296,
			        "foreground",
			        elemental::ViewConfig{
			            "rectangle",
			            0,
			            16,
			            96,
			            elemental::Color{ 255, 255, 255, 255 } },
			        elemental::VelocityConfig{},
			        {} },
			    elemental::EntityConfig{
			        3,
			        "Enemy",
			        "Enemy",
			        1244,
			        296,
			        "foreground",
			        elemental::ViewConfig{
			            "rectangle",
			            0,
			            16,
			            96,
			            elemental::Color{ 255, 255, 255, 255 } },
			        elemental::VelocityConfig{},
			        {} },
			}
		};
		this->state_stack.pushState(
		    std::make_unique<PongScene>(scene_config));
	} catch (const std::exception& e) {
		DBG_PRINT("Exception creating PongScene: " << e.what());
		throw;
	}
}
Phong::~Phong()
{
	auto& thisRef = *this;

	this->event_emitter.deregisterObserver(std::ref(thisRef));
	this->event_emitter.deregisterObserver(state_stack);

	video_renderer.deactivate();
}

auto Phong::run() -> int
{
	this->is_running = true;
	try {
		this->running_threads["simulation_thread"] =
		    std::thread([this]() { this->simulation_thread_loop(); });

		this->event_and_rendering_loop();

		// threading clean-up: wait for all child threads to
		// finish
		for (auto& [key, values]: this->running_threads) {
			values.join();
		}

		return kSuccess;
	} catch (IOCore::Exception& exc) {
		throw;
	} catch (std::exception& excp) {
		throw IOCore::Exception(excp);
	}
	return kError;
}

void Phong::recieveMessage(const Observable& sender, std::any message)
{
	ASSERT(message.has_value());

	auto event = std::any_cast<SDL_Event>(message);
	if (event.type == SDL_QUIT) { this->is_running = false; }
}

void Phong::event_and_rendering_loop()
{
	LoopRegulator frame_regulator(60_Hz);

	do {
		frame_regulator.startUpdate();

		// PHASE 1: State management (render thread only)
		// Execute state changes queued by previous frame's
		// events/simulation
		this->processPendingStateChanges();

		// PHASE 2: Texture preparation (render thread only)
		// Create any textures needed by current state
		this->video_renderer.processTextureQueue();

		// PHASE 3: Clear and setup
		this->video_renderer.clearScreen();

		// PHASE 4: Event processing (may queue state changes for NEXT
		// frame)
		this->event_emitter.pollEvents();
		this->event_emitter.sendEvents();

		// PHASE 5: Draw current state (uses pre-created textures)
		auto cmds = this->state_stack.draw();

		// PHASE 6: Present and regulate
		auto cycle_delay_ms = frame_regulator.delay();

		video_renderer.flip();
	} while (this->is_running);

	this->is_running = false;
}

void Phong::processPendingStateChanges()
{
	std::lock_guard<std::mutex> lock(command_mutex);

	while (!pending_commands.empty()) {
		auto cmd = pending_commands.front();
		pending_commands.pop();

		switch (cmd) {
		case StateCommand::Clear: state_stack.clear(); break;
		case StateCommand::Pop: state_stack.pop(); break;
		case StateCommand::PushMainMenu: {
			this->state_stack.pushState(
			    std::make_unique<MainMenu>(
			        video_renderer, menu_font));
			break;
		}
		case StateCommand::PushGame: {
			this->state_stack.pushState(
			    std::make_unique<PongScene>(game_scene_config));
			break;
		}
		default: break;
		}
	}
}

void Phong::simulation_thread_loop()
{
	LoopRegulator loop_regulator(60_Hz);

	do {
		loop_regulator.startUpdate();

		state_stack.step();

		auto cycle_delay_ms = loop_regulator.delay();
	} while (this->is_running);
}

// clang-format off
// vim: set foldmethod=syntax textwidth=80 ts=8 sts=0 sw=8  noexpandtab ft=cpp.doxygen :
