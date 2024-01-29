/* Kong.hpp
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
#include "ISceneOrchestrator.hpp"

#include "types/configuration.hpp"

#include "Application.hpp"
#include "JsonConfigFile.hpp"
#include "LoopRegulator.hpp"
#include "Scene.hpp"
#include "Singleton.thpp"

#include <functional>
#include <memory>
#include <stack>
#include <thread>

namespace elemental {

// Forward declarations
class IRenderer;
class IEventSource;

class Kong
    : public Application
    , public IObserver
    , public ISceneOrchestrator
{
  public:
	friend class Singleton;

	virtual ~Kong();

	/// \name Application Interface
	/// @{
	virtual int Run() override;
	/// @}

	/// \name IObserver interface
	/// @{
	virtual void RecieveMessage(const Observable& sender,
	                            std::any message = std::any()) override;
	/// @}

	/// \name ISceneOrchestrator interface
	/// @{
	virtual Scene& GetCurrentScene() override;
	virtual void ChangeScene(Scene& new_scene) override;
	virtual void SetNextScene(Scene& next_scne) override;

	virtual SceneContainer& GetAllScenes() override;
	///
	/// @}

  protected:
	Kong();
	bool is_running;

	dictionary<std::thread> running_threads;
	void simulation_thread_loop();

	IRenderer& video_renderer;
	IEventSource& event_emitter;

	SceneContainer loaded_scenes;
	configuration::JsonConfigFile game_settings;
};

} // namespace elemental

// clang-format off
// vim: set foldmethod=syntax textwidth=80 ts=8 sts=0 sw=8 foldlevel=99 noexpandtab ft=cpp.doxygen :
