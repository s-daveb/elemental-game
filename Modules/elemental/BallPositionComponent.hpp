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
	IComponentFactory& factory_;
	InstanceID         instance_id_{ 0 };

	float x_{ 0.0f };
	float y_{ 0.0f };
	float vx_{ 0.0f };
	float vy_{ 0.0f };
	float radius_{ 8.0f };

    public:
	BallPositionComponent(
	    IComponentFactory& f,
	    float              x  = 0.0f,
	    float              y  = 0.0f,
	    float              vx = 300.0f,
	    float              vy = 180.0f,
	    float              r  = 8.0f)
	    : factory_(f), x_(x), y_(y), vx_(vx), vy_(vy), radius_(r)
	{
	}

	~BallPositionComponent() override = default;

	[[nodiscard]] auto getInstanceId() const -> InstanceID override
	{ return instance_id_; }
	[[nodiscard]] auto getTypeIndex() const -> TypeInfo override
	{ return typeid(BallPositionComponent); }
	[[nodiscard]] auto getFactory() const -> IComponentFactory& override
	{ return factory_; }

	[[nodiscard]] auto x() const -> float { return x_; }
	[[nodiscard]] auto y() const -> float { return y_; }
	[[nodiscard]] auto vx() const -> float { return vx_; }
	[[nodiscard]] auto vy() const -> float { return vy_; }
	[[nodiscard]] auto radius() const -> float { return radius_; }

	auto setX(float val) -> void { x_ = val; }
	auto setY(float val) -> void { y_ = val; }
	auto setVx(float val) -> void { vx_ = val; }
	auto setVy(float val) -> void { vy_ = val; }
	auto setRadius(float val) -> void { radius_ = val; }
};

}  // namespace elemental

// clang-format off
// vim: set textwidth=80 ts=8 sts=0 sw=8 noexpandtab ft=cpp.doxygen :