/* SDL_Memory.test.cpp
 * Copyright © 2024 Saul D. Beniquez
 * License: Mozilla Public License v. 2.0
 *
 * This Source Code Form is subject to the terms of the Mozilla Public License,
 * v.2.0. If a copy of the MPL was not distributed with this file, You can
 * obtain one at https://mozilla.org/MPL/2.0/.
 */

#include "SDL_Memory.hpp"

#include "test-utils/common.hpp"
#include "util/debug.hpp"

#include "Exception.hpp"

#include <exception>
#include <iostream>
#include <optional>
#include <stdexcept>

BEGIN_TEST_SUITE("SDL_Memory.hpp tests")
{
	using namespace elemental;

	struct FakeSdlObject
	{
		FakeSdlObject(bool& external_object)
		    : initialized(external_object)
		{
		}
		bool& initialized;
	};

	struct FakeDeleter
	{
		void operator()(FakeSdlObject* ptr)
		{

			// DBG_PRINT("FakeDeleter::operator() called!");
			ptr->initialized = false;
			// DBG_PRINT(((ptr->initialized) ? " initialized"
			//                                : " not
			//                                initialized"));
			delete ptr;
			ptr = nullptr;
		}
	};

	using TestSdlUniquePtr = UniqueSdlPtr<FakeSdlObject, FakeDeleter>;
	using TestSdlSharedPtr = SdlPtr<FakeSdlObject, FakeDeleter>;

	TEST("UniqueSdlPtr is convertible to and from raw pointers")
	{
		bool is_initialized = true;
		auto* fake_object = new FakeSdlObject(is_initialized);
		{
			TestSdlUniquePtr unique_ptr;
			unique_ptr = fake_object;

			REQUIRE(unique_ptr.get() == fake_object);
			REQUIRE(unique_ptr == fake_object);
			REQUIRE(static_cast<FakeSdlObject*>(unique_ptr) ==
			        fake_object);
		}
		REQUIRE(is_initialized == false);
	}

	TEST("SdlPtr is convertible to and from raw pointers")
	{
		bool is_initialized = true;
		auto* fake_object = new FakeSdlObject(is_initialized);
		{
			TestSdlSharedPtr shared_ptr;
			shared_ptr = fake_object;

			REQUIRE(shared_ptr.get() == fake_object);
			REQUIRE(shared_ptr == fake_object);
			REQUIRE(static_cast<FakeSdlObject*>(shared_ptr) ==
			        fake_object);
		}
		REQUIRE(is_initialized == false);
	}
}
// clang-format off
// vim: set foldmethod=syntax textwidth=80 ts=8 sts=0 sw=8  noexpandtab ft=cpp.doxygen :
