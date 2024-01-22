/* Scene.hpp
 * Copyright © 2024 Saul D. Beniquez
 * License: Mozilla Public License v. 2.0
 *
 * This Source Code Form is subject to the terms of the Mozilla Public License,
 * v.2.0. If a copy of the MPL was not distributed with this file, You can
 * obtain one at https://mozilla.org/MPL/2.0/.
 */

#pragma once

#include <any>
#include <filesystem>
#include <functional>
#include <memory>
#include <stack>

#include "ISceneOrchestrator.hpp"

namespace elemental {
struct Scene;
struct ISceneOrchestrator;

class Scene
{
  public:
	virtual void Step() = 0;
	inline void operator()() { Step(); }

	virtual void Terminate() = 0;

  protected:
	Scene(ISceneOrchestrator& parent) : owner(parent){};

	ISceneOrchestrator& owner;
};

}
// vim: set foldmethod=syntax textwidth=80 ts=8 sts=0 sw=8 foldlevel=99
// noexpandtab ft=cpp.doxygen :
