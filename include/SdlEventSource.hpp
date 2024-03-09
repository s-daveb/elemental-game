/* SdlEventSource.hpp
 * Copyright © 2023 Saul D. Beniquez
 * License: Mozilla Public License v. 2.0
 *
 * This Source Code Form is subject to the terms of the Mozilla Public License,
 * v.2.0. If a copy of the MPL was not distributed with this file, You can
 * obtain one at https://mozilla.org/MPL/2.0/.
 */

#pragma once

#include "SDL_Memory.hpp"
#include "Singleton.hpp"

#include "IEventSource.hpp"
#include "types/input.hpp"

#include "util/testing.hpp"

#include <SDL.h>

#include <memory>
#include <mutex>
#include <queue>

namespace elemental {

class SdlEventSource : public IEventSource
{
	TEST_INSPECTABLE(SdlEventSource);
  public:
	using Mutex = std::mutex;
	using MutexLock = std::lock_guard<Mutex>;

	friend class Singleton;

	explicit SdlEventSource(
	    InputDevices device_flags = InputDevices::Keyboard);

	~SdlEventSource() override = default;

	auto pollEvents() -> void override;
	auto transmitEvents() -> void override;

  protected:
	std::queue<SDL_Event> event_queue;
	UniqueSdlPtr<SDL_Joystick> joydev_ptr;

	Mutex mutex;
};
} // namespace elemental
  // clang-format off
// vim: set foldmethod=syntax textwidth=80 ts=8 sts=0 sw=8  noexpandtab ft=cpp.doxygen :
