/* SdlRenderer.test.cpp
 * Copyright © 2023 Saul D. Beniquez
 * License: Mozilla Public License v. 2.0
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v.2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/.
 */

#include "test-utils/common.hpp"

#include "IOCore/TomlTable.hpp"

#include "elemental/IRenderer.hpp"

#include "types/rendering.hpp"

#include <any>
#include <utility>

BEGIN_TEST_SUITE("elemental::IRenderer")
{
	using namespace elemental;
	struct DummyRenderer : public IRenderer
	{
		friend class IRenderer;
		~DummyRenderer() override = default;

		void init(RendererSettings&) override { return; }
		void deactivate() override { return; }
		auto isInitialized() -> bool override { return false; }

		auto getWindowSize() -> Area override { return { 0, 0 }; }
		auto getResolution() -> Resolution override { return { 0, 0 }; }

		void clearScreen() override { return; }
		void flip() override { return; }

		void blit(std::shared_ptr<void> image_data,
		          const Rectangle&      placement) override
		{
		}

		virtual void blit(std::shared_ptr<void> image_data,
		                  Rectangle&            placement)
		{ return; }

		void drawFilledCircle(int32_t      x,
		                      int32_t      y,
		                      int32_t      radius,
		                      const Color& color) override
		{
		}

		void drawFilledRect(const Rectangle& rect,
		                    const Color&     color) override
		{
		}

	    protected:
		DummyRenderer() : IRenderer() {}
	};

	TEST("elemental::IRenderer::GetInstance works with "
	     "properly-derived class")
	{
		REQUIRE_NOTHROW(
		    [&]() { IRenderer::GetInstance<DummyRenderer>(); }());
		SUCCEED();
	}
}

// clang-format off
// vim: set textwidth=80 ts=8 sts=0 sw=8  noexpandtab ft=cpp.doxygen :
