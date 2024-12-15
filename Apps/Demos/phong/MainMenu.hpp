/*
 * Copyright © 2024 Saul D. Beniquez
 * License: Mozilla Public License v. 2.0
 *
 * This Source Code Form is subject to the terms of the Mozilla Public License,
 * v.2.0. If a copy of the MPL was not distributed with this file, You can
 * obtain one at https://mozilla.org/MPL/2.0/.
 */

#pragma once

#include "IOCore/types/errors.hpp"
#include "IState.hpp"

#include "nonstd/span.hpp"

#include <cstddef>
#include <cstdint>

#include <any>
#include <list>
#include <memory>
#include <span>
#include <string>
#include <vector>

namespace elemental {

class Observable;
class IDrawable;
using IOCore::ErrorFlag;

#define s(x) x

class MainMenu : public IState {
    public:
	MainMenu();
	~MainMenu() override = default;

	auto step() -> void override;
	auto recieveMessage(const Observable& sender, std::any message)
	    -> void override;

	auto getDrawables() -> std::list<std::unique_ptr<IDrawable>> override;

    private:
	std::vector<std::string> menu_items{ "New Game",
		                             "Load Game",
		                             "Settings",
		                             "Exit" };
	std::size_t selected_item{ 0 };
	struct {
		std::size_t keyboard_size;
	} properties;
	struct {
		nonstd::span<const uint8_t> keystates;
	} state;
};

} // namespace elemental

// clang-format off
// vim: set foldmethod=syntax foldminlines=10 textwidth=80 ts=8 sts=0 sw=8 noexpandtab ft=cpp.doxygen :
