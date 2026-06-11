/* IRenderer.hpp
 * Copyright © 2023 Saul D. Beniquez
 * License: Mozilla Public License v. 2.0
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v.2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/ .
 */

#pragma once

#include "util/testing.hpp"

#include "IOCore/types.hpp"

#include "Singleton.hpp"
#include "StateCommand.hpp"
#include "types/color.hpp"
#include "types/rendering.hpp"

#include <SDL.h>
#include <SDL_ttf.h>

#include <any>
#include <cstdint>
#include <memory>
#include <string>
#include <type_traits>
#include <utility>

namespace elemental {

struct IRenderer
{
	/*! \brief This macro is used in child classes to construct instances
	 * of the derived classes. \see Singleton.hpp  */
	SINGLETON_INTERFACE(IRenderer);

	/* This macro adds a friend class to this class definition that
	 * can be used in unit tests to view private variables */
	TEST_INSPECTABLE(IRenderer);

	virtual ~IRenderer() = default;

	/*! \name Lazy Initialization Methods
	 * These initialize the current rendering subsystema and
	 * de-initialize it on-demand \{ */
	virtual auto init(RendererSettings& settings) -> void = 0;
	virtual auto deactivate() -> void                     = 0;
	virtual auto isInitialized() -> bool                  = 0;
	/*! \} */

	virtual auto getResolution() -> Resolution = 0;
	virtual auto getWindowSize() -> Area       = 0;

	virtual auto queueStateCommand(StateCommand cmd) -> void = 0;

	/** \name Screen Management Methods
	 * Methods used to clear and update the game display
	 * \{
	 * \brief Clears the screen before new drawing cycles. Throws
	 * exceptions. */
	virtual void clearScreen() = 0;
	/** \brief swaps backbuffer with new frame displays new image. Throws
	 * exceptions. */
	virtual void flip() = 0;
	//! \}

	virtual void blit(
	    std::shared_ptr<void> image_data,
	    const Rectangle&      placement) = 0;

	/** \brief Initialize the renderer with a system font for fallback
	 * display
	 * \param font Font handle for initialization
	 */
	virtual void setFont(SdlPtr<TTF_Font>&& font) = 0;

	/*! \name Texture Queue Methods
	 * Methods for managing a thread-safe texture queue for text rendering.
	 * \{ */
	virtual void queueTextTexture(
	    const std::string& text,
	    FontHandle         font,
	    const Color&       color) = 0;

	virtual void processTextureQueue() = 0;

	virtual std::shared_ptr<void> getTextTexture(
	    const std::string& text) = 0;

	virtual bool hasTextTexture(const std::string& text) const = 0;
	/*! \} */

	/*! \name Primitive Drawing Methods
	 * Methods for drawing primitive shapes directly without textures.
	 * \{ */
	virtual void drawFilledCircle(
	    int32_t      x,
	    int32_t      y,
	    int32_t      radius,
	    const Color& color) = 0;

	virtual void drawFilledRect(
	    const Rectangle& rect,
	    const Color&     color) = 0;
	/*! \} */

	/*! \name DataType Conversion methods
	 * \brief Conversion functions to convert Rectangle objects to the
	 * types used by native APIs to update blocks of the screen.
	 *
	 * Template method bodies shall be  defined by child classes.
	 * \note These are only exposed on the public interface for test
	 * builds
	 */
	/*! \{ */
	template<typename TR>
	auto static toRectangle(const TR& data) -> Rectangle;

	template<typename TR>
	auto static fromRectangle(const Rectangle& rectangle) -> TR;
	/*! \}  */

    protected:
	IRenderer() = default;
};
}  // namespace elemental

// clang-format off
// vim: set textwidth=80 ts=8 sts=0 sw=8  noexpandtab ft=cpp.doxygen :
