/* SdlRenderer.hpp
 * Copyright © 2023 Saul D. Beniquez
 * License: Mozilla Public License v. 2.0
 *
 * This Source Code Form is subject to the terms of the Mozilla Public License,
 * v.2.0. If a copy of the MPL was not distributed with this file, You can
 * obtain one at https://mozilla.org/MPL/2.0/ .
 */

#pragma once

#include <SDL.h>

#include "IRenderer.hpp"
#include "SDL_Memory.hpp"

#include "util/testing.hpp"

#include <memory>

namespace elemental {
class SdlRenderer;

struct SdlRenderer : public IRenderer
{
	TEST_INSPECTABLE(SdlRenderer);

	friend class IRenderer;

	virtual ~SdlRenderer();

	virtual void init(RendererSettings&) override;
	virtual void deactivate() override;
	inline virtual bool isInitialized() override
	{
		return this->is_initialized;
	};
	virtual Resolution getResolution() override;
	virtual Area getWindowSize() override;

	virtual void clearScreen() override;
	virtual void flip() override;

	virtual void blit(std::shared_ptr<void> img_data,
	                  Rectangle& placement) override;

  protected:
	bool is_initialized;
	SdlRenderer();

	SdlPtr<SDL_Window> sdl_window_ptr;
	SdlPtr<SDL_Renderer> sdl_renderer_ptr;
};

template<>
inline Rectangle
IRenderer::toRectangle<SDL_Rect>(const SDL_Rect& other)
{
	return { static_cast<uint32_t>(other.x), static_cast<uint32_t>(other.y),
		 static_cast<uint32_t>(other.w),
		 static_cast<uint32_t>(other.h) };
}
template<>
inline SDL_Rect
IRenderer::fromRectangle<SDL_Rect>(const Rectangle& other)
{
	return { static_cast<int>(other.x), static_cast<int>(other.y),
		 static_cast<int>(other.width),
		 static_cast<int>(other.height) };
}
} // namespace elemental

// clang-format off
// vim: set foldmethod=syntax textwidth=80 ts=8 sts=0 sw=8  noexpandtab ft=cpp.doxygen :
