/* IRenderer.hpp
 * Copyright © 2023 Saul D. Beniquez
 * License: Mozilla Public License v. 2.0
 *
 * This Source Code Form is subject to the terms of the Mozilla Public License,
 * v.2.0. If a copy of the MPL was not distributed with this file, You can
 * obtain one at https://mozilla.org/MPL/2.0/ .
 */

#pragma once

#include "Singleton.thpp"
// #include "any_ptr.thpp"
#include "types.hpp"

#include "types/rendering.hpp"

#include <any>
#include <type_traits>
#include <utility>

#include <SDL.h>

namespace elemental {

struct IRenderer
{
	/*! \brief This macro is used in child classes to construct instances of
	 * the derived classes.
	 * \see Singleton.thpp  */
	SINGLETON_INTERFACE(IRenderer);

	virtual ~IRenderer() {}

	/*! \name Lazy Initialization Methods
	 * These initialize the current rendering subsystema and de-initialize
	 * it on-demand @{ */
	virtual void Init(RendererSettings& settings) = 0;
	virtual void Deactivate() = 0;
	virtual bool IsInitialized() = 0;
	/*! @} */

	//! \brief Does what it says on the tin.
	virtual Resolution GetResolution() = 0;

	//! \brief Does what it says on the tin.
	virtual Area GetWindowSize() = 0;

	/** \name Screen Management Methods
	 * Methods used to clear and update the game display
	 * @{
	 * \brief Clears the screen before new drawing cycles. Throws
	 * exceptions. */
	virtual void ClearScreen() = 0;
	/** \brief swaps backbuffer with new frame displays new image. Throws
	 * exceptions. */
	virtual void Flip() = 0;
	//! @}

	IRenderer() = default;

	/*! \name DataType Conversion methods
	 * \brief Conversion functions to convert Rectangle objects to the types
	 * used by native APIs to update blocks of the screen.
	 *
	 * Template method bodies shall be  defined by child classes.
	 * \note These are only exposed on the public interface for test builds
	 */
	/*! @{ */
	template<typename R>
	Rectangle ToRectangle(const R& data);

	template<typename R>
	R FromRectangle(const Rectangle& rectangle);
	/*! @}  */
};
} // namespace elemental

// clang-format off
// vim: set foldmethod=syntax textwidth=80 ts=8 sts=0 sw=8 foldlevel=99 noexpandtab ft=cpp.doxygen :
