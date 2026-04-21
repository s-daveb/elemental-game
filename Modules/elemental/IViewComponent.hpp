/* IViewComponent.hpp
 * Copyright © 2026 Saul D. Beniquez
 * License: Mozilla Public License v. 2.0
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v.2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/.
 */

#pragma once

#include "IComponent.hpp"
#include "IDrawCommand.hpp"
#include "IRenderer.hpp"
#include "types/color.hpp"
#include "types/id.hpp"
#include "types/rendering.hpp"

#include <cstdint>
#include <list>
#include <memory>

namespace elemental {

struct IViewComponent : public IComponent
{
	virtual ~IViewComponent() = default;

	[[nodiscard]] virtual auto produceDrawCommand(IRenderer& renderer) const
	    -> std::shared_ptr<IDrawCommand> = 0;

	virtual auto               setPosition(Point pos) -> void = 0;
	[[nodiscard]] virtual auto getPosition() const -> Point   = 0;
	[[nodiscard]] virtual auto getColor() const -> Color      = 0;
	virtual auto               setColor(Color color) -> void  = 0;
};

}  // namespace elemental

// clang-format off
// vim: set textwidth=80 ts=8 sts=0 sw=8 noexpandtab ft=cpp.doxygen :