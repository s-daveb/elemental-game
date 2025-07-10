/*
 * Copyright © 2024 Saul D. Beniquez
 * License: Mozilla Public License v. 2.0
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v.2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/.
 */

#include "IDrawable.hpp"
#include "fakeit.hpp"
#include "test-utils/common.hpp"

using fakeit::Mock;

class TestFixture {

	TestFixture() {}
	virtual ~TestFixture() default;

	Mock<IDrawable> mockDrawble;
	IRenderer& renderer;

	void SetUp() override
	{
		// Initialize your renderer here
		renderer = some_renderer;
	}

	auto CreateDrawable() -> std::any { return mockDrawble.fake(); }
};
} // namespace elemental

TEST_WITH_FIXTURE(IDrawableTest, test_draw_success)
{
	WHEN(mockDrawable.draw(Rectangle{}))
	{
		fake().should(BeCalled()).andReturn(ErrorFlag::Success);
	}

	auto drawable = CreateDrawable();

	THEN("The draw method should return Success")
	{
		Expect(mockDrawable.draw).WasCalled(1);
		Expect(drawable->draw(Rectangle{}) == ErrorFlag::Success);
	}
}

TEST_WITH_FIXTURE(IDrawableTest, test_draw_failure)
{
	WHEN(mockDrawable.draw(Rectangle{}))
	{
		fake().should(BeCalled()).andReturn(ErrorFlag::Failure);
	}

	auto drawable = CreateDrawable();

	THEN("The draw method should return Failure")
	{
		Expect(mockDrawable.draw).WasCalled(1);
		Expect(drawable->draw(Rectangle{}) == ErrorFlag::Failure);
	}
}

// clang-format off
// vim: set  textwidth=80 ts=8 sts=0 sw=8 noexpandtab ft=cpp.doxygen :
