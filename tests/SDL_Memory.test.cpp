/* SDL_Memory.test.cpp
 * Copyright © 2024 Saul D. Beniquez
 * License: Mozilla Public License v. 2.0
 *
 * This Source Code Form is subject to the terms of the Mozilla Public License,
 * v.2.0. If a copy of the MPL was not distributed with this file, You can
 * obtain one at https://mozilla.org/MPL/2.0/.
 */

#include "SDL_Memory.thpp"
#include "fakeit.hpp"
#include "test-utils/common.hpp"

#include "Exception.hpp"

#include <exception>
#include <iostream>
#include <optional>
#include <stdexcept>

BEGIN_TEST_SUITE("SDL_Memory.thpp tests")
{
	using namespace elemental;

	struct FakeSDL_Object
	{
		FakeSDL_Object(bool& external_object)
		    : initialized(external_object)
		{
		}
		bool& initialized;
	};

	struct FakeDeleter
	{
		void operator()(FakeSDL_Object* ptr)
		{

			debugprint("FakeDeleter::operator() called!");
			ptr->initialized = false;
			debugprint(((ptr->initialized) ? " initialized"
			                               : " not initialized"));
			delete ptr;
			ptr = nullptr;
		}
	};

	using test_sdl_unique_ptr = unique_sdl_ptr<FakeSDL_Object, FakeDeleter>;
	using test_sdl_shared_ptr = shared_sdl_ptr<FakeSDL_Object, FakeDeleter>;

	TEST("uniqe_sdl_ptr is convertible to and from raw pointers")
	{
		bool is_initialized = true;
		FakeSDL_Object* fake_object =
		    new FakeSDL_Object(is_initialized);
		{
			test_sdl_unique_ptr uniquePtr;
			uniquePtr = fake_object;

			REQUIRE(uniquePtr.get() == fake_object);
			REQUIRE(uniquePtr == fake_object);
			REQUIRE(static_cast<FakeSDL_Object*>(uniquePtr) ==
			        fake_object);
		}
		REQUIRE(is_initialized == false);
	}

	TEST("shared_sdl_ptr is convertible to and from raw pointers")
	{
		bool is_initialized = true;
		FakeSDL_Object* fake_object =
		    new FakeSDL_Object(is_initialized);
		{
			test_sdl_shared_ptr sharedPtr;
			sharedPtr = fake_object;

			REQUIRE(sharedPtr.get() == fake_object);
			REQUIRE(sharedPtr == fake_object);
			REQUIRE(static_cast<FakeSDL_Object*>(sharedPtr) ==
			        fake_object);
		}
		REQUIRE(is_initialized == false);
	}
}
// clang-format off
// vim: set foldmethod=syntax textwidth=80 ts=8 sts=0 sw=8 foldlevel=99 noexpandtab ft=cpp.doxygen :
