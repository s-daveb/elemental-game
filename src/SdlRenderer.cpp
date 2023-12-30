/* SdlRenderer.cpp
 * Copyright © 2023 Saul D. Beniquez
 * License: Mozilla Public License v. 2.0
 *
 * This Source Code Form is subject to the terms of the Mozilla Public License,
 * v.2.0. If a copy of the MPL was not distributed with this file, You can
 * obtain one at https://mozilla.org/MPL/2.0/.
 */

#include "Exception.hpp"
#include "IRenderer.hpp"
#include "renderers/SdlRenderer.hpp"

#include "private/debuginfo.hpp"
#include "types.hpp"
#include "types/any_ptr.hpp"

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_render.h>

#include <any>
#include <sstream>
#include <utility>

using namespace elemental;

namespace {
std::stringstream error_buffer;
}

#define HANDLE_SDL_ERROR(what)                                                 \
	error_buffer.clear();                                                  \
	error_buffer << what << ", SDL Error:" << SDL_GetError()               \
		     << std::flush;                                            \
	throw elemental::Exception(error_buffer.str());

SdlRenderer::~SdlRenderer()
{
	if (this->is_initialized) {
		this->Deactivate();
	}
}

void
SdlRenderer::Init()
{
	if (SDL_InitSubSystem(SDL_INIT_TIMER | SDL_INIT_VIDEO) < 0) {
		HANDLE_SDL_ERROR("Could not initialize video subsystem");
	}
	if (ERROR == IMG_Init(IMG_INIT_JPG | IMG_INIT_PNG | IMG_INIT_TIF |
	                      IMG_INIT_WEBP)) {
		error_buffer.clear();
		error_buffer
		    << "Could not initialize SDL_Image: IMG_INIT() == 0"
		    << std::flush;
		throw elemental::Exception(error_buffer.str());
	}

	/*! @todo: Get desired resolution from .config or .xml file or maybe the
	 *! function arguments. */

	this->sdl_window_ptr = SDL_CreateWindow(
	    "SdlRenderer", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1024,
	    768, SDL_WINDOW_SHOWN);
	if (nullptr == this->sdl_window_ptr) {
		HANDLE_SDL_ERROR("Could not create SDL_Window");
	}

	this->sdl_renderer_ptr = SDL_CreateRenderer(this->sdl_window_ptr, 0,
	                                            SDL_RENDERER_ACCELERATED);
	if (nullptr == this->sdl_renderer_ptr) {
		HANDLE_SDL_ERROR("Could not initialize SDL_Renderer");
	}

	this->is_initialized = true;
}

void
SdlRenderer::Deactivate()
{
	debugprint("Deactivate called!");
	if (this->sdl_window_ptr != nullptr) {
		SDL_DestroyWindow(this->sdl_window_ptr);
	}
	if (this->sdl_renderer_ptr != nullptr) {
		SDL_DestroyRenderer(this->sdl_renderer_ptr);
	}

	SDL_QuitSubSystem(SDL_INIT_VIDEO);
	this->sdl_renderer_ptr = nullptr;
	this->sdl_window_ptr = nullptr;

	this->is_initialized = false;
}

std::pair<uint32_t, uint32_t>
SdlRenderer::GetResolution()
{
	int w, h;
	w = h = -1;

	/* SDL does not seem to catch this condition sometimes */
	ASSERT(this->sdl_renderer_ptr != nullptr);

	if (ERROR ==
	    SDL_GetRendererOutputSize(this->sdl_renderer_ptr, &w, &h)) {
		error_buffer.clear();
		error_buffer << "Could not get Renderer output size: ("
			     << SDL_GetError() << ")" << std::flush;
		throw elemental::Exception(error_buffer.str());
	}

	return std::make_pair(w, h);
}

void
SdlRenderer::Clear()
{
	ASSERT(this->sdl_renderer_ptr != nullptr);

	// Set bg to black
	SDL_SetRenderDrawColor(this->sdl_renderer_ptr, 0, 0, 0, 0);

	if (ERROR == SDL_RenderClear(this->sdl_renderer_ptr)) {
		HANDLE_SDL_ERROR("Call to SDL_RenderClear failed!");
	}
}
void
SdlRenderer::Flip()
{
	ASSERT(this->sdl_renderer_ptr != nullptr);

	// Set bg to black
	SDL_RenderPresent(this->sdl_renderer_ptr);
}

void
// SdlRenderer::Blit(void* image_data, Rectangle& placement)
SdlRenderer::Blit(any_ptr image_data, Rectangle& placement)
{
	ASSERT(this->sdl_renderer_ptr != nullptr);

	try {
		// auto* to_draw = reinterpret_cast<SDL_Texture*>(image_data);
		auto to_draw = any_ptr_cast<SDL_Texture*>(image_data);
		// auto* to_draw = std::any_cast<void*>(image_data);
		auto position = FromRectangle<SDL_Rect>(placement);

		if (ERROR == SDL_RenderCopy(this->sdl_renderer_ptr, to_draw,
		                            nullptr, &position)) {
			HANDLE_SDL_ERROR("SDL_RenderCopy failed.");
		}
	} catch (std::bad_cast& cast_exc) {
		std::stringstream buffer;
		buffer << "bad_any_cast" << std::endl
		       << "image_data.type: " << image_data.type().name()
		       << std::endl;
		throw Exception(buffer.str());
	} catch (Exception& thrown_exception) {
		throw;
	} catch (std::exception& thrown_exception) {
		throw Exception(thrown_exception);
	}
}

SdlRenderer::SdlRenderer()
    : IRenderer()
    , is_initialized(false)
    , sdl_window_ptr(nullptr)
    , sdl_renderer_ptr(nullptr)
{
}

// clang-format off
// vim: set foldmethod=syntax textwidth=80 ts=8 sts=0 sw=8 foldlevel=99 noexpandtab ft=cpp.doxygen :
