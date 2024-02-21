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
#include "SDL_Memory.thpp"
// #include "std::any.thpp"
#include <any>

#include <memory>

namespace elemental {
class SdlRenderer;
namespace debug {
	template<typename T>
	struct Inspector;
}

struct SdlRenderer : public IRenderer
{
	friend class IRenderer;
	friend class debug::Inspector<SdlRenderer>;

	virtual ~SdlRenderer();

	virtual void Init(RendererSettings&) override;
	virtual void Deactivate() override;
	inline virtual bool IsInitialized() override
	{
		return this->is_initialized;
	};
	virtual Resolution GetResolution() override;
	virtual Area GetWindowSize() override;

	virtual void ClearScreen() override;
	virtual void Flip() override;

#ifndef UNIT_TEST
  protected:
#endif
	bool is_initialized;
	SdlRenderer();

	sdl::shared_ptr<SDL_Window> sdl_window_ptr;
	sdl::shared_ptr<SDL_Renderer> sdl_renderer_ptr;
};

template<>
inline Rectangle
IRenderer::ToRectangle<SDL_Rect>(const SDL_Rect& other)
{
	return { static_cast<uint32_t>(other.x), static_cast<uint32_t>(other.y),
		 static_cast<uint32_t>(other.w),
		 static_cast<uint32_t>(other.h) };
}
template<>
inline SDL_Rect
IRenderer::FromRectangle<SDL_Rect>(const Rectangle& other)
{
	return { static_cast<int>(other.x), static_cast<int>(other.y),
		 static_cast<int>(other.width),
		 static_cast<int>(other.height) };
}
} // namespace elemental

// clang-format off
// vim: set foldmethod=syntax textwidth=80 ts=8 sts=0 sw=8 foldlevel=99 noexpandtab ft=cpp.doxygen :
