/* SdlRenderer.test.cpp
 * Copyright © 2023 Saul D. Beniquez
 * License: Mozilla Public License v. 2.0
 *
 * This Source Code Form is subject to the terms of the Mozilla Public License,
 * v.2.0. If a copy of the MPL was not distributed with this file, You can
 * obtain one at https://mozilla.org/MPL/2.0/.
 */

#include "IRenderer.hpp"

#include "./common.hpp"

BEGIN_TEST_SUITE("elemental::IRenderer")
{
	using namespace elemental;
	struct dummy_renderer : public IRenderer
	{
		friend class IRenderer;
		virtual ~dummy_renderer(){};

	  protected:
		dummy_renderer() : IRenderer() {}
	};

	TEST("elemental::IRenderer - GetInstance works with "
	     "properly-derived class")
	{
		REQUIRE_NOTHROW(
		    [&]() { IRenderer::GetInstance<dummy_renderer>(); }());
		SUCCEED();
	}
}

// clang-format off
// vim: set foldmethod=syntax textwidth=80 ts=8 sts=0 sw=8 foldlevel=99 noexpandtab ft=cpp.doxygen :
