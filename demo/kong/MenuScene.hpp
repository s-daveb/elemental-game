/* MenuScene.hpp
 * Copyright © 2024 Saul D. Beniquez
 * License: Mozilla Public License v. 2.0
 *
 * This Source Code Form is subject to the terms of the Mozilla Public License,
 * v.2.0. If a copy of the MPL was not distributed with this file, You can
 * obtain one at https://mozilla.org/MPL/2.0/.
 */

#pragma once

#include "Exception.hpp"
#include "Scene.hpp"

namespace elemental {

/** \todo implement this */
class MenuScene : public Scene
{
  public:
	MenuScene(ISceneOrchestrator& parent) : Scene(parent)
	{
		/** \todo Allocate 2DView, responsible for initializing font
		 *  rendering, and UI toolkits.
		 *  -- Saul 20.01.24 ☕️  */
		throw NOT_IMPLEMENTED();
	}

	virtual void Step() override { throw NOT_IMPLEMENTED(); };

	virtual void Terminate() override { throw NOT_IMPLEMENTED(); };
};
}

// clang-format off
// vim: set foldmethod=syntax textwidth=80 ts=8 sts=0 sw=8 foldlevel=99 noexpandtab ft=cpp.doxygen :
