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

#include "IOCore/Application.hpp"
#include "IOCore/JsonConfigFile.hpp"
#include "IOCore/TomlConfigFile.hpp"

#include "elemental/IObserver.hpp"
#include "elemental/LoopRegulator.hpp"
#include "elemental/Observable.hpp"
#include "elemental/Singleton.hpp"

#include <functional>
#include <memory>
#include <stack>
#include <thread>

namespace elemental {

// Forward declarations
class IRenderer;
class SdlEventSource;

using IOCore::Application;
using IOCore::Dictionary;

class Phong
    : public Application
    , public IObserver {
    public:
	Phong(int argc, c::const_string args[], c::const_string env[]);
	~Phong() override;

	auto run() -> int override;
	void recieveMessage(
	    const Observable& sender, std::any message = std::any()
	) override;

    protected:
	/// \name Deleted constructors & operators
	/// \{
	Phong(const Phong&) = delete;
	Phong(Phong&&) = delete;
	auto operator=(const Phong&) -> Phong& = delete;
	auto operator=(Phong&&) -> Phong& = delete;
	/// \}

	bool is_running{ false };

	Dictionary<std::thread> running_threads;

	void event_and_rendering_loop();
	void simulation_thread_loop();

	IRenderer& video_renderer;
	SdlEventSource& event_emitter;

	GameSettings settings;
	IOCore::TomlConfigFile settings_file;
};

} // namespace elemental

// clang-format off
// vim: set foldmethod=syntax textwidth=80 ts=8 sts=0 sw=8  noexpandtab ft=cpp.doxygen :
