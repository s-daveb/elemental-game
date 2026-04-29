/* StateCommand.hpp
 * Copyright © 2026 Saul D. Beniquez
 * License: Mozilla Public License v. 2.0
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v.2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/.
 */

#pragma once

#include <cstdint>

namespace elemental {

/**
 * @brief Commands for state machine transitions
 *
 * These commands are queued by the simulation thread and executed
 * by the render thread at the start of each frame. This ensures
 * all state mutations happen on the render thread where SDL
 * texture operations are safe.
 */
enum class StateCommand : uint8_t
{
	None,          ///< No command / placeholder
	PushMainMenu,  ///< Push MainMenu state onto stack
	PushGame,      ///< Push PongScene/game state onto stack
	Pop,           ///< Pop current state from stack
	Clear,         ///< Clear all states from stack
};

}  // namespace elemental

// clang-format off
// vim: set textwidth=80 ts=8 sts=0 sw=8 noexpandtab ft=cpp.doxygen :
