/*
 * Copyright © 2024 Saul D. Beniquez
 * License: Mozilla Public License v. 2.0
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v.2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/.
 */

#include "elemental/IDrawCommand.hpp"
#include "types/rendering.hpp"

#include "test-utils/common.hpp"

#include "IOCore/TomlTable.hpp"

#include <any>
#include <utility>

namespace {
using namespace elemental;
struct DummyDrawCommand : public IDrawCommand {
	Rectangle rect;
	std::shared_ptr<void> data;

	DummyDrawCommand(Rectangle& rect, std::shared_ptr<void>& data)
	    : IDrawCommand(rect, data), rect(rect), data(data)
	{
	}
	~DummyDrawCommand() override {}

	auto rectangle() -> Rectangle& override { return rect; }

	auto imageData() -> std::shared_ptr<void>& override { return data; }
	auto draw() -> ErrorFlag override { return IOCore::kSuccess; }
};
}

BEGIN_TEST_SUITE("elemental::IDrawCommand")
{
	using namespace elemental;

	TEST_CASE("Basic IDrawCommand functionality")
	{
		Rectangle rect{ { 0, 0 }, { 100, 200 } };
		std::shared_ptr<void> image_data = nullptr;

		DummyDrawCommand cmd(rect, image_data);

		// Test getters
		REQUIRE(cmd.rectangle().position.x == rect.position.x);
		REQUIRE(cmd.rectangle().position.y == rect.position.y);

		REQUIRE(cmd.imageData() == image_data);

		// Test draw command with valid data
		cmd.draw();
	}
}

// clang-format off
// vim: set foldmethod=syntax foldminlines=10 textwidth=80 ts=8 sts=0 sw=8 noexpandtab ft=cpp.doxygen :
