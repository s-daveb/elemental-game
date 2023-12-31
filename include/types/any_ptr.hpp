/* any_ptr.cpp
 * Copyright © 2023 Saul D. Beniquez
 * License: Mozilla Public License v. 2.0
 *
 * This Source Code Form is subject to the terms of the Mozilla Public License,
 * v.2.0. If a copy of the MPL was not distributed with this file, You can
 * obtain one at https://mozilla.org/MPL/2.0/.
 */

#pragma once

#include "Exception.hpp"

#include <functional>
#include <iostream>
#include <stdexcept>
#include <string>
#include <typeindex>

namespace elemental {
class any_ptr;

template<typename TP>
struct is_pointer_type : std::false_type
{};

template<typename TP>
struct is_pointer_type<TP*> : std::true_type
{};

namespace {

static std::type_index void_ptr_type_index(typeid(void*));
static std::type_index nullptr_t_type_index(typeid(nullptr_t));
static std::hash<std::string> string_hasher{};
}

/*! \brief A slightly type-checked wrapper around void* pointers, for parameter
 * passing only; does not imply ownership of data; like std::any but only for
 * pointers.
 *
 *  This was needed because pointers from to incomplete types originating from
 *  shared libraries were not being detected by std::any as being of the same
 *  type.  Apparently SDL_Surface* declared in libsdl.so or libsdl.dylib appears
 *  as a different type to application-level code!
 * \warning <b><i>void*</i></b> values and <i><b>nullptr_t literals</b></i> are
 * both stored as <i>void*</i>. <u>They are not type-checked when using
 * any_ptr_cast.</u>
 *
 * \sa any_ptr_cast
 *  */
class any_ptr
{

  public:
	any_ptr(void* raw_ptr = nullptr)
	    : stored_raw_ptr(raw_ptr)
	    , saved_type_index(typeid(void*))
	    , type_name_str()
	    , type_name_hashed(0)
	{
	}

	template<typename TP,
	         typename = std::enable_if_t<is_pointer_type<TP>::value>>
	any_ptr(TP raw_ptr)
	    : stored_raw_ptr(raw_ptr)
	    , saved_type_index(typeid(TP))
	    , type_name_str()
	    , type_name_hashed(0)
	{
		type_name_str = saved_type_index.name();
		type_name_hashed = string_hasher(type_name_str);
	}

	any_ptr(const any_ptr& other)
	    : saved_type_index(other.saved_type_index)
	    , stored_raw_ptr(other.stored_raw_ptr)
	    , type_name_str(other.type_name_str)
	    , type_name_hashed(other.type_name_hashed)
	{
	}

	any_ptr& operator=(const any_ptr& other)
	{
		if (this != &other) {
			saved_type_index = other.saved_type_index;
			stored_raw_ptr = other.stored_raw_ptr;
			type_name_str = saved_type_index.name();
			type_name_hashed = string_hasher(type_name_str);
		}
		return *this;
	}

	template<typename TP,
	         typename = std::enable_if_t<is_pointer_type<TP>::value>>
	any_ptr& operator=(TP raw_ptr)
	{
		saved_type_index = typeid(TP);
		stored_raw_ptr = raw_ptr;
		type_name_str = saved_type_index.name();
		type_name_hashed = string_hasher(type_name_str);

		return *this;
	}

	virtual ~any_ptr() {}

	inline const std::type_index& type() const { return saved_type_index; }
	//
	// Method to check if the any_ptr has a value
	bool has_value() const { return stored_raw_ptr != nullptr; }
	// Boolean conversion operator
	explicit operator bool() const { return this->has_value(); }

	///! \brief A custom exception class thown by any_stored_raw_ptrcast.
	///! \sa any_stored_raw_ptrcast
	struct bad_cast : public Exception
	{
		bad_cast() : Exception("any_ptr_cast<T> failed type check") {}
		virtual ~bad_cast() {}
	};

	template<typename TP>
	friend TP any_ptr_cast(const any_ptr& ptr);

#ifndef UNIT_TEST
  private:
#endif
	void* stored_raw_ptr;
	std::type_index saved_type_index;
	std::string type_name_str;
	std::size_t type_name_hashed;
};
/*! \brief A type-aware custom casting function for any_ptr.
 *
 * Checks its stored std::type_index object to make sure users don't perform
 * unsafe, incompatible casts to raw pointers. When the type_index equivalency
 * fails, it also checks that the type_index.name() fields might match and
 * assumes the best if they do.
 *
 * \warning Use caution if you are duplicating type names in different
 *  namespaces. Generated type names are implementation-specific.
 *  */
template<typename TP>
TP
any_ptr_cast(const any_ptr& ptr)
{
	static std::type_index result_type_index(typeid(TP));

	// If: the original pointer was a nullptr literal or a void* pointer
	// Or:  If the user is requesting the internal pointer as void*
	// Or: The pointer was originally void*
	// Then: skip all type checks and just give them the they requested.
	// Caveat usor.
	if (ptr.saved_type_index == nullptr_t_type_index ||
	    ptr.saved_type_index == void_ptr_type_index ||
	    result_type_index == void_ptr_type_index) {
		return static_cast<TP>(ptr.stored_raw_ptr);
	} else {
		// Otherwise, check the requested type against the
		// saved_type_index. Make sure they match.
		// If they don't match, get the stD::type_index.name()s
		// and hash those. If they match, we are good to go!
		// This *could* be unsafe or not portable. Here's hoping. 🤞
		if (result_type_index != ptr.saved_type_index) {
			auto result_type_name_hash =
			    string_hasher(result_type_index.name());

			if (result_type_name_hash != ptr.type_name_hashed) {
				throw any_ptr::bad_cast();
			}
		}
		return static_cast<TP>(ptr.stored_raw_ptr);
	}
}

} // namespace elemental

// clang-format off
// vim: set foldmethod=syntax textwidth=80 ts=8 sts=0 sw=8 foldlevel=99 noexpandtab ft=cpp.doxygen :
