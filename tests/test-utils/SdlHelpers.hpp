/* SdlHelpers.hpp
 * Copyright © 2021-2024 Saul D. Beniquez
 * License: Mozilla Public License v. 2.0
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#pragma once

#include "Exception.hpp"

#include <SDL.h>

#include <iostream>
#include <sstream>
#include <string>

#include <memory>
#include <stdexcept>

namespace elemental {

struct SdlEventSimulator // #region
{
	enum KeyDir
	{
		kUP = 0,
		kRIGHT,
		kDOWN,
		kLEFT
	};

	static inline SDL_Event eventFromScancode(SDL_Scancode scancode)
	{
		SDL_Event event{ .key = {
				     .type = SDL_KEYDOWN,
				     .keysym = { .scancode = scancode } } };
		return event;
	}

	static inline SDL_Event specificArrowKey(KeyDir dir)
	{
		SDL_Event event = {};
		event.type = SDL_KEYDOWN;

		int keyboard_len = 0;

		/*  Get a pointer to the raw keyboard array and remove the const
		 *  specified. Not sure if this will make SDL freak out, but
		 *  modifying the keyboard state is useful to simulate input in
		 *  unit tests.
		 *     -- Saul 30.11.21 ☕️  */
		Uint8* keyboard =
		    const_cast<Uint8*>(SDL_GetKeyboardState(&keyboard_len));

		switch (dir) {
			case kUP:
				event.key.keysym.scancode = SDL_SCANCODE_UP;
				keyboard[SDL_SCANCODE_UP] = 1;
				keyboard[SDL_SCANCODE_DOWN] = 0;
				keyboard[SDL_SCANCODE_LEFT] = 0;
				keyboard[SDL_SCANCODE_RIGHT] = 0;
				break;
			case kDOWN:
				event.key.keysym.scancode = SDL_SCANCODE_DOWN;
				keyboard[SDL_SCANCODE_UP] = 0;
				keyboard[SDL_SCANCODE_DOWN] = 1;
				keyboard[SDL_SCANCODE_LEFT] = 0;
				keyboard[SDL_SCANCODE_RIGHT] = 0;
				break;
			case kLEFT:
				event.key.keysym.scancode = SDL_SCANCODE_LEFT;
				keyboard[SDL_SCANCODE_UP] = 0;
				keyboard[SDL_SCANCODE_DOWN] = 0;
				keyboard[SDL_SCANCODE_LEFT] = 1;
				keyboard[SDL_SCANCODE_RIGHT] = 0;
				break;
			case kRIGHT:
				event.key.keysym.scancode = SDL_SCANCODE_RIGHT;
				keyboard[SDL_SCANCODE_UP] = 0;
				keyboard[SDL_SCANCODE_DOWN] = 0;
				keyboard[SDL_SCANCODE_LEFT] = 0;
				keyboard[SDL_SCANCODE_RIGHT] = 1;
				break;
		}

		// wiki.libsdl.org: The event queue can actually be used as a
		// two way communication channel. Not only can events be read
		// from the queue, but the user can also push their own events
		// onto it. event is a pointer to the event structure you wish
		// to push onto the queue. The event is copied into the queue,
		// and the caller may dispose of the memory pointed to after
		// SDL_PushEvent() returns.
		SDL_PushEvent(&event);
		return event;
	}

	static inline SDL_Event randomArrowKey()
	{
		auto value = static_cast<KeyDir>(::rand() % 4);
		return specificArrowKey(value);
	}
}; // #endregion

struct SdlTestFixture
{

	std::stringstream buffer;

	SdlTestFixture() : buffer()
	{
		SDL_SetHint(SDL_HINT_JOYSTICK_ALLOW_BACKGROUND_EVENTS, "1");

		if (SDL_InitSubSystem(SDL_INIT_EVENTS) != kNO_ERROR) {
			buffer << "SDL Could not initialize; SDL_Error:  "
			       << SDL_GetError();
			throw elemental::Exception(buffer.str());
		}
		SDL_PumpEvents();
	}

	virtual ~SdlTestFixture() = default;
};

} // namespace elemental

// clang-format off
// vim: set foldmethod=syntax textwidth=80 ts=8 sts=0 sw=8  noexpandtab ft=cpp.doxygen :
