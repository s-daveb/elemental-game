/*
 * Copyright © 2024 Saul D. Beniquez
 * License: Mozilla Public License v. 2.0
 *
 * This Source Code Form is subject to the terms of the Mozilla Public License,
 * v.2.0. If a copy of the MPL was not distributed with this file, You can
 * obtain one at https://mozilla.org/MPL/2.0/.`
 */

#include "MainMenu.hpp"

#include "IOCore/Exception.hpp"
#include "IState.hpp"
#include "nonstd/span.hpp"

#include <SDL2/SDL.h>
#include <SDL_ttf.h>

#include <any>
#include <cstdint>

using namespace elemental;

namespace {
struct AssetStore {
	AssetStore() = default;
	virtual ~AssetStore()
	{
		for (auto& texture_ptr : textures) {
			SDL_DestroyTexture(texture_ptr);
		}
	}

	TTF_Font* font{ TTF_OpenFont("assets/fonts/Roboto-Regular.ttf", 24) };
	std::vector<SDL_Texture*> textures;

} assets;

void update_textures()
{
	// Generate textures from text using SDL2_ttf
}

}

MainMenu::MainMenu() : IState()
{
	SDL_GetKeyboardState(reinterpret_cast<int*>(&(properties.keyboard_size))
	);
}

auto MainMenu::step() -> void {}

auto MainMenu::recieveMessage(const Observable& sender, std::any message) -> void
{
	ASSERT_MSG(message.has_value(), "[Message : std::any] is empty");

	SDL_Event event = std::any_cast<SDL_Event>(message);

	if (event.type == SDL_KEYDOWN || event.type == SDL_KEYUP) {
		state.keystates = nonstd::span<const uint8_t>(
		    SDL_GetKeyboardState(nullptr), properties.keyboard_size
		);
		// Check for Up Arrow key
		if (state.keystates[SDL_SCANCODE_UP]) {
			if (selected_item > 0) {
				selected_item--;
			}
		}
		// Check for Down Arrow key
		if (state.keystates[SDL_SCANCODE_DOWN]) {
			if (selected_item < menu_items.size()) {
				selected_item++;
			}
		}
	}
}

auto MainMenu::getDrawables() -> std::list<std::unique_ptr<IDrawable>>
{
	return {};
}

// clang-format off
// vim: set foldmethod=syntax foldminlines=10 textwidth=80 ts=8 sts=0 sw=8 noexpandtab ft=cpp.doxygen :
