/* ElementalGame.cpp
 * Copyright © 2023 Saul D. Beniquez
 * License: Mozilla Public License v. 2.0
 *
 * This Source Code Form is subject to the terms of the Mozilla Public License,
 * v.2.0. If a copy of the MPL was not distributed with this file, You can
 * obtain one at https://mozilla.org/MPL/2.0/.
 */

#include "./ElementalGame.hpp"

/*
#include "IEventSource.hpp"
#include "IInputDriver.hpp"
#include "IVideoDriver.hpp"
*/

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
	return -1;
}

ElementalGame::ElementalGame()
    : Application()
    , video_renderer()
    , input_driver()
    , event_source()
{
}

// clang-format off
// vim: set foldmethod=syntax textwidth=80 ts=8 sts=0 sw=8 foldlevel=99 noexpandtab ft=cpp.doxygen :
