/* ElementalGame.hpp
 * Copyright © 2023 Saul D. Beniquez
 * License: Mozilla Public License v. 2.0
 *
 * This Source Code Form is subject to the terms of the Mozilla Public License,
 * v.2.0. If a copy of the MPL was not distributed with this file, You can
 * obtain one at https://mozilla.org/MPL/2.0/.
 */

#pragma once

#include "Application.hpp"
#include "IApplication.hpp"
#include "Singleton.template.hpp"
#include "types.hpp"

#include <memory>
#include <thread>

namespace elemental {

// Forward declarations
class IVideoDriver;
class IInputDriver;
class IEventSource;

class ElementalGame : public Application
{
  public:
	virtual ~ElementalGame();
	virtual int Run() override;

	template<typename T>
	using ptr = std::shared_ptr<T>;

  private:
	friend class Singleton<ElementalGame>;

	ElementalGame();
	Dictionary<std::thread> running_threads;

	bool is_running;
	uint32_t ticks;

	ptr<IVideoDriver> video_renderer;
	ptr<IInputDriver> input_driver;
	ptr<IEventSource> event_source;
};

} // namespace elemental

// clang-format off
// vim: set foldmethod=syntax textwidth=80 ts=8 sts=0 sw=8 foldlevel=99 noexpandtab ft=cpp.doxygen :
