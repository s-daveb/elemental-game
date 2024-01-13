/* aliases.hpp
 * Copyright © 2023-2024 Saul D. Beniquez
 * License: Mozilla Public License v. 2.0
 *
 * This Source Code Form is subject to the terms of the Mozilla Public License,
 * v.2.0. If a copy of the MPL was not distributed with this file, You can
 * obtain one at https://mozilla.org/MPL/2.0/.
 */

#pragma once

#include <cstdint>
#include <string>

namespace elemental {

/**! \name Aliases for C-types that are unclear.
 **! The C type char* does not immediately scream "STRING", and
 **! socket libraries return `int`, rather than a typedef socket_fd_t. */
///! @{
namespace c {
using string = char*;
using const_string = const char*;
using count_t = size_t;

namespace fd {
using socket = int;
} // namespace fd
} // namespace c
enum error_flag : bool
{
	NO_ERROR = false,
	ERROR = true
};
///! @}

///! \brief Convenience for std::map objects where the key is always a string.
template<typename value_t>
using Dictionary = std::unordered_map<std::string, value_t>;
} // namespace elemental

// clang-format off
// vim: set foldmethod=syntax textwidth=80 ts=8 sts=0 sw=8 foldlevel=99 noexpandtab ft=cpp.doxygen :
