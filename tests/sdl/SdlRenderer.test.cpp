/* SdlRenderer.test.cpp
 * Copyright © 2023 Saul D. Beniquez
 * License: Mozilla Public License v. 2.0
 *
 * This Source Code Form is subject to the terms of the Mozilla Public License,
 * v.2.0. If a copy of the MPL was not distributed with this file, You can
 * obtain one at https://mozilla.org/MPL/2.0/.
 */

#include "SdlRenderer.hpp"
#include "IRenderer.hpp"
#include "sys/platform.hpp"

#include "SDL_Memory.thpp"

#include "test-utils/SdlHelpers.hpp"
#include "test-utils/common.hpp"

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_render.h>

#include <chrono>
#include <thread>

BEGIN_TEST_SUITE("elemental::SdlRenderer")
{
	using namespace elemental;

	struct SdlRendererFixture : public SdlTestFixture
	{
		SdlRendererFixture()
		    : SdlTestFixture()
		    , test_renderer(IRenderer::GetInstance<SdlRenderer>())
		{
			settings = { { "Test",
				       WindowMode::Windowed,      // mode
				       WindowPlacement::Centered, // placement
				       { 0, 0 },                  // window.pos
				       { 1024, 768 } },           // window.size
				     { 1024, 768 } }; // renderer res
		}

		~SdlRendererFixture() { test_renderer.Deactivate(); }
		SdlRenderer& test_renderer;
		RendererSettings settings;
	};

	TEST("elemental::SdlRenderer - GetInstance works")
	{
		REQUIRE_NOTHROW([&]() {
			auto& result = IRenderer::GetInstance<SdlRenderer>();
		}());
		SUCCEED();
	}

	TEST("elemental::SdlRenderer - convert Rectangle to SDL_Rect")
	{
		REQUIRE_NOTHROW([&]() {
			auto& renderer = IRenderer::GetInstance<SdlRenderer>();
			auto test_input = Rectangle{ 0, 1, 10, 20 };
			SDL_Rect result =
			    renderer.FromRectangle<SDL_Rect>(test_input);

			CHECK(result.x == test_input.x);
			CHECK(result.y == test_input.y);
			CHECK(result.w == test_input.width);
			CHECK(result.h == test_input.height);
		}());
	}
	TEST("elemental::SdlRenderer - convert SDL_Rect to Rectangle")
	{
		REQUIRE_NOTHROW([&]() {
			auto& renderer = IRenderer::GetInstance<SdlRenderer>();
			auto test_input = SDL_Rect{ 0, 1, 10, 20 };
			Rectangle result =
			    renderer.ToRectangle<SDL_Rect>(test_input);

			CHECK(result.x == test_input.x);
			CHECK(result.y == test_input.y);
			CHECK(result.width == test_input.w);
			CHECK(result.height == test_input.h);
		}());
	}

#if !defined(NO_GUI)
	TEST_CASE_METHOD(SdlRendererFixture,
	                 "elemental::SdlRenderer - Initialize Renderer")
	{
		// 1. Default initialization does not throw errors
		CHECK(false == test_renderer.is_initialized);
		test_renderer.Init(settings);
		CHECK(true == test_renderer.is_initialized);

		// 2. Verify that an SDL_Window and SDL_Renderer have been
		// created
		REQUIRE(test_renderer.sdl_window_ptr != nullptr);
		REQUIRE(test_renderer.sdl_renderer_ptr != nullptr);
	}
	TEST_CASE_METHOD(SdlRendererFixture,
	                 "elemental::SdlRenderer - Deactivate Renderer")
	{
		// 1. Deactivate method de-initializes
		test_renderer.Init(settings);
		REQUIRE(true == test_renderer.is_initialized);

		test_renderer.Deactivate();
		REQUIRE(false == test_renderer.is_initialized);

		// 2. Verify that an SDL_Window and SDL_Renderer ptrs have been
		// cleared
		REQUIRE(test_renderer.sdl_window_ptr == nullptr);
		REQUIRE(test_renderer.sdl_renderer_ptr == nullptr);
	}
	TEST_CASE_METHOD(SdlRendererFixture,
	                 "elemental::SdlRenderer - IsInitialized Accessor")
	{
		// 1. Default initialization does not throw errors
		CHECK(test_renderer.IsInitialized() ==
		      test_renderer.is_initialized);
		test_renderer.Init(settings);
		CHECK(test_renderer.IsInitialized() ==
		      test_renderer.is_initialized);
	}

	TEST_CASE_METHOD(SdlRendererFixture,
	                 "elemental::SdlRenderer - GetResolution works")
	{
		// 1. Before initialization, throws error
		REQUIRE_THROWS([this]() { test_renderer.GetResolution(); }());

		// 2. After initializtion, the method works
		test_renderer.Init(settings);
		auto resolution_data = test_renderer.GetResolution();

		REQUIRE(resolution_data.width == settings.resolution.width);
		REQUIRE(resolution_data.height == settings.resolution.height);
	}
	TEST_CASE_METHOD(SdlRendererFixture,
	                 "elemental::SdlRenderer - GetWindowSize works")
	{
		// 1. Before initialization, throws error
		REQUIRE_THROWS([this]() { test_renderer.GetWindowSize(); }());

		// 2. After initializtion, the method works
		test_renderer.Init(settings);
		auto window_data = test_renderer.GetWindowSize();

		REQUIRE(window_data.width == settings.window.size.width);
		REQUIRE(window_data.height == settings.window.size.height);
	}
	TEST_CASE_METHOD(SdlRendererFixture,
	                 "elemental::SdlRenderer - ClearScreen works")
	{
		// 1. Before initialization, throws error
		REQUIRE_THROWS([this]() { test_renderer.ClearScreen(); }());

		// 2. After initializtion, the method works
		test_renderer.Init(settings);
		test_renderer.ClearScreen();
	}
	TEST_CASE_METHOD(SdlRendererFixture,
	                 "elemental::SdlRenderer - Flip works")
	{
		// 1. Before initialization, throws error
		REQUIRE_THROWS([this]() { test_renderer.Flip(); }());

		// 2. After initializtion, the method works
		test_renderer.Init(settings);
		test_renderer.Flip();
	}
	TEST_CASE_METHOD(SdlRendererFixture,
	                 "elemental::SdlRenderer - Blit works")
	{
		using std::chrono::seconds;
		unique_sdl_ptr<SDL_Surface> image_surf_ptr;
		unique_sdl_ptr<SDL_Texture> img_texture_ptr = nullptr;
		Rectangle location{ 10, 10, 100, 100 };

		// Prep: Initialize a SdlRenderer and load an image texture
		// using the SDL_Renderer* ptr inside
		test_renderer.Init(settings);
		test_renderer.ClearScreen();
		image_surf_ptr = IMG_Load("data/tests/test-skull.png");
		REQUIRE(image_surf_ptr != nullptr);

		REQUIRE(test_renderer.sdl_renderer_ptr != nullptr);
		img_texture_ptr = SDL_CreateTextureFromSurface(
		    test_renderer.sdl_renderer_ptr, image_surf_ptr);
		REQUIRE(img_texture_ptr != nullptr);

		// 1. Renderer becomes invalid before blitting!

		auto temporary_render_storage =
		    std::move(this->test_renderer.sdl_renderer_ptr);
		REQUIRE_THROWS([&]() {
			// set an invalid SDL_Renderer pointer
			test_renderer.sdl_renderer_ptr = nullptr;
			// 1. Before initialization, throws error
			test_renderer.Blit(img_texture_ptr.get(), location);
		}());

		// 2. With a valid, initialized SdlRenderer, the
		//   method works
		test_renderer.sdl_renderer_ptr.swap(temporary_render_storage);
		test_renderer.Blit(img_texture_ptr.get(), location);
		// Display the image and pause so the user can see it!
		test_renderer.Flip();
		if (platform::MACOSX) {
			for (unsigned i = 0; i < 500; i++) {
				SDL_PumpEvents();
			}
		}
		std::this_thread::sleep_for(seconds(2));
	}
#endif
}

// clang-format off
// vim: set foldmethod=syntax textwidth=80 ts=8 sts=0 sw=8 foldlevel=99 noexpandtab ft=cpp.doxygen :
