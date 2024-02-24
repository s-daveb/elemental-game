/* Phong.hpp
 * Copyright © 2023 Saul D. Beniquez
 * License: Mozilla Public License v. 2.0
 *
 * This Source Code Form is subject to the terms of the Mozilla Public License,
 * v.2.0. If a copy of the MPL was not distributed with this file, You can
 * obtain one at https://mozilla.org/MPL/2.0/.
 */

#pragma once

#include "./GameSettings.hpp"

#include "IApplication.hpp"
#include "IObserver.hpp"

#include "Application.hpp"
#include "JsonConfigFile.hpp"
#include "LoopRegulator.hpp"
#include "Singleton.thpp"

#include <functional>
#include <memory>
#include <stack>
#include <thread>

namespace elemental {

// Forward declarations
class IRenderer;
class SdlEventSource;

class Phong
    : public Application
    , public IObserver
{
  public:
	friend class Singleton;

	virtual ~Phong();

	/// \name Application Interface
	/// @{
	virtual int Run() override;
	/// @}

	/// \name IObserver interface
	/// @{
	virtual void RecieveMessage(const Observable& sender,
	                            std::any message = std::any()) override;
	/// @}

  protected:
	Phong();

	Phong(const Phong&) = delete;
	Phong(Phong&&) = delete;
	Phong& operator=(const Phong&) = delete;
	Phong& operator=(Phong&&) = delete;

	bool is_running;

	dictionary<std::thread> running_threads;
	void simulation_thread_loop();

	IRenderer& video_renderer;
	SdlEventSource& event_emitter;

	GameSettings settings;
	configuration::JsonConfigFile settings_file;
};

} // namespace elemental

// clang-format off
// vim: set foldmethod=syntax textwidth=80 ts=8 sts=0 sw=8  noexpandtab ft=cpp.doxygen :
