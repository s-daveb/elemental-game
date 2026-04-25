/* ViewComponents.cpp
 * Copyright © 2026 Saul D. Beniquez
 * License: Mozilla Public License v. 2.0
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v.2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/.
 */

#include "CircleViewComponent.hpp"
#include "RectangleViewComponent.hpp"
#include "ShapeDrawCommand.hpp"
#include "TextViewComponent.hpp"
#include "types/rendering.hpp"

namespace elemental {

auto CircleViewComponent::produceDrawCommand(IRenderer& renderer) const
    -> std::shared_ptr<IDrawCommand>
{
	uint32_t  diameter = radius * 2;
	Rectangle bounds{ position.x > radius ? position.x - radius : 0,
		          position.y > radius ? position.y - radius : 0,
		          diameter,
		          diameter };

	return std::make_shared<ShapeDrawCommand>(
	    ShapeType::FilledCircle, bounds, color, renderer);
}

auto RectangleViewComponent::produceDrawCommand(IRenderer& renderer) const
    -> std::shared_ptr<IDrawCommand>
{
	Rectangle bounds{ position.x, position.y, width, height };

	return std::make_shared<ShapeDrawCommand>(
	    ShapeType::FilledRectangle, bounds, color, renderer);
}

auto TextViewComponent::produceDrawCommand(IRenderer& renderer) const
    -> std::shared_ptr<IDrawCommand>
{
	// TODO: Implement proper text rendering with SDL_ttf
	// For now, return a placeholder rectangle
	Rectangle bounds{ position, { width, 16 } };
	return std::make_shared<ShapeDrawCommand>(
	    ShapeType::FilledRectangle, bounds, color, renderer);
}

}  // namespace elemental

// clang-format off
// vim: set textwidth=80 ts=8 sts=0 sw=8 noexpandtab ft=cpp.doxygen :