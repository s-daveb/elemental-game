/* SdlRenderer.cpp
 * Copyright © 2023 Saul D. Beniquez
 * License: Mozilla Public License v. 2.0
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v.2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/.
 */

#include "SdlRenderer.hpp"

#include "util/debug.hpp"

#include "types/input.hpp"
#include "types/rendering.hpp"

#include <SDL.h>
#include <SDL2_gfxPrimitives.h>
#include <SDL2_rotozoom.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <fmt/core.h>

#include <IOCore/Exception.hpp>
#include <memory>
#include <nlohmann/json.hpp>
#include <sstream>
#include <utility>

using namespace elemental;
using namespace IOCore;

namespace {
std::stringstream error_buffer;
}  // namespace

#define HANDLE_SDL_ERROR(what)                                   \
	error_buffer.str("");                                    \
	error_buffer << what << ", SDL Error:" << SDL_GetError() \
	             << std::flush;                              \
	throw IOCore::Exception(error_buffer.str());

SdlRenderer::~SdlRenderer()
{
	if (this->is_initialized) { this->deactivate(); }
}

void SdlRenderer::init(RendererSettings& settings)
{
	if (this->is_initialized) {
		DBG_PRINT(
		    "SdlRenderer::init called on already-initialized renderer, "
		    "skipping.");
		return;
	}
	if (SDL_InitSubSystem(SDL_INIT_TIMER | SDL_INIT_VIDEO) < 0) {
		HANDLE_SDL_ERROR("Could not initialize video subsystem");
	}
	if (kError ==
	    IMG_Init(
	        IMG_INIT_JPG | IMG_INIT_PNG | IMG_INIT_TIF | IMG_INIT_WEBP)) {
		HANDLE_SDL_ERROR(
		    fmt::format(
		        "Could not initialize SDL_Image: {}", IMG_GetError())
		        .c_str());
	}
	if (kError == TTF_Init()) {
		HANDLE_SDL_ERROR(
		    fmt::format(
		        "Could not initialize SDL_TTF: {}", TTF_GetError())
		        .c_str());
	}
	int window_xpos, window_ypos, window_width, window_height, res_width,
	    res_height;
	Uint32      sdl_flags = SDL_WINDOW_SHOWN;
	std::string window_title;

	window_title  = settings.window.title;
	window_width  = settings.window.size.width;
	window_height = settings.window.size.height;

	res_width  = settings.resolution.width;
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
	    sdl_flags);
	if (nullptr == this->sdl_window_ptr) {
		HANDLE_SDL_ERROR("Could not create SDL_Window");
	}

	this->sdl_renderer_ptr = SDL_CreateRenderer(
	    this->sdl_window_ptr, 0, SDL_RENDERER_ACCELERATED);
	if (nullptr == this->sdl_renderer_ptr) {
		HANDLE_SDL_ERROR("Could not initialize SDL_Renderer");
	}

	if (SDL_RenderSetLogicalSize(
	        this->sdl_renderer_ptr, res_width, res_height)) {
		HANDLE_SDL_ERROR("Could not set SDL_Renderer LogicalSize");
	}

	SDL_SetRenderDrawBlendMode(
	    this->sdl_renderer_ptr.get(), SDL_BLENDMODE_BLEND);

	this->is_initialized = true;
}

void SdlRenderer::deactivate()
{
	DBG_PRINT("SdlRenderer::Deactivate called!");
	if (this->sdl_window_ptr != nullptr) { this->sdl_window_ptr.reset(); }
	if (this->sdl_renderer_ptr != nullptr) {
		this->sdl_renderer_ptr.reset();
	}

	SDL_QuitSubSystem(SDL_INIT_VIDEO);
	this->is_initialized = false;
}
auto SdlRenderer::isInitialized() -> bool
{ return this->is_initialized; };

