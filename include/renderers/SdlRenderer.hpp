/* SdlRenderer.hpp
 * Copyright © 2023 Saul D. Beniquez
 * License: Mozilla Public License v. 2.0
 *
 * This Source Code Form is subject to the terms of the Mozilla Public License,
 * v.2.0. If a copy of the MPL was not distributed with this file, You can
 * obtain one at https://mozilla.org/MPL/2.0/ .
 */

#include "IRenderer.hpp"

namespace elemental {

struct Rectangle;

struct SdlRenderer : public IRenderer
{
	friend class IRenderer;
	virtual ~SdlRenderer();

	virtual error_t Init() override;
	virtual void Deactivate() override;

	virtual error_t Flip() override;
	virtual error_t Blit(std::any& image, Rectangle& placement) override;

#ifndef UNIT_TEST
  protected:
#endif
	SDL_Rect get_SDL_Rect(const Rectangle&) const;
	SdlRenderer();
};

}

// clang-format off
// vim: set foldmethod=syntax textwidth=80 ts=8 sts=0 sw=8 foldlevel=99 noexpandtab ft=cpp.doxygen :
