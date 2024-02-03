/* SdlEventSource.hpp
 * Copyright © 2023 Saul D. Beniquez
 * License: Mozilla Public License v. 2.0
 *
 * This Source Code Form is subject to the terms of the Mozilla Public License,
 * v.2.0. If a copy of the MPL was not distributed with this file, You can
 * obtain one at https://mozilla.org/MPL/2.0/.
 */

#pragma once

#include "SDL_Memory.thpp"
#include "Singleton.thpp"
#include "any_ptr.thpp"

#include "Observable.hpp"

#include <SDL.h>

#include <memory>
#include <mutex>
#include <queue>

namespace elemental {
class SdlEventSource : public Observable
{
  public:
	virtual ~SdlEventSource() {}

	virtual void Notify() override;

	void PollEvents();

#ifndef UNIT_TEST
  private:
#endif
	SdlEventSource();

	void InitJoystick();

	std::queue<SDL_Event> event_queue;
	unique_sdl_ptr<SDL_Joystick> joydev;

	std::mutex event_queue_mutex;
};
} // namespace elemental
  // clang-format off
// vim: set foldmethod=syntax textwidth=80 ts=8 sts=0 sw=8 foldlevel=99 noexpandtab ft=cpp.doxygen :
