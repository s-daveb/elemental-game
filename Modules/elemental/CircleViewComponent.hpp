/* CircleViewComponent.hpp
 * Copyright © 2026 Saul D. Beniquez
 * License: Mozilla Public License v. 2.0
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v.2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/.
 */

#pragma once

#include "IViewComponent.hpp"
#include "types/color.hpp"
#include "types/id.hpp"
#include "types/rendering.hpp"

#include <cstdint>

namespace elemental {

class CircleViewComponent final : public IViewComponent
{
	IComponentFactory& factory;
	Point              position{ 0, 0 };
	uint32_t           radius{ 16 };
	Color              color{ 255, 255, 255, 255 };
	InstanceID         instance_id{ 0 };

    public:
	CircleViewComponent(
	    IComponentFactory& f,
	    Point              pos = { 0, 0 },
	    uint32_t           rad = 16,
	    Color              col = { 255, 255, 255, 255 })
	    : factory(f), position(pos), radius(rad), color(col)
	{
	}

	~CircleViewComponent() override = default;

	[[nodiscard]] auto getInstanceId() const -> InstanceID override
	{ return instance_id; }
	[[nodiscard]] auto getTypeIndex() const -> TypeInfo override
	{ return typeid(CircleViewComponent); }
	[[nodiscard]] auto getFactory() const -> IComponentFactory& override
	{ return factory; }

	[[nodiscard]] auto produceDrawCommand(IRenderer& renderer) const
	    -> std::shared_ptr<IDrawCommand> override;

	auto setPosition(Point pos) -> void override { position = pos; }
	[[nodiscard]] auto getPosition() const -> Point override
	{ return position; }
	[[nodiscard]] auto getColor() const -> Color override { return color; }
	auto               setColor(Color col) -> void override { color = col; }

	[[nodiscard]] auto getRadius() const -> uint32_t { return radius; }
	auto               setRadius(uint32_t r) -> void { radius = r; }
};

}  // namespace elemental

// clang-format off
// vim: set textwidth=80 ts=8 sts=0 sw=8 noexpandtab ft=cpp.doxygen :