/* SdlRenderer.cpp
 * Copyright © 2023 Saul D. Beniquez
 * License: Mozilla Public License v. 2.0
 *
 * This Source Code Form is subject to the terms of the Mozilla Public License,
 * v.2.0. If a copy of the MPL was not distributed with this file, You can
 * obtain one at https://mozilla.org/MPL/2.0/.
 */

#include "IRenderer.hpp"
#include "renderers/SdlRenderer.hpp"

#include "private/debuginfo.hpp"
#include "types.hpp"

using namespace elemental;

SdlRenderer::~SdlRenderer() {}

error_t
SdlRenderer::Init()
{
	return ERROR;
}

void
SdlRenderer::Deactivate()
{
}
SDL_Rect
SdlRenderer::get_SDL_Rect(const Rectangle& area) const
{
	return SDL_Rect{ static_cast<int>(area.x), static_cast<int>(area.y),
		         static_cast<int>(area.width),
		         static_cast<int>(area.height) };
}

error_t
SdlRenderer::Flip()
{

	return ERROR;
}

error_t
SdlRenderer::Blit(std::any& image_data, Rectangle& placement)
{
	return ERROR;
}

SdlRenderer::SdlRenderer() : IRenderer() {}

// clang-format off
// vim: set foldmethod=syntax textwidth=80 ts=8 sts=0 sw=8 foldlevel=99 noexpandtab ft=cpp.doxygen :
