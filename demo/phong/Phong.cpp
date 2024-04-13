/* Phong.cpp
 * Copyright © 2023 Saul D. Beniquez
 * License: Mozilla Public License v. 2.0
 *
 * This Source Code Form is subject to the terms of the Mozilla Public License,
 * v.2.0. If a copy of the MPL was not distributed with this file, You can
 * obtain one at https://mozilla.org/MPL/2.0/.
 */

#include "./Phong.hpp"

#include "IRenderer.hpp"

#include "sys/paths.hpp"
#include "util/debug.hpp"

#include "Exception.hpp"
#include "JsonConfigFile.hpp"
#include "LoopRegulator.hpp"
#include "SdlEventSource.hpp"
#include "SdlRenderer.hpp"

#include <SDL_events.h>
#include <chrono>
#include <iostream>
#include <stack>
#include <thread>
#include <utility>

#include "types/units.hpp"

using namespace elemental;

const GameSettings kDefaultSettings{ { { "Phong",
	                                 WindowMode::Windowed,
	                                 WindowPlacement::Centered,
	                                 Position2D({ 0, 0 }),
	                                 { 1270_px, 720_px } },
	                               { 1024_px, 768_px } } };

/// \name Helper Functions
/// \{
void
print_cycle_rate(milliseconds& cycle_length,
                 c::const_string label = "cycle_length")
{
	DBG_PRINT(label << cycle_length.count() << "ms.");
}

/// \}

Phong::~Phong()
{
	video_renderer.deactivate();
}

auto
Phong::run() -> int
{
	this->is_running = true;
	try {

		this->running_threads["simulation_thread"] =
		    std::thread([this]() { this->simulation_thread_loop(); });

		this->event_and_rendering_loop();

		/* threading clean-up:
		 * wait for all child threads to finish */
		for (auto& [key, values] : this->running_threads) {
			values.join();
		}

		return kSuccess;
	} catch (Exception& exc) {
		throw;
	} catch (std::exception& excp) {
		throw Exception(excp);
	}
	return kError;
}

void
Phong::recieveMessage(const Observable& sender, std::any message)
{
	ASSERT(message.has_value());

	auto event = std::any_cast<SDL_Event>(message);
	if (event.type == SDL_QUIT) {
		this->is_running = false;
	}
}

Phong::Phong()
    : Application()
    , IObserver()
    , running_threads()
    , video_renderer(IRenderer::GetInstance<SdlRenderer>())
    , event_emitter(Singleton::getReference<SdlEventSource>())
    , settings_file(paths::get_app_config_root() / "phong" / "settings.json",
                    CreateDirs::Enabled)
    , settings()
{

	try {
		settings_file.read();
		settings = settings_file.get<GameSettings>();
	} catch (nlohmann::json::exception& except) {
		settings_file.set(kDefaultSettings);
		settings_file.write();
		settings = settings_file.get<GameSettings>();
	}

	this->video_renderer.init(settings.renderer_settings);

	//** EventDispatcher ** /
	auto quit_event_filter = [](SDL_Event& event) -> bool {
		return event.type == SDL_QUIT;
	};
	auto quit_event_handler = [&](std::any event_data) -> void {
		auto sdl_event_data = std::any_cast<SDL_Event>(event_data);

		if (sdl_event_data.type == SDL_EventType::SDL_QUIT) {
			this->is_running = false;
		}
	};
	/**/

	this->event_emitter.registerObserver(*this);
	this->event_emitter.pollEvents();
}

void
Phong::event_and_rendering_loop()
{
	LoopRegulator frame_regulator(60_Hz);

	do {
		frame_regulator.startUpdate();
		this->video_renderer.clearScreen();

		this->event_emitter.pollEvents();

		auto cycle_delay_ms = frame_regulator.delay();
		print_cycle_rate(cycle_delay_ms, "frame delay");
		video_renderer.flip();
	} while (this->is_running);

	this->is_running = false;
}

void
Phong::simulation_thread_loop()
{
	LoopRegulator loop_regulator(60_Hz);

	do {
		loop_regulator.startUpdate();

		this->event_emitter.sendEvents();

		auto cycle_delay_ms = loop_regulator.delay();
		print_cycle_rate(cycle_delay_ms);
	} while (this->is_running);
}

// clang-format off
// vim: set foldmethod=syntax textwidth=80 ts=8 sts=0 sw=8  noexpandtab ft=cpp.doxygen :
