/* Singleton.template.test.cpp
 * Copyright © 2023 Saul D. Beniquez
 * License: Mozilla Public License v. 2.0
 *
 * This Source Code Form is subject to the terms of the Mozilla Public License,
 * v.2.0. If a copy of the MPL was not distributed with this file, You can
 * obtain one at https://mozilla.org/MPL/2.0/.
 */

#include "Singleton.template.hpp"

#include "./common.hpp"

#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers_string.hpp>

#include <exception>
#include <iostream>
#include <optional>
#include <stdexcept>

using elemental::Singleton;

BEGIN_TEST_SUITE("elemental::Singleton<T>")
{
	struct simple_structure
	{
		uint32_t id;
		char name[128];
		uint32_t flags;
	};

	struct non_constructible_structure
	{
		uint32_t id;
		char name[128];

	  private:
		non_constructible_structure(){};
		friend class Singleton<non_constructible_structure>;
	};

	TEST("elemental::Singleton<T> -  Can wrap a simple class")
	{
		simple_structure stack_obj;
		auto& first_reference =
		    elemental::Singleton<simple_structure>::GetInstance();

		SECTION("1. No exceptions were thrown")
		{
			SUCCEED();
		}

		SECTION("2. Subsequent calls result in the same object being "
		        "returned")
		{
			auto& second_reference = elemental::Singleton<
			    simple_structure>::GetInstance();
			CHECK(&stack_obj != &first_reference);
			CHECK(&first_reference == &second_reference);
		}
	}

	TEST("elemental::Singleton<T> - As a friend class, can call a "
	     "private constructor")
	{
		auto& first_reference = elemental::Singleton<
		    non_constructible_structure>::GetInstance();

		SECTION("1. No exceptions were thrown")
		{
			SUCCEED();
		}

		SECTION("2. Subsequent calls result in the same object being "
		        "returned")
		{
			auto& second_reference = elemental::Singleton<
			    non_constructible_structure>::GetInstance();
			CHECK(&first_reference == &second_reference);
		}
	}
}

// clang-format off
// vim: set foldmethod=syntax foldlevel=99 textwidth=80 ts=4 sts=0 sw=4  noexpandtab ft=cpp.doxygen :
