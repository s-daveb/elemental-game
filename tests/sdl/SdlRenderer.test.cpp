/* SdlRenderer.test.cpp
 * Copyright © 2023 Saul D. Beniquez
 * License: Mozilla Public License v. 2.0
 *
 * This Source Code Form is subject to the terms of the Mozilla Public License,
 * v.2.0. If a copy of the MPL was not distributed with this file, You can
 * obtain one at https://mozilla.org/MPL/2.0/.
 */

#include "IRenderer.hpp"
#include "renderers/SdlRenderer.hpp"
#include "sys/platform.hpp"

#include "common.hpp"
#include "utils/SdlHelpers.hpp"

#include <SDL.h>
#include <SDL_image.h>

#include <chrono>
#include <thread>

BEGIN_TEST_SUITE("elemental::SdlRenderer")
{
	using namespace elemental;

	struct SdlRendererFixture : public SdlTestFixture
	{
		SdlRendererFixture()
		    : SdlTestFixture()
		    , test_subject(IRenderer::GetInstance<SdlRenderer>())
		{
		}

		~SdlRendererFixture() { test_subject->Deactivate(); }
		SdlRenderer* test_subject;
	};

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
			SDL_Rect result =
			    renderer->FromRectangle<SDL_Rect>(test_input);

			CHECK(result.x == test_input.x);
			CHECK(result.y == test_input.y);
			CHECK(result.w == test_input.width);
			CHECK(result.h == test_input.height);
		}());
	}
	TEST("elemental::SdlRenderer - convert SDL_Rect to Rectangle")
	{
		REQUIRE_NOTHROW([&]() {
			auto* renderer = IRenderer::GetInstance<SdlRenderer>();
			auto test_input = SDL_Rect{ 0, 1, 10, 20 };
			Rectangle result =
			    renderer->ToRectangle<SDL_Rect>(test_input);

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
		CHECK(false == test_subject->is_initialized);
		test_subject->Init();
		CHECK(true == test_subject->is_initialized);

		// 2. Verify that an SDL_Window and SDL_Renderer have been
		// created
		REQUIRE(test_subject->sdl_window_ptr != nullptr);
		REQUIRE(test_subject->sdl_renderer_ptr != nullptr);
	}
	TEST_CASE_METHOD(SdlRendererFixture,
	                 "elemental::SdlRenderer - Deactivate Renderer")
	{
		// 1. Deactivate method de-initializes
		test_subject->Init();
		REQUIRE(true == test_subject->is_initialized);

		test_subject->Deactivate();
		REQUIRE(false == test_subject->is_initialized);

		// 2. Verify that an SDL_Window and SDL_Renderer ptrs have been
		// cleared
		REQUIRE(test_subject->sdl_window_ptr == nullptr);
		REQUIRE(test_subject->sdl_renderer_ptr == nullptr);
	}
	TEST_CASE_METHOD(SdlRendererFixture,
	                 "elemental::SdlRenderer - IsInitialized Accessor")
	{
		// 1. Default initialization does not throw errors
		CHECK(test_subject->IsInitialized() ==
		      test_subject->is_initialized);
		test_subject->Init();
		CHECK(test_subject->IsInitialized() ==
		      test_subject->is_initialized);
	}

	TEST_CASE_METHOD(SdlRendererFixture,
	                 "elemental::SdlRenderer - GetResolution works")
	{
		// 1. Before initialization, throws error
		REQUIRE_THROWS([this]() { test_subject->GetResolution(); }());

		// 2. After initializtion, the method works
		test_subject->Init();
		auto resolution_data = test_subject->GetResolution();

		REQUIRE(resolution_data.first > 0);
		REQUIRE(resolution_data.second > 0);
	}

	TEST_CASE_METHOD(SdlRendererFixture,
	                 "elemental::SdlRenderer - Clear works")
	{
		// 1. Before initialization, throws error
		REQUIRE_THROWS([this]() { test_subject->Clear(); }());

		// 2. After initializtion, the method works
		test_subject->Init();
		test_subject->Clear();
	}
	TEST_CASE_METHOD(SdlRendererFixture,
	                 "elemental::SdlRenderer - Flip works")
	{
		// 1. Before initialization, throws error
		REQUIRE_THROWS([this]() { test_subject->Flip(); }());

		// 2. After initializtion, the method works
		test_subject->Init();
		test_subject->Flip();
	}
	TEST_CASE_METHOD(SdlRendererFixture,
	                 "elemental::SdlRenderer - Blit works")
	{
		using std::chrono::seconds;
		SDL_Surface* image_surf_ptr = nullptr;
		SDL_Texture* img_texture_ptr = nullptr;
		Rectangle location{ 10, 10, 100, 100 };

		// Prep: Initialize a SdlRenderer and load an image texture
		// using the SDL_Renderer* ptr inside
		test_subject->Init();
		test_subject->Clear();
		image_surf_ptr = IMG_Load("data/test/test-skull.png");
		REQUIRE(image_surf_ptr != nullptr);

		REQUIRE(test_subject->sdl_renderer_ptr != nullptr);
		img_texture_ptr = SDL_CreateTextureFromSurface(
		    test_subject->sdl_renderer_ptr, image_surf_ptr);
		REQUIRE(img_texture_ptr != nullptr);

		// 1. Renderer becomes invalid before blitting!
		auto* temporary_render_storage =
		    this->test_subject->sdl_renderer_ptr;
		REQUIRE_THROWS([&]() {
			// set an invalid SDL_Renderer pointer
			test_subject->sdl_renderer_ptr = nullptr;
			// 1. Before initialization, throws error
			test_subject->Blit(img_texture_ptr, location);
		}());

		// 2. With a valid, initialized SdlRenderer, the
		//   method works
		test_subject->sdl_renderer_ptr = temporary_render_storage;

		test_subject->Blit(img_texture_ptr, location);
		// Display the image and pause so the user can see it!
		test_subject->Flip();
		if (platform::MACOS) {
			for (unsigned i = 0; i < 500; i++) {
				SDL_PumpEvents();
			}
		}
		std::this_thread::sleep_for(seconds(5));

		SDL_FreeSurface(image_surf_ptr);
	}
#endif
}

// clang-format off
// vim: set foldmethod=syntax textwidth=80 ts=8 sts=0 sw=8 foldlevel=99 noexpandtab ft=cpp.doxygen :
