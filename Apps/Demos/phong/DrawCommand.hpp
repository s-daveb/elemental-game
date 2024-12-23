/*
 * Copyright © 2024 Saul D. Beniquez
 * License: Mozilla Public License v. 2.0
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v.2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/.
 */

#include <SDL_render.h>
#include <exception>
#include <memory>

#include "IDrawable.hpp"
#include "IRenderer.hpp"
#include "types/errors.hpp"
#include "types/rendering.hpp"

namespace elemental {

struct DrawCommand : public IDrawable {
	DrawCommand(
	    IRenderer& renderer, std::shared_ptr<void> data, Rectangle rect
	)
	    : IDrawable(), renderer(renderer), texture(data), rect(rect)
	{
	}

	auto draw(const Rectangle& rect) -> ErrorFlag override
	{
		try {
			renderer.blit(texture, rect);
			return kSuccess;
		} catch (const std::exception& e) {
			return kError;
		}
	}

	auto draw() -> ErrorFlag override { return this->draw(this->rect); }

	IRenderer& renderer;

	std::shared_ptr<void> texture;
	Rectangle rect;
};

}
// clang-format off
// vim: set foldmethod=syntax foldminlines=10 textwidth=80 ts=8 sts=0 sw=8 noexpandtab ft=cpp.doxygen :
