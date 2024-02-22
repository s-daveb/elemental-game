/* SdlRenderer.test.cpp
 * Copyright © 2023 Saul D. Beniquez
 * License: Mozilla Public License v. 2.0
 *
 * This Source Code Form is subject to the terms of the Mozilla Public License,
 * v.2.0. If a copy of the MPL was not distributed with this file, You can
 * obtain one at https://mozilla.org/MPL/2.0/.
 */

#include "IRenderer.hpp"
#include "types/rendering.hpp"

#include "test-utils/common.hpp"

#include <any>
#include <utility>

BEGIN_TEST_SUITE("elemental::IRenderer")
{
	using namespace elemental;
	struct dummy_renderer : public IRenderer
	{
		friend class IRenderer;
		virtual ~dummy_renderer(){};

		virtual void Init(RendererSettings&) override { return; }
		virtual void Deactivate() override { return; }
		virtual bool IsInitialized() override { return false; }
		virtual Resolution GetResolution() override { return { 0, 0 }; }
		virtual Area GetWindowSize() override { return { 0, 0 }; }

		virtual void ClearScreen() override { return; }
		virtual void Flip() override { return; }

		virtual void Blit(std::shared_ptr<void> image_data,
		                  Rectangle& placement) override
		{
			return;
		}

	  protected:
		dummy_renderer() : IRenderer() {}
	};

	TEST("elemental::IRenderer::GetInstance works with "
	     "properly-derived class")
	{
		REQUIRE_NOTHROW(
		    [&]() { IRenderer::GetInstance<dummy_renderer>(); }());
		SUCCEED();
	}

	TEST("elemental::Point serialization and deserialization")
	{
		Point originalPoint{ 42, 24 };
		nlohmann::json jsonPoint = originalPoint;

		// Check serialization
		REQUIRE(jsonPoint["x"] == 42);
		REQUIRE(jsonPoint["y"] == 24);

		// Check deserialization
		Point deserializedPoint = jsonPoint;
		REQUIRE(deserializedPoint.x == 42);
		REQUIRE(deserializedPoint.y == 24);
	}

	TEST("elemental::Area serialization and deserialization")
	{
		Area originalArea{ 800, 600 };
		nlohmann::json jsonArea = originalArea;

		// Check serialization
		REQUIRE(jsonArea["width"] == 800);
		REQUIRE(jsonArea["height"] == 600);

		// Check deserialization
		Area deserializedArea = jsonArea;
		REQUIRE(deserializedArea.width == 800);
		REQUIRE(deserializedArea.height == 600);
	}

	TEST("elemental::Resolution serialization and deserialization")
	{
		Resolution originalResolution{ 1920, 1080 };
		nlohmann::json jsonResolution = originalResolution;

		// Check serialization
		REQUIRE(jsonResolution["width"] == 1920);
		REQUIRE(jsonResolution["height"] == 1080);

		// Check deserialization
		Resolution deserializedResolution = jsonResolution;
		REQUIRE(deserializedResolution.width == 1920);
		REQUIRE(deserializedResolution.height == 1080);
	}

	TEST("elemental::Rectangle serialization and deserialization")
	{
		Rectangle originalRectangle{ { 10, 20 }, { 30, 40 } };
		nlohmann::json jsonRectangle = originalRectangle;

		// Check serialization
		REQUIRE(jsonRectangle["position"]["x"] == 10);
		REQUIRE(jsonRectangle["position"]["y"] == 20);
		REQUIRE(jsonRectangle["size"]["width"] == 30);
		REQUIRE(jsonRectangle["size"]["height"] == 40);

		// Check deserialization
		Rectangle deserializedRectangle = jsonRectangle;
		REQUIRE(deserializedRectangle.position.x == 10);
		REQUIRE(deserializedRectangle.position.y == 20);
		REQUIRE(deserializedRectangle.size.width == 30);
		REQUIRE(deserializedRectangle.size.height == 40);
	}
}

// clang-format off
// vim: set foldmethod=syntax textwidth=80 ts=8 sts=0 sw=8 foldlevel=99 noexpandtab ft=cpp.doxygen :
