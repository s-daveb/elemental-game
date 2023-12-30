/* any_ptr.cpp
 * Copyright © 2023 Saul D. Beniquez
 * License: Mozilla Public License v. 2.0
 *
 * This Source Code Form is subject to the terms of the Mozilla Public License,
 * v.2.0. If a copy of the MPL was not distributed with this file, You can
 * obtain one at https://mozilla.org/MPL/2.0/.
 */

#include "test-utils/common.hpp"
#include "types/any_ptr.hpp"

#include <exception>
#include <iostream>
#include <optional>
#include <stdexcept>

using namespace elemental;

BEGIN_TEST_SUITE("elemental::any_ptr")
{
	TEST("elemental::any_ptr - default_constructor")
	{
		any_ptr ptr;
		REQUIRE(ptr.type() == typeid(void*));
	}

	TEST("elemental::any_ptr - constructor_with_pointer")
	{
		int int_value = 42;
		any_ptr ptr(&int_value);
		REQUIRE(ptr.type() == typeid(int*));
		REQUIRE(any_ptr_cast<int*>(ptr) == &int_value);
	}

	TEST("elemental::any_ptr - copy_constructor")
	{
		int int_value = 42;
		any_ptr original_ptr(&int_value);
		any_ptr copied_ptr(original_ptr);

		REQUIRE(original_ptr.type() == copied_ptr.type());
		REQUIRE(any_ptr_cast<int*>(original_ptr) ==
		        any_ptr_cast<int*>(copied_ptr));
	}

	TEST("elemental::any_ptr - assignment_operator")
	{
		int int_value = 42;
		double double_value = 3.14;

		any_ptr ptr1(&int_value);
		any_ptr ptr2;

		ptr2 = &double_value;

		REQUIRE(ptr1.type() == typeid(int*));
		REQUIRE(any_ptr_cast<int*>(ptr1) == &int_value);

		REQUIRE(ptr2.type() == typeid(double*));
		REQUIRE(any_ptr_cast<double*>(ptr2) == &double_value);
	}

	TEST("elemental::any_ptr - any_ptr_cast_function")
	{
		int int_value = 42;
		double double_value = 3.14;

		any_ptr ptr1(&int_value);
		any_ptr ptr2(&double_value);

		int* casted_ptr1 = any_ptr_cast<int*>(ptr1);
		double* casted_ptr2 = any_ptr_cast<double*>(ptr2);

		REQUIRE(*casted_ptr1 == int_value);
		REQUIRE(*casted_ptr2 == double_value);
	}

} // namespace
