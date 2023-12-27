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

/*
#include "IEventSource.hpp"
#include "IInputDriver.hpp"
*/

#include <chrono>
#include <thread>

using namespace elemental;

ElementalGame::~ElementalGame()
{
	/* wait for all child threads to finish */
	for (auto& [key, val] : this->running_threads) {
		val.join();
	}
}
int
ElementalGame::Run()
{
	// Define behavior of the background thread
	auto simulationLoop = [this]() -> void {
		do {
			this->loop_regulator.StartUpdate();
			/*
			this->input_driver->Update();
			this->event_emitter->Emit();
			this->UpdateModel();
			*/
			this->loop_regulator.EndUpdate();
			this->loop_regulator.Delay();
		} while (this->is_running);
	};

	return -1;
}

ElementalGame::ElementalGame()
    : Application()
    , video_renderer(nullptr)
    , input_driver(nullptr)
    , event_emitter(nullptr)
{
	video_renderer = IRenderer::GetInstance<SdlRenderer>();
}

// clang-format off
// vim: set foldmethod=syntax textwidth=80 ts=8 sts=0 sw=8 foldlevel=99 noexpandtab ft=cpp.doxygen :
