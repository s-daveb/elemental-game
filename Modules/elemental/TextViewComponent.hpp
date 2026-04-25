/* TextViewComponent.hpp
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
#include "types/rendering.hpp"

#include <cstdint>
#include <string>

namespace elemental {

class TextViewComponent final : public IViewComponent
{
	IComponentFactory& factory_;
	Point              position{ 0, 0 };
	uint32_t           width{ 0 };
	Color              color{ 255, 255, 255, 255 };
	InstanceID         instance_id{ 0 };
	std::string        text;

    public:
	TextViewComponent(
	    IComponentFactory& f,
	    Point              pos      = { 0, 0 },
	    uint32_t           w        = 0,
	    Color              col      = { 255, 255, 255, 255 },
	    const std::string& text_arg = "")
	    : factory_(f), position(pos), width(w), color(col), text(text_arg)
	{
	}

	~TextViewComponent() override = default;

	[[nodiscard]] auto getInstanceId() const -> InstanceID override
	{ return instance_id; }
	[[nodiscard]] auto getTypeIndex() const -> TypeInfo override
	{ return typeid(TextViewComponent); }
	[[nodiscard]] auto getFactory() const -> IComponentFactory& override
	{ return factory_; }

	[[nodiscard]] auto produceDrawCommand(IRenderer& renderer) const
	    -> std::shared_ptr<IDrawCommand> override;

	auto setPosition(Point pos) -> void override { position = pos; }
	[[nodiscard]] auto getPosition() const -> Point override
	{ return position; }
	[[nodiscard]] auto getColor() const -> Color override { return color; }
	auto               setColor(Color col) -> void override { color = col; }

	[[nodiscard]] auto getWidth() const -> uint32_t { return width; }
	auto               setWidth(uint32_t w) -> void { width = w; }

	[[nodiscard]] auto getText() const -> std::string { return text; }
	auto               setText(const std::string& t) -> void { text = t; }
};

}  // namespace elemental

// clang-format off
// vim: set textwidth=80 ts=8 sts=0 sw=8 noexpandtab ft=cpp.doxygen :
