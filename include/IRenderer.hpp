/* IRenderer.hpp
 * Copyright © 2023 Saul D. Beniquez
 * License: Mozilla Public License v. 2.0
 *
 * This Source Code Form is subject to the terms of the Mozilla Public License,
 * v.2.0. If a copy of the MPL was not distributed with this file, You can
 * obtain one at https://mozilla.org/MPL/2.0/ .
 */

#pragma once

#include "Singleton.hpp"

#include "types.hpp"
#include "types/rendering.hpp"

#include "util/testing.hpp"

#include <any>
#include <memory>
#include <type_traits>
#include <utility>

#include <SDL.h>

namespace elemental {

struct IRenderer
{
	/*! \brief This macro is used in child classes to construct instances of
	 * the derived classes.
	 * \see Singleton.hpp  */
	SINGLETON_INTERFACE(IRenderer);
	TEST_INSPECTABLE(IRenderer);

	virtual ~IRenderer() {}

	/*! \name Lazy Initialization Methods
	 * These initialize the current rendering subsystema and de-initialize
	 * it on-demand @{ */
	virtual void init(RendererSettings& settings) = 0;
	virtual void deactivate() = 0;
	virtual bool isInitialized() = 0;
	/*! @} */

	//! \brief Does what it says on the tin.
	virtual Resolution getResolution() = 0;

	//! \brief Does what it says on the tin.
	virtual Area getWindowSize() = 0;

	/** \name Screen Management Methods
	 * Methods used to clear and update the game display
	 * @{
	 * \brief Clears the screen before new drawing cycles. Throws
	 * exceptions. */
	virtual void clearScreen() = 0;
	/** \brief swaps backbuffer with new frame displays new image. Throws
	 * exceptions. */
	virtual void flip() = 0;
	//! @}

	virtual void blit(std::shared_ptr<void> image_data,
	                  Rectangle& placement) = 0;

	/*! \name DataType Conversion methods
	 * \brief Conversion functions to convert Rectangle objects to the types
	 * used by native APIs to update blocks of the screen.
	 *
	 * Template method bodies shall be  defined by child classes.
	 * \note These are only exposed on the public interface for test builds
	 */
	/*! @{ */
	template<typename TR>
	Rectangle toRectangle(const TR& data);

	template<typename TR>
	TR fromRectangle(const Rectangle& rectangle);
	/*! @}  */

  protected:
	IRenderer() = default;
};
} // namespace elemental

// clang-format off
// vim: set foldmethod=syntax textwidth=80 ts=8 sts=0 sw=8  noexpandtab ft=cpp.doxygen :
