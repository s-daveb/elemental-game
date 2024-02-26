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

#include "Singleton.hpp"
#include "types.hpp"

#include <memory>
#include <stack>
#include <thread>

namespace elemental {

// Forward declarations
class IRenderer;
class IInputDriver;
class SdlEventSource;

class ElementalGame
    : public Application
    , public IObserver
{
  public:
	friend class Singleton;

	virtual void init(int argc, c::const_string argv[],
	                  c::const_string envp[]) override;
	virtual ~ElementalGame();
	virtual int run() override;

	virtual void recieveMessage(const Observable& sender,
	                            std::any message = std::any()) override;

  protected:
	ElementalGame();

	Dictionary<std::thread> running_threads;

	void simulation_thread_loop();

	bool is_running;

	IRenderer& video_renderer;
	SdlEventSource& event_emitter;
};

} // namespace elemental

// clang-format off
// vim: set foldmethod=syntax textwidth=80 ts=8 sts=0 sw=8  noexpandtab ft=cpp.doxygen :
