/* SdlRenderer.test.cpp
 * Copyright © 2023 Saul D. Beniquez
 * License: Mozilla Public License v. 2.0
 *
 * This Source Code Form is subject to the terms of the Mozilla Public License,
 * v.2.0. If a copy of the MPL was not distributed with this file, You can
 * obtain one at https://mozilla.org/MPL/2.0/.
 */

/** @TODO:  move this to a manual test suite that configures a window and
 * renderer and does some automated tests */

#include "IRenderer.hpp"
#include "renderers/SdlRenderer.hpp"

#include "./common.hpp"

BEGIN_TEST_SUITE("elemental::SdlRenderer")
{
	using namespace elemental;

	TEST("elemental::SdlRenderer - GetInstance works")
	{
		REQUIRE_NOTHROW([&]() {
			auto* result = IRenderer::GetInstance<SdlRenderer>();

			REQUIRE(nullptr != result);
		}());
		SUCCEED();
	}

	TEST("elemental::SdlRenderer - convert Rectangle to SDL_Rect")
	{
		REQUIRE_NOTHROW([&]() {
			auto* renderer = IRenderer::GetInstance<SdlRenderer>();
			auto test_input = Rectangle{ 0, 1, 10, 20 };
			auto result = renderer->get_SDL_Rect(test_input);

			CHECK(result.x == test_input.x);
			CHECK(result.y == test_input.y);
			CHECK(result.w == test_input.width);
			CHECK(result.h == test_input.height);
		}());
	}
}

// clang-format off
// vim: set foldmethod=syntax textwidth=80 ts=8 sts=0 sw=8 foldlevel=99 noexpandtab ft=cpp.doxygen :
