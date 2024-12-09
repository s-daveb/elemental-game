/*
 * Copyright © 2024 Saul D. Beniquez
 * License: Mozilla Public License v. 2.0
 *
 * This Source Code Form is subject to the terms of the Mozilla Public License,
 * v.2.0. If a copy of the MPL was not distributed with this file, You can
 * obtain one at https://mozilla.org/MPL/2.0/.
 */

#pragma once

#include "IState.hpp"

namespace elemental {

class MainMenu : public IState {
    public:
	MainMenu() = default;
	~MainMenu() override = default;

	auto step() -> void override;
	auto recieveMessage(const Observable& sender, std::any message)
	    -> void override;
};

} // namespace elemental

// clang-format off
// vim: set foldmethod=syntax foldminlines=10 textwidth=80 ts=8 sts=0 sw=8 noexpandtab ft=cpp.doxygen :
