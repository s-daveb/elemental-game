/* SdlEventSource.cpp
 * Copyright © 2023 Saul D. Beniquez
 * License: Mozilla Public License v. 2.0
 *
 * This Source Code Form is subject to the terms of the Mozilla Public License,
 * v.2.0. If a copy of the MPL was not distributed with this file, You can
 * obtain one at https://mozilla.org/MPL/2.0/.
 */

#include "SDL_Memory.thpp"

#include "sys/debuginfo.hpp"
#include "util/debug.hpp"

#include "Exception.hpp"
#include "IObserver.hpp"
#include "SdlEventSource.hpp"

#include <SDL.h>

#include <any>
#include <iostream>

using namespace elemental;

void
SdlEventSource::Notify()
{
	std::lock_guard<std::mutex> lock(event_queue_mutex);
	while (!event_queue.empty()) {
		auto& event = event_queue.front();

		for (auto& observer_ref : observers) {
			observer_ref.get().RecieveMessage(*this, event);
		}

		event_queue.pop();
	}
}

void
SdlEventSource::PollEvents()
{
	std::lock_guard<std::mutex> lock(event_queue_mutex);
	SDL_Event event;

	while (SDL_PollEvent(&event)) {
		event_queue.push(event);
	}
}
SdlEventSource::SdlEventSource()
    : event_queue()
    , joydev_ptr()
    , event_queue_mutex()
{
	InitJoysticks();
}

void
SdlEventSource::InitJoysticks()
{
	SDL_SetHint(SDL_HINT_JOYSTICK_ALLOW_BACKGROUND_EVENTS, "1");

	SDL_InitSubSystem(SDL_INIT_JOYSTICK | SDL_INIT_GAMECONTROLLER);

	if (SDL_NumJoysticks() > 0) {

		this->joydev_ptr = SDL_JoystickOpen(0);

		DBG_PRINT("Opened Joystick 0" << std::endl);
		DBG_PRINT("Name: " << SDL_JoystickNameForIndex(0)
		                    << std::endl);
		DBG_PRINT("Number of Axes: "
		           << SDL_JoystickNumAxes(this->joydev_ptr.get())
		           << std::endl);
		DBG_PRINT("Number of Buttons: "
		           << SDL_JoystickNumButtons(this->joydev_ptr.get())
		           << std::endl);
		DBG_PRINT("Number of Balls: "
		           << SDL_JoystickNumBalls(this->joydev_ptr.get())
		           << std::endl);

		SDL_GameControllerEventState(SDL_ENABLE);
	} else {
		auto msg = "Warning: No Joystick or Core::Input "
			   "detected";
		DBG_PRINT(msg << std::endl);
	}
}

// clang-format off
// vim: set foldmethod=syntax textwidth=80 ts=8 sts=0 sw=8 foldlevel=99 noexpandtab ft=cpp.doxygen :
