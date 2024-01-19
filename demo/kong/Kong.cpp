/* Kong.cpp
 * Copyright © 2023 Saul D. Beniquez
 * License: Mozilla Public License v. 2.0
 *
 * This Source Code Form is subject to the terms of the Mozilla Public License,
 * v.2.0. If a copy of the MPL was not distributed with this file, You can
 * obtain one at https://mozilla.org/MPL/2.0/.
 */

#include "./Kong.hpp"

#include "Exception.hpp"
#include "IRenderer.hpp"
#include "LoopRegulator.hpp"
#include "SdlRenderer.hpp"

#include "private/debuginfo.hpp"

#include "IEventSource.hpp"
#include "SdlEventSource.hpp"

#include <chrono>
#include <iostream>
#include <thread>

using namespace elemental;

/**! \name Helper Functions */
//! @{
void
print_cps(milliseconds& cycle_length, c::const_string label = "cycle_length")
{
	debugprint(label << cycle_length.count() << "ms.");
}
//! @}

Kong::~Kong()
{
	video_renderer.Deactivate();
}

int
Kong::Run()
{
	try {
		LoopRegulator frame_regulator(60_Hz);

		this->is_running = true;
		this->running_threads["simulation_thread"] =
		    std::thread([this]() { this->simulation_thread_loop(); });

		while (this->is_running) {
			frame_regulator.StartUpdate();
			this->video_renderer.ClearScreen();

			this->event_emitter.PollEvents();

			auto cycle_delay_ms = frame_regulator.Delay();
			print_cps(cycle_delay_ms, "frame delay");
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

Kong::Kong()
    : Application()
    , IObserver()
    , ticks(0)
    , is_running(false)
    , video_renderer(IRenderer::GetInstance<SdlRenderer>())
    , event_emitter(IEventSource::GetInstance<SdlEventSource>())
{
	video_renderer.Init();

	event_emitter.InitDevices(DeviceFlags(MOUSE | KEYBOARD | JOYSTICK));
	event_emitter.RegisterObserver(*this);
	event_emitter.PollEvents();
}

void
Kong::simulation_thread_loop()
{
	LoopRegulator loop_regulator(30_Hz);
	do {
		loop_regulator.StartUpdate();
		this->event_emitter.Notify();
		// Scene.Update()

		auto cycle_delay_ms = loop_regulator.Delay();
		print_cps(cycle_delay_ms);
	} while (this->is_running);
}
// clang-format off
// vim: set foldmethod=syntax textwidth=80 ts=8 sts=0 sw=8 foldlevel=99 noexpandtab ft=cpp.doxygen :
