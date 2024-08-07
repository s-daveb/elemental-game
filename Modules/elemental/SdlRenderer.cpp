/* SdlRenderer.cpp
 * Copyright © 2023 Saul D. Beniquez
 * License: Mozilla Public License v. 2.0
 *
 * This Source Code Form is subject to the terms of the Mozilla Public License,
 * v.2.0. If a copy of the MPL was not distributed with this file, You can
 * obtain one at https://mozilla.org/MPL/2.0/.
 */

#include "SdlRenderer.hpp"

#include "types/input.hpp"
#include "types/rendering.hpp"
#include "util/debug.hpp"

#include <IOCore/Exception.hpp>

#include <SDL.h>
#include <SDL_image.h>
#include <fmt/core.h>
#include <nlohmann/json.hpp>

#include <memory>
#include <sstream>
#include <utility>

using namespace elemental;

namespace {
std::stringstream error_buffer;
} // namespace

#define HANDLE_SDL_ERROR(what)                                                  \
	error_buffer.str("");                                                   \
	error_buffer << what << ", SDL Error:" << SDL_GetError() << std::flush; \
	throw IOCore::Exception(error_buffer.str());

SdlRenderer::~SdlRenderer()
{
	if (this->is_initialized) {
		this->deactivate();
	}
}

void SdlRenderer::init(RendererSettings& settings)
{

	if (SDL_InitSubSystem(SDL_INIT_TIMER | SDL_INIT_VIDEO) < 0) {
		HANDLE_SDL_ERROR("Could not initialize video subsystem");
	}
	if (kError ==
	    IMG_Init(
		IMG_INIT_JPG | IMG_INIT_PNG | IMG_INIT_TIF | IMG_INIT_WEBP
	    )) {
		HANDLE_SDL_ERROR(fmt::format(
				     "Could not initialize SDL_Image: {}",
				     IMG_GetError()
		)
		                     .c_str());
	}
	int window_xpos, window_ypos, window_width, window_height, res_width,
	    res_height;
	Uint32 sdl_flags = SDL_WINDOW_SHOWN;
	std::string window_title;

	window_title = settings.window.title;
	window_width = settings.window.size.width;
	window_height = settings.window.size.height;

	res_width = settings.resolution.width;
	res_height = settings.resolution.height;

	if (settings.window.placement == WindowPlacement::Manual) {
		window_xpos = settings.window.position.x;
		window_ypos = settings.window.position.y;
	} else if (settings.window.placement == WindowPlacement::Centered) {
		window_xpos = window_ypos = SDL_WINDOWPOS_CENTERED;
	}

	if (settings.window.mode == WindowMode::Fullscreen) {
		sdl_flags |= SDL_WINDOW_FULLSCREEN;
	}

	this->sdl_window_ptr = SDL_CreateWindow(
	    window_title.c_str(),
	    window_xpos,
	    window_ypos,
	    window_width,
	    window_height,
	    sdl_flags
	);
	if (nullptr == this->sdl_window_ptr) {
		HANDLE_SDL_ERROR("Could not create SDL_Window");
	}

	this->sdl_renderer_ptr = SDL_CreateRenderer(
	    this->sdl_window_ptr, 0, SDL_RENDERER_ACCELERATED
	);
	if (nullptr == this->sdl_renderer_ptr) {
		HANDLE_SDL_ERROR("Could not initialize SDL_Renderer");
	}

	if (SDL_RenderSetLogicalSize(
		this->sdl_renderer_ptr, res_width, res_height
	    )) {

		HANDLE_SDL_ERROR("Could not set SDL_Renderer LogicalSize");
	}

	this->is_initialized = true;
}

void SdlRenderer::deactivate()
{
	DBG_PRINT("SdlRenderer::Deactivate called!");
	if (this->sdl_window_ptr != nullptr) {
		this->sdl_window_ptr.reset();
	}
	if (this->sdl_renderer_ptr != nullptr) {
		this->sdl_renderer_ptr.reset();
	}

	SDL_QuitSubSystem(SDL_INIT_VIDEO);
	this->is_initialized = false;
}
auto SdlRenderer::isInitialized() -> bool
{
	return this->is_initialized;
};

auto SdlRenderer::getResolution() -> Resolution
{
	int width, height;

	/* SDL does not seem to catch this condition sometimes */
	ASSERT(this->sdl_renderer_ptr.get() != nullptr)

	if (kError == SDL_GetRendererOutputSize(
			  this->sdl_renderer_ptr.get(), &width, &height
		      )) {
		HANDLE_SDL_ERROR("Could not get Renderer output size");
	}

	return { static_cast<uint32_t>(width), static_cast<uint32_t>(height) };
}

auto SdlRenderer::getWindowSize() -> Area
{
	int width, height;

	/* SDL does not seem to catch this condition sometimes */
	ASSERT(this->sdl_window_ptr != nullptr);
	SDL_GetWindowSize(this->sdl_window_ptr.get(), &width, &height);

	// Prevent negative ints being casted to large values.
	// Throws an exception if ASSERT is false
	ASSERT(width > 0);
	ASSERT(height > 0);

	return { static_cast<uint32_t>(width), static_cast<uint32_t>(height) };
}

void SdlRenderer::clearScreen()
{
	ASSERT(this->sdl_renderer_ptr != nullptr);

	// Set bg to black
	SDL_SetRenderDrawColor(this->sdl_renderer_ptr.get(), 0, 0, 0, 0);

	if (kError == SDL_RenderClear(this->sdl_renderer_ptr.get())) {
		HANDLE_SDL_ERROR("Call to SDL_RenderClear failed!");
	}
}
void SdlRenderer::flip()
{
	ASSERT(this->sdl_renderer_ptr != nullptr);

	// Set bg to black
	SDL_RenderPresent(this->sdl_renderer_ptr.get());
}

/*! \todo convert this to a private method, used internally to wrap SDL_Blit */
void SdlRenderer::blit(std::shared_ptr<void> image_data, Rectangle& placement)
{
	ASSERT(this->sdl_renderer_ptr != nullptr);
	ASSERT(image_data.get() != nullptr);

	try {
		auto to_draw = std::static_pointer_cast<SDL_Texture>(image_data);
		auto position = fromRectangle<SDL_Rect>(placement);

		if (kError == SDL_RenderCopy(
				  this->sdl_renderer_ptr.get(),
				  to_draw.get(),
				  nullptr,
				  &position
			      )) {
			HANDLE_SDL_ERROR("SDL_RenderCopy failed.");
		}
	} catch (IOCore::Exception& thrown_exception) {
		throw;
	} catch (std::exception& thrown_exception) {
		throw IOCore::Exception(thrown_exception);
	}
}

SdlRenderer::SdlRenderer()
    : IRenderer(), sdl_window_ptr(nullptr), sdl_renderer_ptr(nullptr)
{
}

// clang-format off
// vim: set foldmethod=syntax textwidth=80 ts=8 sts=0 sw=8  noexpandtab ft=cpp.doxygen :
