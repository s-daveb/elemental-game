/* ElementalGame.hpp
 * Copyright © 2023 Saul D. Beniquez
 * License: Mozilla Public License v. 2.0
 *
 * This Source Code Form is subject to the terms of the Mozilla Public License,
 * v.2.0. If a copy of the MPL was not distributed with this file, You can
 * obtain one at https://mozilla.org/MPL/2.0/.
 */

#pragma once

#include "IApplication.hpp"
#include "IObserver.hpp"
#include "IRenderer.hpp"

#include "Application.hpp"
#include "LoopRegulator.hpp"

#include "Singleton.thpp"
#include "type-aliases.hpp"

#include <memory>
#include <stack>
#include <thread>

namespace elemental {

// Forward declarations
class IRenderer;
class IInputDriver;
class IEventSource;

class ElementalGame
    : public Application
    , public IObserver
{
  public:
	friend class Singleton;

	virtual ~ElementalGame();
	virtual int Run() override;

	virtual void RecieveMessage(const Observable& sender,
	                            std::any message = std::any()) override;

  protected:
	ElementalGame();
	dictionary<std::thread> running_threads;

	void simulation_thread_loop();

	uint32_t ticks;
	bool is_running;

	IRenderer& video_renderer;
	IEventSource& event_emitter;
};

} // namespace elemental

// clang-format off
// vim: set foldmethod=syntax textwidth=80 ts=8 sts=0 sw=8 foldlevel=99 noexpandtab ft=cpp.doxygen :
