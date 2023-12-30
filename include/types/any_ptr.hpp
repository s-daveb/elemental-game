/* any-ptr.cpp
 * Copyright © 2023 Saul D. Beniquez
 * License: Mozilla Public License v. 2.0
 *
 * This Source Code Form is subject to the terms of the Mozilla Public License,
 * v.2.0. If a copy of the MPL was not distributed with this file, You can
 * obtain one at https://mozilla.org/MPL/2.0/.
 */

#pragma once

#include <functional>
#include <stdexcept>
#include <typeindex>

namespace elemental {
class any_ptr;

template<typename TP>
struct is_pointer_type : std::false_type
{};

template<typename TP>
struct is_pointer_type<TP*> : std::true_type
{};

class any_ptr
{
  public:
	any_ptr() : data(nullptr), saved_type_index(typeid(void*)) {}

	template<typename TP,
	         typename = std::enable_if_t<is_pointer_type<TP>::value>>
	any_ptr(TP ptr)
	    : data(ptr)
	    , saved_type_index(typeid(std::remove_pointer_t<TP>*))
	{
	}

	any_ptr(const any_ptr& other)
	    : saved_type_index(other.saved_type_index)
	    , data(other.data)
	{
	}

	any_ptr& operator=(const any_ptr& other)
	{
		if (this != &other) {
			saved_type_index = other.saved_type_index;
			data = other.data;
		}
		return *this;
	}

	template<typename T,
	         typename = std::enable_if_t<is_pointer_type<T>::value>>
	any_ptr& operator=(T ptr)
	{
		saved_type_index = typeid(std::remove_pointer_t<T>*);
		data = ptr;
		return *this;
	}

	~any_ptr()
	{
		// No deletion here; it's assumed to be handled externally
	}

	const inline std::type_index type() const { return saved_type_index; }

	template<typename TP>
	friend TP any_ptr_cast(const any_ptr& ptr);

  private:
	void* data;
	std::type_index saved_type_index;
};

template<typename TP>
TP
any_ptr_cast(const any_ptr& ptr)
{
	return static_cast<TP>(ptr.data);
};
}
