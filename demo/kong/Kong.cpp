/* Kong.cpp
 * Copyright © 2023 Saul D. Beniquez
 * License: Mozilla Public License v. 2.0
 *
 * This Source Code Form is subject to the terms of the Mozilla Public License,
 * v.2.0. If a copy of the MPL was not distributed with this file, You can
 * obtain one at https://mozilla.org/MPL/2.0/.
 */

#include "./Kong.hpp"

#include "IEventSource.hpp"
#include "IRenderer.hpp"
#include "ISceneOrchestrator.hpp"

#include "private/debuginfo.hpp"
#include "sys/paths.hpp"
#include "types/configuration.hpp"

#include "Exception.hpp"
#include "JsonConfigFile.hpp"
#include "LoopRegulator.hpp"
#include "Scene.hpp"
#include "SdlRenderer.hpp"

#include "./MenuScene.hpp"
// #include "./GameScene.hpp"

#include "SdlEventSource.hpp"

#include <chrono>
#include <iostream>
#include <stack>
#include <thread>

using namespace elemental;

const configuration::dictionary DEFAULT_SETTINGS = {
	{ "resolution", "1280x720" },
	{ "window_size", "1280x720" },
	{ "window_title", "Kong" }
};

/// \name Helper Functions
/// @{
void
print_cps(milliseconds& cycle_length, c::const_string label = "cycle_length")
{
	debugprint(label << cycle_length.count() << "ms.");
}

bool
fix_missing_settings(configuration::dictionary& settings)
{
	auto added_settings = settings.empty() ? true : false;

	for (auto& [key, value] : DEFAULT_SETTINGS) {
		if (settings.find(key) == settings.end()) {
			if (!added_settings) {
				added_settings = true;
			}
			settings.emplace(configuration::pair{ key, value });
		}
	}

	return added_settings;
}
/// @}

Kong::~Kong()
{
	video_renderer.Deactivate();
}

int
Kong::Run()
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
			::print_cps(cycle_delay_ms, "frame delay");
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
Kong::RecieveMessage(const Observable& sender, std::any message)
{
	ASSERT(message.has_value());
	SDL_Event event = std::any_cast<SDL_Event>(message);
	if (event.type == SDL_QUIT) {
		this->is_running = false;
	}
}

Scene&
Kong::GetCurrentScene()
{

	throw Exception("This code is untested");
	return this->loaded_scenes.top().get();
}
void
Kong::ChangeScene(Scene& new_scene)
{
	this->loaded_scenes.top().get().Terminate();
	this->loaded_scenes.pop();
	this->loaded_scenes.push(new_scene);

	throw Exception("This code is untested");
}
void
Kong::SetNextScene(Scene& next_scene)
{
	auto& scene_ref = this->GetCurrentScene();
	this->loaded_scenes.pop();
	this->loaded_scenes.push(next_scene);
	this->loaded_scenes.push(scene_ref);

	throw Exception("This code is untested");
}

SceneContainer&
Kong::GetAllScenes()
{

	throw Exception("This code is untested");
	return this->loaded_scenes;
}

Kong::Kong()
    : Application()
    , IObserver()
    , ISceneOrchestrator()
    , is_running(false)
    , running_threads()
    , video_renderer(IRenderer::GetInstance<SdlRenderer>())
    , event_emitter(IEventSource::GetInstance<SdlEventSource>())
    , loaded_scenes()
    , game_settings(paths::GetAppConfigRoot() / "kong" / "settings.json",
                    FileResource::CREATE_MISSING_DIRS)
{
	auto& loaded_settings = this->game_settings.Read();

	if (fix_missing_settings(loaded_settings)) {
		this->game_settings.Write();
	}

	auto devices = DeviceFlags(MOUSE | KEYBOARD | JOYSTICK);
	this->event_emitter.InitDevices(devices);
	this->event_emitter.RegisterObserver(*this);
	this->event_emitter.PollEvents();

	this->video_renderer.Init();
	// this->video_renderer.Init(configuration["window_title"]);
}

void
Kong::simulation_thread_loop()
{
	LoopRegulator loop_regulator(60_Hz);
	do {
		loop_regulator.StartUpdate();
		this->event_emitter.Notify();
		// Scene.Update()

		auto cycle_delay_ms = loop_regulator.Delay();
		print_cps(cycle_delay_ms);
	} while (this->is_running);
}

void
Kong::load_game_config()
{
}
// clang-format off
// vim: set foldmethod=syntax textwidth=80 ts=8 sts=0 sw=8 foldlevel=99 noexpandtab ft=cpp.doxygen :
