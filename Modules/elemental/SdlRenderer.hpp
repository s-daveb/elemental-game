/* SdlRenderer.hpp
 * Copyright © 2023 Saul D. Beniquez
 * License: Mozilla Public License v. 2.0
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v.2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/ .
 */

#pragma once

#include <SDL.h>

#include "IRenderer.hpp"
#include "SDL_Memory.hpp"

#include "types/rendering.hpp"
#include "util/testing.hpp"

#include <SDL_rect.h>
#include <SDL_render.h>
#include <SDL_video.h>

#include <cstdint>
#include <memory>
#include <type_traits>

namespace elemental {
class SdlRenderer;

struct SdlRenderer : public IRenderer {
	TEST_INSPECTABLE(SdlRenderer);

	friend class IRenderer;

	~SdlRenderer() override;

	auto init(RendererSettings&) -> void override;
	auto deactivate() -> void override;

	auto isInitialized() -> bool override;
	auto getResolution() -> Resolution override;
	auto getWindowSize() -> Area override;

	void clearScreen() override;
	void flip() override;

	void blit(std::shared_ptr<void> img_data, const Rectangle& placement)
	    override;

	template<typename T>
	auto get() const -> T
	{
		static_assert(
		    std::is_pointer_v<T>, "T must be a pointer type"
		);

		using Type = std::remove_pointer_t<T>;

		if constexpr (std::is_same_v<Type, SDL_Renderer>) {
			return sdl_renderer_ptr.get();
		} else if constexpr (std::is_same_v<Type, SDL_Window>) {
			return sdl_window_ptr.get();
		}

		static_assert(
		    (std::is_same_v<Type, SDL_Renderer> ||
		     std::is_same_v<Type, SDL_Window>),
		    "Invalid type"
		);
	}

    protected:
	bool is_initialized{ false };
	SdlRenderer();

	SdlPtr<SDL_Window> sdl_window_ptr;
	SdlPtr<SDL_Renderer> sdl_renderer_ptr;
};

template<>
inline auto IRenderer::toRectangle<SDL_Rect>(const SDL_Rect& other)
    -> Rectangle
{
	return { { static_cast<uint32_t>(other.x),
		   static_cast<uint32_t>(other.y) },
		 { static_cast<uint32_t>(other.w),
		   static_cast<uint32_t>(other.h) } };
}
template<>
inline auto IRenderer::fromRectangle<SDL_Rect>(const Rectangle& other)
    -> SDL_Rect
{
	return { static_cast<int>(other.x),
		 static_cast<int>(other.y),
		 static_cast<int>(other.width),
		 static_cast<int>(other.height) };
}
} // namespace elemental

// clang-format off
// vim: set textwidth=80 ts=8 sts=0 sw=8  noexpandtab ft=cpp.doxygen :
