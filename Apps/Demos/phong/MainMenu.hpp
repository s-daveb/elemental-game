/*
 * Copyright © 2024 Saul D. Beniquez
 * License: Mozilla Public License v. 2.0
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v.2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/.
 */

#pragma once

#include "FontBook.hpp"
#include "IOCore/types/errors.hpp"
#include "IState.hpp"

#include "elemental/SDL_Memory.hpp"

#include "nonstd/span.hpp"

#include <SDL_render.h>
#include <SDL_ttf.h>
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
	using SdlTextPtr = SdlPtr<SDL_Texture>;

    public:
	MainMenu();
	~MainMenu() override = default;

	auto step() -> void override;
	auto recieveMessage(const Observable& sender, std::any message)
	    -> void override;

	auto getDrawables()
	    -> std::list<std::shared_ptr<IDrawable>> override;

    private:
	TTF_Font* font{ nullptr };
	std::size_t selected_menu_item{ 0 };
	std::vector<std::string> menu_items{ "New Game",
		                             "Load Game",
		                             "Settings",
		                             "Exit" };

	std::vector<SdlTextPtr> unselected_textures;
	std::vector<SdlTextPtr> selected_textures;

	void init_textures();

	struct {
		std::size_t keyboard_size;
		std::size_t screen_width{ 0 };
		std::size_t screen_height{ 0 };
	} properties;
	struct {
		nonstd::span<const uint8_t> keystates;
	} state;
};

} // namespace elemental

// clang-format off
// vim: set foldmethod=syntax foldminlines=10 textwidth=80 ts=8 sts=0 sw=8 noexpandtab ft=cpp.doxygen :
