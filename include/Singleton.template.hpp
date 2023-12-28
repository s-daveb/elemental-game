/* Singleton.template.hpp
 * Copyright © 2023 Saul D. Beniquez
 * License: Mozilla Public License v. 2.0
 *
 * This Source Code Form is subject to the terms of the Mozilla Public License,
 * v.2.0. If a copy of the MPL was not distributed with this file, You can
 * obtain one at https://mozilla.org/MPL/2.0/.
 */

#pragma once

#include <utility>

//! \brief Inserts a GetInstance<T>() template method into your class
//! class so that child classes can be easily made into singletons.
//! typename T must be a child class of whatever class into which you add this
//! macro.
//!\note
//! Make sure to make your child class's Constructor "protected", and declare
//! the parent class with this SINLGETON_INTERFACE as a friend class!
#define SINGLETON_INTERFACE(TypeName)                                          \
	template<typename T>                                                   \
	static T* GetInstance()                                                \
	{                                                                      \
		static_assert(std::is_base_of<TypeName, T>::value,             \
		              "T must be a derivative of " #TypeName);         \
		static T instance;                                             \
		return &instance;                                              \
	};

namespace elemental {
template<typename T>
struct Singleton
{
  public:
	static T& GetInstance()
	{
		static T instance;
		return instance;
	}
};

} // namespace elemental

// clang-format off
// vim: set foldmethod=syntax textwidth=80 ts=8 sts=0 sw=8 foldlevel=99 noexpandtab ft=cpp.doxygen :
