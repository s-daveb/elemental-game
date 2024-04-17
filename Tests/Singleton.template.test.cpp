/* Singleton.template.test.cpp
 * Copyright © 2023 Saul D. Beniquez
 * License: Mozilla Public License v. 2.0
 *
 * This Source Code Form is subject to the terms of the Mozilla Public License,
 * v.2.0. If a copy of the MPL was not distributed with this file, You can
 * obtain one at https://mozilla.org/MPL/2.0/.
 */

#include "Singleton.hpp"

#include "test-utils/common.hpp"

#include <exception>
#include <iostream>
#include <optional>
#include <stdexcept>

using elemental::Singleton;

BEGIN_TEST_SUITE("elemental::Singleton<T>")
{
	struct SimpleStructure
	{
		uint32_t id;
		char name[128];
		uint32_t flags;
	};

	struct NonConstructibleStructure
	{
		uint32_t id;
		char name[128];

	  private:
		NonConstructibleStructure(){};
		friend class Singleton;
	};

	TEST("elemental::Singleton<T> - Can wrap a simple class")
	{
		SimpleStructure stack_obj;
		auto& first_reference =
		    Singleton::getReference<SimpleStructure>();

		SECTION("1. No exceptions were thrown")
		{
			SUCCEED();
		}

		SECTION("2. Subsequent calls result in the same object being "
		        "returned")
		{
			auto& second_reference =
			    Singleton::getReference<SimpleStructure>();
			CHECK(&stack_obj != &first_reference);
			CHECK(&first_reference == &second_reference);
		}
	}

	TEST("elemental::Singleton<T> - As a friend class, can call a "
	     "private constructor")
	{
		auto& first_reference =
		    Singleton::getReference<SimpleStructure>();

		SECTION("1. No exceptions were thrown")
		{
			SUCCEED();
		}

		SECTION("2. Subsequent calls result in the same object being "
		        "returned")
		{
			auto& second_reference =
			    Singleton::getReference<SimpleStructure>();
			CHECK(&first_reference == &second_reference);
		}
	}
}

// clang-format off
// vim: set foldmethod=syntax  textwidth=80 ts=4 sts=0 sw=4  noexpandtab ft=cpp.doxygen :
