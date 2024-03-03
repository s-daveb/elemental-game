/* ElementalGame.cpp
 * Copyright © 2023 Saul D. Beniquez
 * License: Mozilla Public License v. 2.0
 *
 * This Source Code Form is subject to the terms of the Mozilla Public License,
 * v.2.0. If a copy of the MPL was not distributed with this file, You can
 * obtain one at https://mozilla.org/MPL/2.0/.
 */

#include "./ElementalGame.hpp"

#include "Singleton.hpp"

#include "Exception.hpp"
#include "IRenderer.hpp"
#include "LoopRegulator.hpp"
#include "SdlRenderer.hpp"

#include "util/debug.hpp"

#include "SdlEventSource.hpp"

#include <chrono>
#include <iostream>
#include <thread>

using namespace elemental;

/**! \name Helper Functions */
/// \{
void
print_cycle_rate(milliseconds& cycle_length,
                 c::const_string label = "cycle_length")
{
	DBG_PRINT(label << cycle_length.count() << "ms.");
}
/// \}

ElementalGame::~ElementalGame()
{
	video_renderer.deactivate();
}

auto
ElementalGame::run() -> int
{
	try {
		LoopRegulator frame_regulator(60_Hz);

		this->is_running = true;
		this->running_threads["simulation_thread"] =
		    std::thread([this]() { this->simulation_thread_loop(); });

		while (this->is_running) {
			frame_regulator.startUpdate();
			this->video_renderer.clearScreen();

			this->event_emitter.pollEvents();

			auto cycle_delay_ms = frame_regulator.delay();
			print_cycle_rate(cycle_delay_ms, "frame delay");
			video_renderer.flip();
		}

		this->is_running = false;

		/* threading clean-up:
		 * wait for all child threads to finish */
		for (auto& [key, values] : this->running_threads) {
			values.join();
		}
		return kSUCCESS;
	} catch (Exception& exc) {
		throw;
	} catch (std::exception& excp) {
		throw Exception(excp);
	}
	return kERROR;
}

void
ElementalGame::recieveMessage(const Observable& sender, std::any message)
{
	ASSERT(message.has_value());
	auto event = std::any_cast<SDL_Event>(message);
	if (event.type == SDL_QUIT) {
		this->is_running = false;
	}
}

ElementalGame::ElementalGame()
    : Application()
    , IObserver()
    , video_renderer(IRenderer::GetInstance<SdlRenderer>())
    , event_emitter(Singleton::getReference<SdlEventSource>())
{
	RendererSettings renderer_settings = {
		{ "Test",
		  WindowMode::Windowed,      // mode
		  WindowPlacement::Centered, // placement
		  { 0, 0 },                  // window.pos
		  { 1024_px, 768_px } },     // window.size
		{ 1024_px, 768_px }          // internal resolution
	};

	// Set up video renderer
	video_renderer.init(renderer_settings);

	// Initialize event_emitter
	event_emitter.registerObserver(*this);

#ifdef __APPLE__
	event_emitter.pollEvents();
#endif
}

void
ElementalGame::simulation_thread_loop()
{
	LoopRegulator loop_regulator(30_Hz);
	do {
		loop_regulator.startUpdate();
		this->event_emitter.notify();
		// Scene.Update()

		auto cycle_delay_ms = loop_regulator.delay();
		print_cycle_rate(cycle_delay_ms);
	} while (this->is_running);
}
// clang-format off
// vim: set foldmethod=syntax textwidth=80 ts=8 sts=0 sw=8  noexpandtab ft=cpp.doxygen :
