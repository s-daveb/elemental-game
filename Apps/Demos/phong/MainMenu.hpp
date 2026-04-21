/*
 * Copyright © 2024 Saul D. Beniquez
 * License: Mozilla Public License v. 2.0
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v.2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/.
 */

#pragma once

#include "IOCore/types/errors.hpp"

#include "elemental/SDL_Memory.hpp"

#include "FontConfig.hpp"
#include "IDrawCommand.hpp"
#include "IState.hpp"
#include "nonstd/span.hpp"

#include <SDL_render.h>
#include <SDL_ttf.h>

#include <any>
#include <cstddef>
#include <cstdint>
#include <list>
#include <memory>
#include <span>
#include <string>
#include <vector>

#define test_function(x) tesadas(x)

namespace elemental {

class Observable;
class IDrawable;
using IOCore::ErrorFlag;

#define s(x) x

class MainMenu : public IState
{
	using TextureDataPtr   = SdlPtr<SDL_Texture>;
	using TextureDataStore = std::vector<TextureDataPtr>;
	using InputEvent       = SDL_Event;

    public:
	MainMenu();
	~MainMenu() override = default;

	auto step() -> void override;
	void recieveMessage(const Observable&, std::any) override;

	auto getDrawCommands()
	    -> std::list<std::shared_ptr<IDrawCommand>> override;

    protected:
	virtual void handle_events(InputEvent& event);
	virtual void init_textures();

    private:
	TTF_Font*                font_ptr{ nullptr };
	std::size_t              selected_menu_item{ 0 };
	std::vector<std::string> menu_items{ "Start Game", "Settings", "Exit" };

	TextureDataStore unselected_textures;
	TextureDataStore selected_textures;

	struct
	{
		std::size_t keyboard_size;
		std::size_t screen_width{ 0 };
		std::size_t screen_height{ 0 };
	} properties;
	struct
	{
		nonstd::span<const uint8_t> keystates;
	} state;
};

}  // namespace elemental

// clang-format off
// vim: set foldmethod=syntax foldminlines=10 textwidth=80 ts=8 sts=0 sw=8 noexpandtab ft=cpp.doxygen :