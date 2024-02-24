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

#include "Observable.hpp"

#include <SDL.h>

#include <memory>
#include <mutex>
#include <queue>

namespace elemental {
class SdlEventSource : public Observable
{
  public:
	friend class Singleton;

	virtual ~SdlEventSource() {}

	/** \todo Possible way to add filters to event handlers.
	 ** \code{.cpp}
	 **  // Calling code:
	 **  auto quit_event_filter = [](SDL_Event& e) -> bool {
	 **	return e.type == SDL_QUIT;
	 ** };
	 ** event_emitter.SetEventFilter(observer, quit_event_filter);
	 ** // Or perhaps define an override:
	 ** // SdlEventEmitter.hpp:
	 ** template<typename T>
	 ** void RegisterObserver(observer_ref, std::function<bool(T)>
	 *predicate)
	 ** \endcode
	 ** Then modify the Notify() method to check for these filters
	 */

	virtual void Notify() override;

	void PollEvents();

	SdlEventSource();

	void InitJoysticks();

	std::queue<SDL_Event> event_queue;
	UniqueSdlPtr<SDL_Joystick> joydev_ptr;

	std::mutex event_queue_mutex;
};
} // namespace elemental
  // clang-format off
// vim: set foldmethod=syntax textwidth=80 ts=8 sts=0 sw=8  noexpandtab ft=cpp.doxygen :
