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
	IComponentFactory& factory_;
	InstanceID         instance_id_{ 0 };

	float y_{ 0.0f };
	float speed_{ 400.0f };
	bool  up_pressed_{ false };
	bool  down_pressed_{ false };

    public:
	PaddlePositionComponent(
	    IComponentFactory& f,
	    float              y     = 0.0f,
	    float              speed = 400.0f)
	    : factory_(f), y_(y), speed_(speed)
	{
	}

	~PaddlePositionComponent() override = default;

	[[nodiscard]] auto getInstanceId() const -> InstanceID override
	{ return instance_id_; }
	[[nodiscard]] auto getTypeIndex() const -> TypeInfo override
	{ return typeid(PaddlePositionComponent); }
	[[nodiscard]] auto getFactory() const -> IComponentFactory& override
	{ return factory_; }

	[[nodiscard]] auto y() const -> float { return y_; }
	[[nodiscard]] auto speed() const -> float { return speed_; }
	[[nodiscard]] auto upPressed() const -> bool { return up_pressed_; }
	[[nodiscard]] auto downPressed() const -> bool { return down_pressed_; }

	auto setY(float val) -> void { y_ = val; }
	auto setSpeed(float val) -> void { speed_ = val; }
	auto setUpPressed(bool val) -> void { up_pressed_ = val; }
	auto setDownPressed(bool val) -> void { down_pressed_ = val; }
};

}  // namespace elemental

// clang-format off
// vim: set textwidth=80 ts=8 sts=0 sw=8 noexpandtab ft=cpp.doxygen :