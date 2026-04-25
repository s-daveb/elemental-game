/* BallPositionComponent.hpp
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

class BallPositionComponent final : public IComponent
{
	IComponentFactory& factory;
	InstanceID         instance_id{ 0 };

	float pos_x{ 0.0f };
	float pos_y{ 0.0f };
	float vel_x{ 0.0f };
	float vel_y{ 0.0f };
	float ball_radius{ 8.0f };

    public:
	BallPositionComponent(
	    IComponentFactory& f,
	    float              x  = 0.0f,
	    float              y  = 0.0f,
	    float              vx = 300.0f,
	    float              vy = 180.0f,
	    float              r  = 8.0f)
	    : factory(f), pos_x(x), pos_y(y), vel_x(vx), vel_y(vy),
	      ball_radius(r)
	{
	}

	~BallPositionComponent() override = default;

	[[nodiscard]] auto getInstanceId() const -> InstanceID override
	{ return instance_id; }
	[[nodiscard]] auto getTypeIndex() const -> TypeInfo override
	{ return typeid(BallPositionComponent); }
	[[nodiscard]] auto getFactory() const -> IComponentFactory& override
	{ return factory; }

	[[nodiscard]] auto x() const -> float { return pos_x; }
	[[nodiscard]] auto y() const -> float { return pos_y; }
	[[nodiscard]] auto vx() const -> float { return vel_x; }
	[[nodiscard]] auto vy() const -> float { return vel_y; }
	[[nodiscard]] auto radius() const -> float { return ball_radius; }

	auto setX(float val) -> void { pos_x = val; }
	auto setY(float val) -> void { pos_y = val; }
	auto setVx(float val) -> void { vel_x = val; }
	auto setVy(float val) -> void { vel_y = val; }
	auto setRadius(float val) -> void { ball_radius = val; }
};

}  // namespace elemental

// clang-format off
// vim: set textwidth=80 ts=8 sts=0 sw=8 noexpandtab ft=cpp.doxygen :