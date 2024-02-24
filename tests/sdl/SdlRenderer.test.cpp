/* SdlRenderer.test.cpp
 * Copyright © 2023 Saul D. Beniquez
 * License: Mozilla Public License v. 2.0
 *
 * This Source Code Form is subject to the terms of the Mozilla Public License,
 * v.2.0. If a copy of the MPL was not distributed with this file, You can
 * obtain one at https://mozilla.org/MPL/2.0/.
 */

#include <SDL.h>
#include <SDL_image.h>

#include "IRenderer.hpp"
#include "SdlRenderer.hpp"
#include "sys/platform.hpp"

#include "test-utils/SdlHelpers.hpp"
#include "test-utils/common.hpp"

#include <chrono>
#include <thread>

namespace elemental::debug {
template<>
struct Inspector<SdlRenderer>
{
	struct
	{
		bool& is_initialized;
		SdlPtr<SDL_Window>& sdl_window_ptr;
		SdlPtr<SDL_Renderer>& sdl_renderer_ptr;
	} state;

	Inspector(SdlRenderer& subject)
	    : state{ subject.is_initialized, subject.sdl_window_ptr,
		     subject.sdl_renderer_ptr } {};
};
} // namespace elemental::debug

BEGIN_TEST_SUITE("elemental::SdlRenderer")
{
	using namespace elemental;
	using namespace elemental::debug;

	struct SdlRendererFixture : public SdlTestFixture
	{
		SdlRendererFixture()
		    : SdlTestFixture()
		    , settings()
		    , test_renderer(IRenderer::GetInstance<SdlRenderer>())
		    , renderer_info(test_renderer)
		{
			settings = { { "Test",
				       WindowMode::Windowed,      // mode
				       WindowPlacement::Centered, // placement
				       { 0, 0 },                  // window.pos
				       { 1024, 768 } },           // window.size
				     { 1024, 768 } }; // renderer res
		}

		~SdlRendererFixture() { test_renderer.Deactivate(); }

		RendererSettings settings;
		SdlRenderer& test_renderer;
		Inspector<SdlRenderer> renderer_info;
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
		CHECK(false == renderer_info.state.is_initialized);
		test_renderer.Init(settings);
		CHECK(true == renderer_info.state.is_initialized);

		// 2. Verify that an SDL_Window and SDL_Renderer have been
		// created
		REQUIRE(renderer_info.state.sdl_window_ptr != nullptr);
		REQUIRE(renderer_info.state.sdl_renderer_ptr != nullptr);
	}
	TEST_CASE_METHOD(SdlRendererFixture,
	                 "elemental::SdlRenderer - Deactivate Renderer")
	{
		// 1. Deactivate method de-initializes
		test_renderer.Init(settings);
		REQUIRE(true == renderer_info.state.is_initialized);

		test_renderer.Deactivate();
		REQUIRE(false == renderer_info.state.is_initialized);

		// 2. Verify that an SDL_Window and SDL_Renderer ptrs have been
		// cleared
		REQUIRE(renderer_info.state.sdl_window_ptr == nullptr);
		REQUIRE(renderer_info.state.sdl_renderer_ptr == nullptr);
	}
	TEST_CASE_METHOD(SdlRendererFixture,
	                 "elemental::SdlRenderer - IsInitialized Accessor")
	{
		// 1. Default initialization does not throw errors
		CHECK(test_renderer.IsInitialized() ==
		      renderer_info.state.is_initialized);
		test_renderer.Init(settings);
		CHECK(test_renderer.IsInitialized() ==
		      renderer_info.state.is_initialized);
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

		UniqueSdlPtr<SDL_Surface> image_surf_ptr;
		SdlPtr<SDL_Texture> img_texture_ptr = nullptr;
		Rectangle location{ 10, 10, 100, 100 };

		// Prep: Initialize a SdlRenderer and load an image texture
		// using the SDL_Renderer* ptr inside
		test_renderer.Init(settings);
		test_renderer.ClearScreen();
		image_surf_ptr = IMG_Load("data/tests/test-skull.png");
		REQUIRE(image_surf_ptr != nullptr);

		REQUIRE(renderer_info.state.sdl_renderer_ptr != nullptr);
		img_texture_ptr = SDL_CreateTextureFromSurface(
		    renderer_info.state.sdl_renderer_ptr, image_surf_ptr);
		REQUIRE(img_texture_ptr != nullptr);

		// C1. Renderer becomes invalid before blitting, throws error
		auto temporary_render_storage =
		    std::move(this->renderer_info.state.sdl_renderer_ptr);
		REQUIRE_THROWS([&]() {
			// set an invalid SDL_Renderer pointer
			renderer_info.state.sdl_renderer_ptr = nullptr;
			// 1. Before initialization, throws error
			test_renderer.Blit(img_texture_ptr, location);
		}());

		// C2. With a valid, initialized SdlRenderer, the method works
		renderer_info.state.sdl_renderer_ptr.swap(
		    temporary_render_storage);
		test_renderer.Blit(img_texture_ptr, location);

		// Display the image and pause so the user can see it!
		test_renderer.Flip();

		//  Pump events on macOS to mmake the window appear
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
// vim: set foldmethod=syntax textwidth=80 ts=8 sts=0 sw=8  noexpandtab ft=cpp.doxygen :
