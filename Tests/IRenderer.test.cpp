/* SdlRenderer.test.cpp
 * Copyright © 2023 Saul D. Beniquez
 * License: Mozilla Public License v. 2.0
 *
 * This Source Code Form is subject to the terms of the Mozilla Public License,
 * v.2.0. If a copy of the MPL was not distributed with this file, You can
 * obtain one at https://mozilla.org/MPL/2.0/.
 */

#include "elemental/IRenderer.hpp"
#include "types/rendering.hpp"

#include "test-utils/common.hpp"

#include "IOCore/TomlTable.hpp"

#include <any>
#include <utility>

BEGIN_TEST_SUITE("elemental::IRenderer")
{
	using namespace elemental;
	struct DummyRenderer : public IRenderer {
		friend class IRenderer;
		~DummyRenderer() override = default;

		void init(RendererSettings&) override { return; }
		void deactivate() override { return; }
		auto isInitialized() -> bool override { return false; }

		auto getWindowSize() -> Area override { return { 0, 0 }; }
		auto getResolution() -> Resolution override { return { 0, 0 }; }

		void clearScreen() override { return; }
		void flip() override { return; }

		void blit(std::shared_ptr<void> image_data, Rectangle& placement)
		    override
		{
			return;
		}

	    protected:
		DummyRenderer() : IRenderer() {}
	};

	TEST("elemental::IRenderer::GetInstance works with "
	     "properly-derived class")
	{
		REQUIRE_NOTHROW([&]() {
			IRenderer::GetInstance<DummyRenderer>();
		}());
		SUCCEED();
	}

	TEST("elemental::Point serialization and deserialization")
	{
		Point original_point{ 42, 24 };
		IOCore::TomlTable toml_point = original_point;

		// Check serialization
		REQUIRE(toml_point["x"] == 42);
		REQUIRE(toml_point["y"] == 24);

		// Check deserialization
		auto deserialized_point = toml_point.as<Point>();
		REQUIRE(deserialized_point.x == 42);
		REQUIRE(deserialized_point.y == 24);
	}

	TEST("elemental::Area serialization and deserialization")
	{
		Area original_area{ 800, 600 };
		IOCore::TomlTable toml_area = original_area;

		// Check serialization
		REQUIRE(toml_area["width"] == 800);
		REQUIRE(toml_area["height"] == 600);

		// Check deserialization
		auto deserialized_area = toml_area.as<Area>();
		REQUIRE(deserialized_area.width == 800);
		REQUIRE(deserialized_area.height == 600);
	}

	TEST("elemental::Resolution serialization and deserialization")
	{
		Resolution original_resolution{ 1920, 1080 };
		IOCore::TomlTable toml_resolution = original_resolution;

		// Check serialization
		REQUIRE(toml_resolution["width"] == 1920);
		REQUIRE(toml_resolution["height"] == 1080);

		// Check deserialization
		auto deserialized_resolution = toml_resolution.as<Resolution>();
		REQUIRE(deserialized_resolution.width == 1920);
		REQUIRE(deserialized_resolution.height == 1080);
	}

	TEST("elemental::Rectangle serialization and deserialization")
	{
		Rectangle original_rectangle{ { 10, 20 }, { 30, 40 } };
		IOCore::TomlTable toml_rectangle = original_rectangle;

		// Check serialization
		REQUIRE(toml_rectangle["position"]["x"] == 10);
		REQUIRE(toml_rectangle["position"]["y"] == 20);
		REQUIRE(toml_rectangle["size"]["width"] == 30);
		REQUIRE(toml_rectangle["size"]["height"] == 40);

		// Check deserialization
		auto deserialized_rectangle = toml_rectangle.as<Rectangle>();
		REQUIRE(deserialized_rectangle.position.x == 10);
		REQUIRE(deserialized_rectangle.position.y == 20);
		REQUIRE(deserialized_rectangle.size.width == 30);
		REQUIRE(deserialized_rectangle.size.height == 40);
	}
}

// clang-format off
// vim: set foldmethod=syntax textwidth=80 ts=8 sts=0 sw=8  noexpandtab ft=cpp.doxygen :
