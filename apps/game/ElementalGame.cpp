/* ElementalGame.cpp
 * Copyright © 2023 Saul D. Beniquez
 * License: Mozilla Public License v. 2.0
 *
 * This Source Code Form is subject to the terms of the Mozilla Public License,
 * v.2.0. If a copy of the MPL was not distributed with this file, You can
 * obtain one at https://mozilla.org/MPL/2.0/.
 */

#include "./ElementalGame.hpp"
#include "IRenderer.hpp"

#include "ElementalGame.hpp"
#include "Exception.hpp"

#include "private/debuginfo.hpp"

/*
#include "IEventSource.hpp"
#include "IInputDriver.hpp"
*/

#include <chrono>
#include <iostream>
#include <thread>

using namespace elemental;

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
	this->is_running = true;
	this->running_threads["simulation_thread"] =
	    std::thread([this]() { this->simulation_thread_loop(); });

	/*this->running_threads["render_thread"] =
	    std::thread([this]() { this->render_thread_loop(); });*/

	std::this_thread::sleep_for(seconds(10));
	this->is_running = false;
	return -1;
}

ElementalGame::ElementalGame()
    : Application()
    , video_renderer_ptr(nullptr)
    , input_driver(nullptr)
    , event_emitter(nullptr)
{
	video_renderer_ptr = IRenderer::GetInstance<SdlRenderer>();
}

void
ElementalGame::render_thread_loop()
{
	throw Exception("Not implemented");
}

void
print_cps(milliseconds& cycle_length)
{
	debugprint("cycle_length: " << cycle_length.count() << "ms.");
}

void
ElementalGame::simulation_thread_loop()
{
	do {
		loop_regulator.StartUpdate();

		auto cycle_delay_ms = loop_regulator.Delay();
		print_cps(cycle_delay_ms);
		// throw Exception("Not implemented");
	} while (this->is_running);
}
// clang-format off
// vim: set foldmethod=syntax textwidth=80 ts=8 sts=0 sw=8 foldlevel=99 noexpandtab ft=cpp.doxygen :
