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
#include "Singleton.hpp"

#include <functional>
#include <memory>
#include <stack>
#include <thread>

namespace elemental {

// Forward declarations
class IRenderer;
class SdlEventSource;

class IPhong
    : public Application
    , public IObserver
{
  public:
	friend class Singleton;

	~IPhong() override;

	/// \name Application Interface
	/// @{
	auto run() -> int override;
	/// @}

	/// \name IObserver interface
	/// @{
	void recieveMessage(const Observable& sender,
	                    std::any message = std::any()) override;
	/// @}

  protected:
	IPhong();

	IPhong(const IPhong&) = delete;
	IPhong(IPhong&&) = delete;
	auto operator=(const IPhong&) -> IPhong& = delete;
	auto operator=(IPhong&&) -> IPhong& = delete;

	bool is_running{ false };

	Dictionary<std::thread> running_threads;
	void simulation_thread_loop();

	IRenderer& video_renderer;
	SdlEventSource& event_emitter;

	GameSettings settings;
	configuration::JsonConfigFile settings_file;
};

} // namespace elemental

// clang-format off
// vim: set foldmethod=syntax textwidth=80 ts=8 sts=0 sw=8  noexpandtab ft=cpp.doxygen :
