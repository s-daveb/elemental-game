/* PaddlePositionComponent.hpp
 * Copyright © 2026 Saul D. Beniquez
 * License: Mozilla Public License v. 2.0
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v.2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/.
 */

#pragma once

#include "IComponent.hpp"
#include "IComponentFactory.hpp"
#include "types/id.hpp"

#include <cstdint>

namespace elemental {

class PaddlePositionComponent final : public IComponent
{
	IComponentFactory& factory;
	InstanceID         instance_id{ 0 };

	float pos_y{ 0.0f };
	float paddle_speed{ 400.0f };
	bool  up_pressed{ false };
	bool  down_pressed{ false };

    public:
	PaddlePositionComponent(
	    IComponentFactory& f,
	    float              y     = 0.0f,
	    float              speed = 400.0f)
	    : factory(f), pos_y(y), paddle_speed(speed)
	{
	}

	~PaddlePositionComponent() override = default;

	[[nodiscard]] auto getInstanceId() const -> InstanceID override
	{ return instance_id; }
	[[nodiscard]] auto getTypeIndex() const -> TypeInfo override
	{ return typeid(PaddlePositionComponent); }
	[[nodiscard]] auto getFactory() const -> IComponentFactory& override
	{ return factory; }

	[[nodiscard]] auto y() const -> float { return pos_y; }
	[[nodiscard]] auto speed() const -> float { return paddle_speed; }
	[[nodiscard]] auto upPressed() const -> bool { return up_pressed; }
	[[nodiscard]] auto downPressed() const -> bool { return down_pressed; }

	auto setY(float val) -> void { pos_y = val; }
	auto setSpeed(float val) -> void { paddle_speed = val; }
	auto setUpPressed(bool val) -> void { up_pressed = val; }
	auto setDownPressed(bool val) -> void { down_pressed = val; }
};

}  // namespace elemental

// clang-format off
// vim: set textwidth=80 ts=8 sts=0 sw=8 noexpandtab ft=cpp.doxygen :