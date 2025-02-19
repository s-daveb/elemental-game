/*
 * Copyright © 2024 Saul D. Beniquez
 * License: Mozilla Public License v. 2.0
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v.2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/.`
 */

#include "MainMenu.hpp"

#include "DrawCommand.hpp"
#include "FontConfig.hpp"
#include "IOCore/Exception.hpp"
#include "IRenderer.hpp"
#include "IState.hpp"
#include "SdlRenderer.hpp"

#include "IDrawCommand.hpp"

#include "types/errors.hpp"
#include "types/rendering.hpp"
#include "util/debug.hpp"

#include <SDL.h>

#include <fmt/core.h>
#include <nonstd/span.hpp>

#include <any>
#include <cstddef>
#include <cstdint>
#include <cstdlib>
#include <exception>
#include <list>
#include <memory>
#include <vector>

using elemental::MainMenu;

static auto font_color = SDL_Color{ 255, 255, 255 };
static auto selected_font_color = SDL_Color{ 255, 0, 0 };

MainMenu::MainMenu() : IState()
{
	static IRenderer& renderer = IRenderer::GetInstance<SdlRenderer>();

	this->properties.screen_width = renderer.getWindowSize().width;
	this->properties.screen_height = renderer.getWindowSize().height;

	SDL_GetKeyboardState(
	    reinterpret_cast<int*>(&(properties.keyboard_size))
	);
	init_textures();
}

auto MainMenu::step() -> void {}

auto MainMenu::recieveMessage(const Observable& sender, std::any message)
    -> void
{
	ASSERT_MSG(message.has_value(), "[Message : std::any] is empty");

	SDL_Event event = std::any_cast<SDL_Event>(message);
	if (event.type == SDL_KEYDOWN || event.type == SDL_KEYUP) {
		handle_events(event);
	}
}

auto MainMenu::getDrawCommands() -> std::list<std::shared_ptr<IDrawCommand>>

{
	static IRenderer& renderer = IRenderer::GetInstance<SdlRenderer>();
	std::list<std::shared_ptr<IDrawCommand>> result;

	for (size_t i = 0; i < menu_items.size(); i++) {
		TextureDataPtr texture;

		if (i == selected_menu_item) {
			texture = selected_textures.at(i);
		} else {
			texture = unselected_textures.at(i);
		}
		auto sdl_texture =
		    std::static_pointer_cast<SDL_Texture>(texture);

		SDL_Rect sdl_rect = { 0, 0, 0, 0 };
		SDL_QueryTexture(
		    texture, nullptr, nullptr, &sdl_rect.w, &sdl_rect.h
		);

		sdl_rect.x =
		    (properties.screen_width / 2) - (sdl_rect.w / 2);
		sdl_rect.y =
		    ((properties.screen_height / 2) + (i * sdl_rect.h));

		auto rect = renderer.toRectangle<SDL_Rect>(sdl_rect);
		result.push_back(std::make_shared<DrawCommand>(
		    renderer, rect, sdl_texture
		));
	}
	return result;
}

void MainMenu::handle_events(InputEvent& event)
{
	ASSERT(event.type == SDL_KEYDOWN || event.type == SDL_KEYUP);

	this->state.keystates = nonstd::span<const uint8_t>(
	    SDL_GetKeyboardState(nullptr), properties.keyboard_size
	);

	// Check for Up Arrow key
	if (this->state.keystates[SDL_SCANCODE_UP]) {
		DBG_PRINT(fmt::format(
		    "Message received {} {}", "UP", this->selected_menu_item
		));
		if (this->selected_menu_item == 0) {
			this->selected_menu_item = 3;
		} else {
			this->selected_menu_item--;
		}
	}
	// Check for Down Arrow key
	if (this->state.keystates[SDL_SCANCODE_DOWN]) {
		DBG_PRINT(fmt::format(
		    "Message received {} {}",
		    "DOWN",
		    this->selected_menu_item
		));
		if (++this->selected_menu_item < menu_items.size()) {
			;
		} else {
			this->selected_menu_item = 0;
		}
	}
	if (this->state.keystates[SDL_SCANCODE_RETURN]) {
		DBG_PRINT(fmt::format(
		    "Message received {} {}",
		    "RETURN",
		    this->selected_menu_item
		));

		if (this->selected_menu_item == 3) {
			SDL_Event* event = new SDL_Event();
			event->type = SDL_QUIT;
			SDL_PushEvent(event);
		}
	}
}

void MainMenu::init_textures()
{
	SdlRenderer& sdl_renderer = IRenderer::GetInstance<SdlRenderer>();

	FontConfig& font_book = FontConfig::getInstance();
	auto font_path = font_book.getFont("Arial");

	font = TTF_OpenFont(font_path.c_str(), 24);

	if (font == nullptr) {
		throw IOCore::Exception("Failed to load font");
	}

	for (auto& item : menu_items) {
		// Unselected texture
		auto surface =
		    TTF_RenderText_Solid(font, item.c_str(), font_color);
		if (surface == nullptr) {
			throw IOCore::Exception("Failed to render text");
		}
		auto texture = SDL_CreateTextureFromSurface(
		    sdl_renderer.get<SDL_Renderer*>(), surface
		);
		if (texture == nullptr) {
			throw IOCore::Exception("Failed to create texture");
		}
		unselected_textures.emplace_back(texture);

		SDL_FreeSurface(surface);

		// Selected texture
		surface = TTF_RenderText_Solid(
		    font, item.c_str(), selected_font_color
		);
		if (surface == nullptr) {
			throw IOCore::Exception("Failed to render text");
		}
		texture = SDL_CreateTextureFromSurface(
		    sdl_renderer.get<SDL_Renderer*>(), surface
		);
		if (texture == nullptr) {
			throw IOCore::Exception("Failed to create texture");
		}
		selected_textures.emplace_back(texture);

		SDL_FreeSurface(surface);
	}
}
// clang-format off
// vim: set foldmethod=syntax foldminlines=10 textwidth=80 ts=8 sts=0 sw=8 noexpandtab ft=cpp.doxygen :
