/* bitwise.hpp
 * Copyright © 2024 Saul D. Beniquez
 * License:  Mozilla Public License v. 2.0
 *
 * This Source Code Form is subject to the terms of the Mozilla Public License,
 * v.2.0. If a copy of the MPL was not distributed with this file, You can
 * obtain one at https://mozilla.org/MPL/2.0/.jj
 */

#pragma once

#include <type_traits>


struct Enum final
{
	template<typename TEnum>
	using enable_if_enum_t = std::enable_if_t<std::is_enum_v<TEnum>, bool>;

	template<typename TEnum>
	static constexpr enable_if_enum_t<TEnum> ContainsFlag(
	    const TEnum& flag_set, const TEnum& flag_to_check)
	{
		return static_cast<std::underlying_type_t<TEnum>>(flag_set) &
		       static_cast<std::underlying_type_t<TEnum>>(
			   flag_to_check);
	}

	template<typename TEnum>
	static constexpr enable_if_enum_t<TEnum> SetFlag(
	    TEnum& flag_set, const TEnum& flag_to_set)
	{
		flag_set = static_cast<TEnum>(
		    static_cast<std::underlying_type_t<TEnum>>(flag_set) |
		    static_cast<std::underlying_type_t<TEnum>>(flag_to_set));
		return flag_set; // Returning the modified flag_set
	}
};

// clang-format off
// vim: set foldmethod=syntax foldminlines=10 textwidth=80 ts=8 sts=0 sw=8 noexpandtab ft=cpp.doxygen :
