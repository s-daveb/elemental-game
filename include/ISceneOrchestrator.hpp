/* ISceneOrchestrator.hpp
 * Copyright © 2024 Saul D. Beniquez
 * License: Mozilla Public License v. 2.0
 *
 * This Source Code Form is subject to the terms of the Mozilla Public License,
 * v.2.0. If a copy of the MPL was not distributed with this file, You can
 * obtain one at https://mozilla.org/MPL/2.0/.
 */

#pragma once

#include "Scene.hpp"

namespace elemental {
struct Scene;

using SceneContainer = std::stack<std::reference_wrapper<Scene>>;

struct ISceneOrchestrator
{
	virtual Scene& GetCurrentScene() = 0;
	virtual void ChangeScene(Scene& new_scene) = 0;
	virtual void SetNextScene(Scene& next_scne) = 0;

	virtual SceneContainer& GetAllScenes() = 0;
};
}

// clang-format off
// vim: set foldmethod=syntax textwidth=80 ts=8 sts=0 sw=8 foldlevel=99 noexpandtab ft=cpp.doxygen :
