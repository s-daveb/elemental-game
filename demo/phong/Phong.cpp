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

#include "util/debug.hpp"
#include "sys/paths.hpp"

#include "Exception.hpp"
#include "JsonConfigFile.hpp"
#include "LoopRegulator.hpp"
#include "SdlEventSource.hpp"
#include "SdlRenderer.hpp"

#include <chrono>
#include <iostream>
#include <stack>
#include <thread>
#include <utility>

using namespace elemental;

const GameSettings DEFAULT_SETTINGS{ { { "Phong",
	                                 WindowMode::Windowed,
	                                 WindowPlacement::Centered,
	                                 { 0, 0 },
	                                 { 1270, 720 } },
	                               { 1024, 768 } } };

/// \name Helper Functions
/// @{
void
print_cycle_rate(milliseconds& cycle_length,
                 c::const_string label = "cycle_length")
{
	DBG_PRINT(label << cycle_length.count() << "ms.");
}

/// @}

Phong::~Phong()
{
	video_renderer.Deactivate();
}

int
Phong::Run()
{
	this->is_running = true;
	try {
		LoopRegulator frame_regulator(60_Hz);

		this->running_threads["simulation_thread"] =
		    std::thread([this]() { this->simulation_thread_loop(); });

		while (this->is_running) {
			frame_regulator.StartUpdate();
			this->video_renderer.ClearScreen();

			this->event_emitter.PollEvents();

			auto cycle_delay_ms = frame_regulator.Delay();
			print_cycle_rate(cycle_delay_ms, "frame delay");
			video_renderer.Flip();
		}

		this->is_running = false;

		/* threading clean-up:
		 * wait for all child threads to finish */
		for (auto& [key, values] : this->running_threads) {
			values.join();
		}

		return NO_ERROR;
	} catch (Exception& exc) {
		throw;
	} catch (std::exception& excp) {
		throw Exception(excp);
	}
	return ERROR;
}

void
Phong::RecieveMessage(const Observable& sender, std::any message)
{
	ASSERT(message.has_value());
	SDL_Event event = std::any_cast<SDL_Event>(message);
	if (event.type == SDL_QUIT) {
		this->is_running = false;
	}
}

Phong::Phong()
    : Application()
    , IObserver()
    , is_running(false)
    , running_threads()
    , video_renderer(IRenderer::GetInstance<SdlRenderer>())
    , event_emitter(Singleton::GetReference<SdlEventSource>())
    , settings_file(paths::GetAppConfigRoot() / "phong" / "settings.json",
                    FileResource::CREATE_MISSING_DIRS)
    , settings()
{
	this->event_emitter.RegisterObserver(*this);
	this->event_emitter.PollEvents();

	try {
		settings_file.Read();
		settings = settings_file.Get<GameSettings>();
	} catch (nlohmann::json::exception& except) {
		settings = DEFAULT_SETTINGS;
		settings_file.Set(DEFAULT_SETTINGS);
		settings_file.Write();
	}

	this->video_renderer.Init(settings.renderer_settings);
	// this->video_renderer.Init(configuration["window_title"]);
}

void
Phong::simulation_thread_loop()
{
	LoopRegulator loop_regulator(60_Hz);
	do {
		loop_regulator.StartUpdate();
		this->event_emitter.Notify();

		auto cycle_delay_ms = loop_regulator.Delay();
		print_cycle_rate(cycle_delay_ms);
	} while (this->is_running);
}

// clang-format off
// vim: set foldmethod=syntax textwidth=80 ts=8 sts=0 sw=8 foldlevel=99 noexpandtab ft=cpp.doxygen :