auto SdlRenderer::getResolution() -> Resolution
{
	int width, height;

	ASSERT(this->sdl_renderer_ptr.get() != nullptr)

	SDL_RenderGetLogicalSize(this->sdl_renderer_ptr.get(), &width, &height);

	if (width == 0 || height == 0) {
		if (kError ==
		    SDL_GetRendererOutputSize(
		        this->sdl_renderer_ptr.get(), &width, &height)) {
			HANDLE_SDL_ERROR("Could not get Renderer output size");
		}
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

	// Set bg to black (fully opaque)
	SDL_SetRenderDrawColor(this->sdl_renderer_ptr.get(), 0, 0, 0, 255);

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

/*! \todo convert this to a private method, used internally to wrap SDL_Blit
 */
void SdlRenderer::blit(
    std::shared_ptr<void> image_data,
    const Rectangle&      placement)
{
	ASSERT(this->sdl_renderer_ptr != nullptr);
	ASSERT(image_data.get() != nullptr);

	try {
		auto to_draw =
		    std::static_pointer_cast<SDL_Texture>(image_data);
		auto position = fromRectangle<SDL_Rect>(placement);

		if (kError == SDL_RenderCopy(
		                  this->sdl_renderer_ptr.get(),
		                  to_draw.get(),
		                  nullptr,
		                  &position)) {
			HANDLE_SDL_ERROR("SDL_RenderCopy failed.");
		}
	} catch (IOCore::Exception& thrown_exception) {
		throw;
	} catch (std::exception& thrown_exception) {
		throw IOCore::Exception(thrown_exception);
	}
}

void SdlRenderer::setFont(SdlPtr<TTF_Font>&& font)
{ menu_font = std::move(font); }

SdlRenderer::SdlRenderer()
    : IRenderer(), sdl_window_ptr(nullptr), sdl_renderer_ptr(nullptr)
{
}

void SdlRenderer::drawFilledCircle(
    int32_t      x,
    int32_t      y,
    int32_t      radius,
    const Color& color)
{
	ASSERT(this->sdl_renderer_ptr != nullptr);
	filledCircleRGBA(
	    this->sdl_renderer_ptr.get(),
	    static_cast<Sint16>(x),
	    static_cast<Sint16>(y),
	    static_cast<Sint16>(radius),
	    color.r,
	    color.g,
	    color.b,
	    color.a);
}

void SdlRenderer::drawFilledRect(const Rectangle& rect, const Color& color)
{
	ASSERT(this->sdl_renderer_ptr != nullptr);
	boxRGBA(
	    this->sdl_renderer_ptr.get(),
	    static_cast<Sint16>(rect.x),
	    static_cast<Sint16>(rect.y),
	    static_cast<Sint16>(rect.x + rect.width),
	    static_cast<Sint16>(rect.y + rect.height),
	    color.r,
	    color.g,
	    color.b,
	    color.a);
}

void SdlRenderer::drawText(
    const std::string& text,
    const Rectangle&   bounds,
    const Color&       color)
{
	ASSERT(this->sdl_renderer_ptr != nullptr);

	auto texture = getTextTexture(text);
	if (!texture) {
		DBG_PRINT("drawText: texture not found for '" << text << "'");

		switch (missing_texture_behavior_) {
		case MissingTextureBehavior::Throw:
			throw IOCore::Exception(
			    fmt::format(
			        "Missing text texture for '{}' - "
			        "did you call queueTextTexture() before "
			        "draw()?",
			        text));
		case MissingTextureBehavior::VisualFallback: {
			// Draw bright magenta "missing texture" box
			boxRGBA(
			    this->sdl_renderer_ptr.get(),
			    static_cast<Sint16>(bounds.x),
			    static_cast<Sint16>(bounds.y),
			    static_cast<Sint16>(bounds.x + bounds.width),
			    static_cast<Sint16>(bounds.y + bounds.height),
			    255,
			    0,
			    255,
			    255);
			rectangleRGBA(
			    this->sdl_renderer_ptr.get(),
			    static_cast<Sint16>(bounds.x),
			    static_cast<Sint16>(bounds.y),
			    static_cast<Sint16>(bounds.x + bounds.width),
			    static_cast<Sint16>(bounds.y + bounds.height),
			    255,
			    255,
			    0,
			    255);
			break;
		}
		default:
		case MissingTextureBehavior::Log: break;
		}
		return;
	}

	auto sdl_texture = std::static_pointer_cast<SDL_Texture>(texture);

	SDL_SetTextureColorMod(sdl_texture.get(), color.r, color.g, color.b);
	SDL_SetTextureAlphaMod(sdl_texture.get(), color.a);

	SDL_Rect dest_rect = { static_cast<int>(bounds.x),
		               static_cast<int>(bounds.y),
		               static_cast<int>(bounds.width),
		               static_cast<int>(bounds.height) };

	SDL_RenderCopy(
	    this->sdl_renderer_ptr.get(),
	    sdl_texture.get(),
	    nullptr,
	    &dest_rect);
}

void SdlRenderer::setMissingTextureBehavior(MissingTextureBehavior behavior)
{ missing_texture_behavior_ = behavior; }

void SdlRenderer::setStateCommandHandler(
    std::function<void(StateCommand)> handler)
{ state_command_handler_ = std::move(handler); }

void SdlRenderer::queueStateCommand(StateCommand cmd)
{
	if (state_command_handler_) { state_command_handler_(cmd); }
}

void SdlRenderer::queueTextTexture(
    const std::string& text,
    FontHandle         font,
    const Color&       color)
{
	std::lock_guard<std::mutex> lock(queue_mutex);
	texture_queue.push({ text, font, color });
}

void SdlRenderer::processTextureQueue()
{
	std::queue<TextureRequest> local_queue;

	// Move all pending requests to local queue (minimize lock time)
	{
		std::lock_guard<std::mutex> lock(queue_mutex);
		std::swap(local_queue, texture_queue);
	}

	// Process each request on render thread (SDL context thread)
	while (!local_queue.empty()) {
		auto& req = local_queue.front();

		// Skip if already cached
		{
			std::lock_guard<std::mutex> cache_lock(cache_mutex);
			if (texture_cache.find(req.text) !=
			    texture_cache.end()) {
				local_queue.pop();
				continue;
			}
		}

		// Create surface from text
		SDL_Color    sdl_color = { static_cast<Uint8>(req.color.r),
			                   static_cast<Uint8>(req.color.g),
			                   static_cast<Uint8>(req.color.b),
			                   static_cast<Uint8>(req.color.a) };
		SDL_Surface* surface   = TTF_RenderText_Blended(
		    req.font.get(), req.text.c_str(), sdl_color);
		if (!surface) {
			DBG_PRINT(
			    "Failed to render text '"
			    << req.text << "': " << TTF_GetError());
			local_queue.pop();
			continue;
		}

		// Create texture from surface
		SDL_Texture* texture = SDL_CreateTextureFromSurface(
		    this->sdl_renderer_ptr.get(), surface);
		if (surface) { SDL_FreeSurface(surface); }

		if (texture) {
			std::lock_guard<std::mutex> cache_lock(cache_mutex);
			texture_cache[req.text] =
			    TextureCacheEntry{ req.text,
				               SdlPtr<SDL_Texture>{ texture } };
		} else {
			DBG_PRINT(
			    "Failed to create texture for text '"
			    << req.text << "': " << SDL_GetError());
		}

		local_queue.pop();
	}
}

Size SdlRenderer::getTextSize(const std::string& text, FontHandle font)
{
	int w, h;
	if (TTF_SizeText(font.get(), text.c_str(), &w, &h) != 0) {
		DBG_PRINT(
		    "Failed to measure text '" << text
		                               << "': " << TTF_GetError());
		return { 0, 0 };
	}
	return { static_cast<uint32_t>(w), static_cast<uint32_t>(h) };
}

std::shared_ptr<void> SdlRenderer::getTextTexture(const std::string& text)
{
	std::lock_guard<std::mutex> lock(cache_mutex);
	auto                        it = texture_cache.find(text);
	if (it != texture_cache.end()) {
		return std::shared_ptr<void>(
		    it->second.texture.get(), [](void*) {});
	}
	return nullptr;
}

bool SdlRenderer::hasTextTexture(const std::string& text) const
{
	std::lock_guard<std::mutex> lock(cache_mutex);
	return texture_cache.find(text) != texture_cache.end();
}

// clang-format off
// vim: set textwidth=80 ts=8 sts=0 sw=8  noexpandtab ft=cpp.doxygen :
