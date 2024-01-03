/* any_ptr.cpp
 * Copyright © 2023 Saul D. Beniquez
 * License: Mozilla Public License v. 2.0
 *
 * This Source Code Form is subject to the terms of the Mozilla Public License,
 * v.2.0. If a copy of the MPL was not distributed with this file, You can
 * obtain one at https://mozilla.org/MPL/2.0/.
 */

#include "any_ptr.thpp"
#include "test-utils/common.hpp"

#include "Exception.hpp"

#include <exception>
#include <iostream>
#include <optional>
#include <stdexcept>

using namespace elemental;
namespace ns1 {
struct dummy_type
{
	int num;
};
}
namespace ns2 {
struct dummy_type
{
	char* byte_array;
};
}
BEGIN_TEST_SUITE("elemental::any_ptr")
{
	TEST("elemental::any_ptr - default_constructor")
	{
		any_ptr ptr;
		REQUIRE(ptr.saved_type_index == typeid(void*));
		REQUIRE(ptr.stored_raw_ptr == nullptr);
	}

	TEST("elemental::any_ptr - constructor with nullptr")
	{
		any_ptr ptr(nullptr);
		REQUIRE(ptr.saved_type_index == typeid(void*));
		REQUIRE(ptr.stored_raw_ptr == nullptr);
	}

	TEST("elemental::any_ptr - constructor with null intptr")
	{
		int* int_ptr = nullptr;
		any_ptr ptr(int_ptr);
		REQUIRE(ptr.saved_type_index == typeid(int*));
		REQUIRE(ptr.stored_raw_ptr == nullptr);
	}

	TEST("elemental::any_ptr - constructor_with_pointer")
	{
		int int_value = 42;
		any_ptr ptr(&int_value);
		REQUIRE(ptr.saved_type_index == typeid(int*));
		REQUIRE(ptr.stored_raw_ptr == &int_value);
	}
	TEST("elemental::any_ptr - constructor_with_VOID_pointer")
	{
		int int_value = 42;
		void* raw_void_ptr = &int_value;

		any_ptr ptr(raw_void_ptr);
		REQUIRE(ptr.saved_type_index == typeid(void*));
		REQUIRE(any_ptr_cast<int*>(ptr) == &int_value);

		REQUIRE_NOTHROW([&]() { any_ptr_cast<double*>(ptr); }());
	}
	TEST("elemental::any_ptr - copy_constructor")
	{
		int int_value = 42;
		any_ptr original_ptr(&int_value);
		any_ptr copied_ptr(original_ptr);

		REQUIRE(original_ptr.saved_type_index ==
		        copied_ptr.saved_type_index);
		REQUIRE(original_ptr.stored_raw_ptr ==
		        copied_ptr.stored_raw_ptr);
	}

	TEST("elemental::any_ptr - assignment_operator")
	{
		int int_value = 42;
		double double_value = 3.14;

		any_ptr ptr1(&int_value);
		any_ptr ptr2;

		ptr2 = &double_value;

		REQUIRE(ptr1.saved_type_index == typeid(int*));
		REQUIRE(ptr1.stored_raw_ptr == &int_value);

		REQUIRE(ptr2.saved_type_index == typeid(double*));
		REQUIRE(ptr2.stored_raw_ptr == &double_value);
	}
	TEST("elemental::any_ptr - assignment_operator reassignment")
	{
		int int_value = 42;
		double double_value = 3.14;

		any_ptr ptr1(&int_value);

		REQUIRE(ptr1.saved_type_index == typeid(int*));
		REQUIRE(ptr1.stored_raw_ptr == &int_value);

		ptr1 = &double_value;

		REQUIRE(ptr1.saved_type_index == typeid(double*));
		REQUIRE(ptr1.stored_raw_ptr == &double_value);
	}

	TEST("elemental::any_ptr::type() - returns saved_type_index")
	{
		any_ptr ptr;
		REQUIRE(&(ptr.type()) == &ptr.saved_type_index);
	}
	TEST("elemental::any_ptr::has_value")
	{
		int int_value = 42;
		any_ptr test_any_ptr(&int_value);
		any_ptr null_any_ptr(nullptr);

		REQUIRE_FALSE(null_any_ptr.has_value());
		REQUIRE(test_any_ptr.has_value());
	}
	TEST("elemental::any_ptr::operator_bool")
	{
		int int_value = 42;
		any_ptr test_any_ptr(&int_value);
		any_ptr null_any_ptr(nullptr);

		REQUIRE_FALSE(null_any_ptr);
		REQUIRE(test_any_ptr);
	}

	TEST("elemental::any_ptr_cast to void always works")
	{
		int int_value = 42;
		short short_value = 24;
		double dbl_value = 42.42;
		void* raw_void_ptr = &short_value;

		any_ptr empty_ptr;
		any_ptr int_ptr = &int_value;
		any_ptr void_ptr = raw_void_ptr;

		REQUIRE_NOTHROW([&]() {
			REQUIRE(any_ptr_cast<short*>(void_ptr) != nullptr);
			REQUIRE(any_ptr_cast<short*>(void_ptr) == raw_void_ptr);
		}());
	}

	TEST("elemental::any_ptr_cast with matching type")
	{
		int int_value = 42;
		any_ptr ptr(&int_value);

		int* casted_ptr = any_ptr_cast<int*>(ptr);

		REQUIRE_NOTHROW([&]() {
			REQUIRE(casted_ptr != nullptr);
			REQUIRE(*casted_ptr == int_value);
			REQUIRE(casted_ptr == &int_value);
		}());
	}

	TEST("elemental::any_ptr_cast with nullptr and void*")
	{
		any_ptr void_ptr(nullptr);
		any_ptr nullptr_ptr(nullptr);

		void* casted_void_ptr = any_ptr_cast<void*>(void_ptr);
		int* casted_nullptr_ptr = any_ptr_cast<int*>(nullptr_ptr);

		REQUIRE_NOTHROW(
		    [&]() { REQUIRE(casted_void_ptr == nullptr); }());
		REQUIRE(casted_nullptr_ptr == nullptr);
	}

	TEST("elemental::any_ptr_cast with mismatched type")
	{
		int int_value = 42;
		any_ptr ptr(&int_value);

		// Attempting to cast to a different type should throw bad_cast
		REQUIRE_THROWS_AS(any_ptr_cast<double*>(ptr),
		                  any_ptr::bad_cast);
	}

	TEST("elemental::any_ptr_cast with matching struct types")
	{
		ns1::dummy_type obj;
		any_ptr dummy_ptr(&obj);

		// Since the type name hash matches, casting should succeed
		REQUIRE(dummy_ptr.stored_raw_ptr != nullptr);
		REQUIRE(dummy_ptr.stored_raw_ptr == &obj);
		REQUIRE(dummy_ptr.saved_type_index == typeid(&obj));

		auto result = any_ptr_cast<ns1::dummy_type*>(dummy_ptr);
		REQUIRE(result == &obj);
		REQUIRE(result->num == obj.num);
	}

	TEST("elemental::any_ptr_cast with mismatched struct types")
	{
		ns1::dummy_type obj;
		any_ptr dummy_ptr(&obj);

		// Since the type name hash matches, casting should succeed
		REQUIRE(dummy_ptr.stored_raw_ptr != nullptr);
		REQUIRE(dummy_ptr.stored_raw_ptr == &obj);
		REQUIRE(dummy_ptr.saved_type_index == typeid(&obj));

		REQUIRE_THROWS_AS(
		    [&]() { any_ptr_cast<ns2::dummy_type*>(dummy_ptr); }(),
		    any_ptr::bad_cast);
	}

	TEST("elemental::any_ptr_cast with mismatched type_info but matching "
	     "type_info::name hashes")
	{
		ns1::dummy_type obj;
		any_ptr dummy_ptr(&obj);
		ns1::dummy_type* result = nullptr;

		// Deliberately ruin the saved_type_index structure
		// so that any_cast will have to match by typename hashes
		dummy_ptr.saved_type_index = typeid(double);

		// This shouldn't throw because the type_index.name hashes
		// were not changed.
		REQUIRE_NOTHROW([&]() {
			result = any_ptr_cast<ns1::dummy_type*>(dummy_ptr);
		}());

		REQUIRE(result != nullptr);
		REQUIRE(result == &obj);
		REQUIRE(typeid(result) == typeid(&obj));
	}
} // namespace elemental
  // clang-format off
// vim: set foldmethod=syntax textwidth=80 ts=8 sts=0 sw=8 foldlevel=99 noexpandtab ft=cpp.doxygen :
