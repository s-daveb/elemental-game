/* SDL_Memory.hpp
 * Copyright © 2024 Saul D. Beniquez
 * License: Mozilla Public License v. 2.0
 *
 * This Source Code Form is subject to the terms of the Mozilla Public License,
 * v.2.0. If a copy of the MPL was not distributed with this file, You can
 * obtain one at https://mozilla.org/MPL/2.0/.
 */
/*! \file SDL_Memory.hpp
 * \brief Custom smart pointers for SDL resources.
 *
 * This file defines two custom smart pointer classes, `unique_ptr` and
 * `shared_ptr`, tailored for managing SDL resources. These classes automate
 * resource destruction by calling the appropriate SDL_Destroy*() functions in
 * their destructors. Additionally, they provide automatic conversion back into
 * a raw pointer for seamless integration with SDL functions.
 *
 * Usage example:
 * \code
 * #include "SDL_Memory.hpp"
 *
 * int main() {
 *     SDL_Window* sdlWindow = SDL_CreateWindow("SDL Window",
 *                                              SDL_WINDOWPOS_UNDEFINED,
 *                                              SDL_WINDOWPOS_UNDEFINED,
 *                                              800, 600, SDL_WINDOW_SHOWN);
 *     if (sdlWindow == nullptr) {
 *         // Handle error
 *         return 1;
 *     }
 *
 *     // Using unique_ptr with custom deleter
 *     elemental::UniquePtr<SDL_Window> windowPtr(sdlWindow);
 *
 *     // Now you can use windowPtr directly as SDL_Window*
 *     // UniquePtr is automatically converted to SDL_Window* where needed
 *     SDL_ShowWindow(windowPtr);
 *
 *     return 0;
 * }
 * \endcode
 *
 */
#pragma once

#include <SDL.h>
#include <memory>

namespace elemental {
/*! \brief A catch-call deleter functor.  Add a method overload for your
 * favorite SDL_* pointer type, to have the uniqe_sdl_ptr and shared_ptr
 * automatically manage resource SDL resources. */
struct SdlResourceDeleter
{
	inline auto operator()(SDL_Window* window_ptr) -> void
	{
		SDL_DestroyWindow(window_ptr);
	}
	inline auto operator()(SDL_Renderer* renderer_ptr) -> void
	{
		SDL_DestroyRenderer(renderer_ptr);
	}
	inline auto operator()(SDL_Surface* surface_ptr) -> void
	{
		SDL_FreeSurface(surface_ptr);
	}
	inline auto operator()(SDL_Texture* texture_ptr) -> void
	{
		SDL_DestroyTexture(texture_ptr);
	}
	inline auto operator()(SDL_Joystick* joystick_ptr) -> void
	{
		SDL_JoystickClose(joystick_ptr);
	}
};

template<typename TSdlData, typename TDeleter = SdlResourceDeleter>
struct UniqueSdlPtr : public std::unique_ptr<TSdlData, TDeleter>
{

	using std::unique_ptr<TSdlData, TDeleter>::unique_ptr;

	operator TSdlData*() const { return this->get(); }

	auto operator=(TSdlData* ptr) -> UniqueSdlPtr&
	{
		this->reset(ptr);
		return *this;
	}
};

template<typename TSdlData, typename TDeleter = SdlResourceDeleter>
struct SdlPtr : public std::shared_ptr<TSdlData>
{
	using std::shared_ptr<TSdlData>::shared_ptr;
	SdlPtr(TSdlData* ptr, TDeleter deleter = TDeleter{})
	    : std::shared_ptr<TSdlData>(ptr, deleter)
	{
	}
	operator TSdlData*() const { return this->get(); }

	// Assignment operator for raw pointer using std::move
	auto operator=(TSdlData* ptr) -> SdlPtr&
	{
		this->reset(ptr, TDeleter{});
		return *this;
	}
};
} // namespace elemental

// clang-format off
// vim: set foldmethod=syntax textwidth=80 ts=8 sts=0 sw=8 noexpandtab ft=cpp.doxygen :
