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
#include "types/rendering.hpp"

namespace elemental {

auto CircleViewComponent::produceDrawCommand(IRenderer& renderer)
    -> std::shared_ptr<IDrawCommand>
{
	uint32_t  diameter = radius_ * 2;
	Rectangle bounds{ position_.x > radius_ ? position_.x - radius_ : 0,
		          position_.y > radius_ ? position_.y - radius_ : 0,
		          diameter,
		          diameter };

	return std::make_shared<ShapeDrawCommand>(
	    ShapeType::FilledCircle, bounds, color_, renderer);
}

auto RectangleViewComponent::produceDrawCommand(IRenderer& renderer)
    -> std::shared_ptr<IDrawCommand>
{
	Rectangle bounds{ position_.x, position_.y, width_, height_ };

	return std::make_shared<ShapeDrawCommand>(
	    ShapeType::FilledRectangle, bounds, color_, renderer);
}

}  // namespace elemental

// clang-format off
// vim: set textwidth=80 ts=8 sts=0 sw=8 noexpandtab ft=cpp.doxygen :