/* ElementalGame.cpp
 * Copyright © 2023 Saul D. Beniquez
 * License: Mozilla Public License v. 2.0
 *
 * This Source Code Form is subject to the terms of the Mozilla Public License,
 * v.2.0. If a copy of the MPL was not distributed with this file, You can
 * obtain one at https://mozilla.org/MPL/2.0/.
 */

#include "./ElementalGame.hpp"

#include "Exception.hpp"
#include "IRenderer.hpp"
#include "LoopRegulator.hpp"
#include "renderers/SdlRenderer.hpp"

#include "private/debuginfo.hpp"

/*
#include "IEventEmitter.hpp"
#include "IInputDriver.hpp"
#include "SdlEventEmitter.hpp"
*/

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

ElementalGame::~ElementalGame()
{
	video_renderer_ptr->Deactivate();

	/* threading clean-up:
	 * wait for all child threads to finish */
	for (auto& [key, values] : this->running_threads) {
		values.join();
	}
}

int
ElementalGame::Run()
{
	LoopRegulator frame_regulator(60_Hz);

	this->is_running = true;
	this->running_threads["simulation_thread"] =
	    std::thread([this]() { this->simulation_thread_loop(); });

	while (this->is_running) {
		frame_regulator.StartUpdate();
		//		this->event_emitter->emit();
		//		this->input_driver->process();
		this->render_step();

		auto cycle_delay_ms = frame_regulator.Delay();
		print_cps(cycle_delay_ms, "frame delay");
	}

	this->is_running = false;
	return -1;
}

ElementalGame::ElementalGame()
    : Application()
    , video_renderer_ptr(nullptr)
    , input_driver(nullptr)
    , event_emitter(nullptr)
    , is_running(false)
    , ticks(0)
{
	video_renderer_ptr = IRenderer::GetInstance<SdlRenderer>();

	video_renderer_ptr->Init();

	for (int i = 0; i < 200; i++) {
		SDL_PumpEvents();
	}
}

void
ElementalGame::render_step()
{
	video_renderer_ptr->Clear();

	//! \todo: draw objects out of a std::queue here.

	video_renderer_ptr->Flip();
}
void
ElementalGame::simulation_thread_loop()
{
	do {
		loop_regulator.StartUpdate();

		auto cycle_delay_ms = loop_regulator.Delay();
		print_cps(cycle_delay_ms);
	} while (this->is_running);
}
// clang-format off
// vim: set foldmethod=syntax textwidth=80 ts=8 sts=0 sw=8 foldlevel=99 noexpandtab ft=cpp.doxygen :
