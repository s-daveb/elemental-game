/* SdlEventSource.cpp
 * Copyright © 2023 Saul D. Beniquez
 * License: Mozilla Public License v. 2.0
 *
 * This Source Code Form is subject to the terms of the Mozilla Public License,
 * v.2.0. If a copy of the MPL was not distributed with this file, You can
 * obtain one at https://mozilla.org/MPL/2.0/.
 */

#include "Exception.hpp"
#include "IEventSource.hpp"
#include "IObserver.hpp"
#include "SdlEventSource.hpp"

#include <SDL.h>

#include <iostream>

using namespace elemental;

void
init_joystick_impl(SDL_JoystickDevice_ptr& joydev)
{
	SDL_SetHint(SDL_HINT_JOYSTICK_ALLOW_BACKGROUND_EVENTS, "1");

	SDL_InitSubSystem(SDL_INIT_JOYSTICK);
	SDL_InitSubSystem(SDL_INIT_GAMECONTROLLER);

	if (SDL_NumJoysticks() > 0) {
		joydev = SDL_JoystickDevice_ptr(SDL_JoystickOpen(0),
		                                SDL_JoystickClose);
		debugprint("Opened Joystick 0" << std::endl);
		debugprint("Name: " << SDL_JoystickNameForIndex(0)
		                    << std::endl);
		debugprint("Number of Axes: "
		           << SDL_JoystickNumAxes(joydev.get()) << std::endl);
		debugprint("Number of Buttons: "
		           << SDL_JoystickNumButtons(joydev.get())
		           << std::endl);
		debugprint("Number of Balls: "
		           << SDL_JoystickNumBalls(joydev.get()) << std::endl);

		SDL_GameControllerEventState(SDL_ENABLE);
	} else {
		debugprint("Warning: No Joystick or Core::Input "
		           "detected"
		           << std::endl);
	}
}
void
SdlEventSource::InitDevices(DeviceFlags dflags)
{
	if (dflags == 0) {
		dflags = ALL;
	}

	if (dflags == 0 || (dflags & JOYSTICK)) {
		init_joystick_impl(this->joydev);
	}
}

void
SdlEventSource::Enqueue(any event)
{
	ASSERT(event != nullptr);
	auto* unwrapped_event = static_cast<SDL_Event*>(event);
	{
		std::lock_guard<std::mutex> lock(event_queue_mutex);
		this->event_queue.push(*unwrapped_event);
	}
}

void
SdlEventSource::Notify()
{
	std::lock_guard<std::mutex> lock(event_queue_mutex);
	while (!event_queue.empty()) {
		auto& event = event_queue.front();

		for (auto& observer_ref : observers) {
			observer_ref.get().OnNotice(*this, event);
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
    : IEventSource()
    , event_queue()
    , joydev(nullptr, nullptr)
    , event_queue_mutex()
{
}

// clang-format off
// vim: set foldmethod=syntax textwidth=80 ts=8 sts=0 sw=8 foldlevel=99 noexpandtab ft=cpp.doxygen :
