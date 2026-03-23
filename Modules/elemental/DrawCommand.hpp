/*
 * Copyright © 2024 Saul D. Beniquez
 * License: Mozilla Public License v. 2.0
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v.2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/.
 */

#include "IOCore/types/errors.hpp"

#include "IDrawCommand.hpp"
#include "IRenderer.hpp"
#include "types/rendering.hpp"

#include <SDL_render.h>

#include <exception>
#include <memory>

namespace elemental {
using namespace IOCore;

struct DrawCommand : public IDrawCommand
{
	DrawCommand(
	    IRenderer&            renderer,
	    Rectangle             rect,
	    std::shared_ptr<void> data)
	    : IDrawCommand(rect, data), renderer(renderer), texture(data),
	      rect(rect)
	{
	}

	auto rectangle() -> Rectangle& override { return this->rect; }
	auto imageData() -> std::shared_ptr<void>& override
	{ return this->texture; }

	auto draw() -> ErrorFlag override
	{
		try {
			renderer.blit(texture, rect);
			return kSuccess;
		} catch (const std::exception& e) {
			return kError;
		}
	}

	IRenderer& renderer;

	std::shared_ptr<void> texture;
	Rectangle             rect;
};

}  // namespace elemental
// clang-format off
// vim: set  textwidth=80 ts=8 sts=0 sw=8 noexpandtab ft=cpp.doxygen :
