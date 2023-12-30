/* SdlEventEmitter.hpp
 * Copyright © 2023 Saul D. Beniquez
 * License: Mozilla Public License v. 2.0
 *
 * This Source Code Form is subject to the terms of the Mozilla Public License,
 * v.2.0. If a copy of the MPL was not distributed with this file, You can
 * obtain one at https://mozilla.org/MPL/2.0/.
 */

#pragma once

#include "IEventEmitter.hpp"

#include <SDL.h>

#include <memory>
#include <mutex>
#include <queue>

namespace elemental {

using SDL_JoystickDevice_ptr =
    std::unique_ptr<SDL_Joystick, decltype(&SDL_JoystickClose)>;

struct SdlEventEmitter : public IEventEmitter
{
	friend class IEventEmitter;

	virtual ~SdlEventEmitter() {}
	virtual void InitDevices(DeviceFlags flags = ALL);

	virtual void Enqueue(any event);
	virtual void Notify();

	virtual void PollEvents();

  protected:
	SdlEventEmitter();

  private:
	std::queue<SDL_Event> event_queue;
	SDL_JoystickDevice_ptr joydev;

	std::mutex event_queue_mutex;
};
}
// clang-format off
// vim: set foldmethod=syntax textwidth=80 ts=8 sts=0 sw=8 foldlevel=99 noexpandtab ft=cpp.doxygen :
