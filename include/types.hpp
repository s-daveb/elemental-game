/* types.hpp
 * Copyright © 2023 Saul D. Beniquez
 * License: Mozilla Public License v. 2.0
 *
 * This Source Code Form is subject to the terms of the Mozilla Public License,
 * v.2.0. If a copy of the MPL was not distributed with this file, You can
 * obtain one at https://mozilla.org/MPL/2.0/.
 */

#pragma once

#include <filesystem>
#include <functional>
#include <map>
#include <memory>
#include <optional>
#include <string>
#include <utility>

namespace elemental {

/* Aliases for C-types that are unclear.
 * example: the type char* does not immediately scream "STRING", and
 * 	    socket libraries return `int`, rather than a typedef socket_fd_t. */
namespace c {
using string = char*;
using const_string = const char*;
using count_t = size_t;

namespace fd {
using socket = int;
} // namespace fd
} // namespace c

enum error_t : bool
{
	NO_ERROR = false,
	ERROR = true
};

template<typename value_t>
using Dictionary = std::map<const std::string, value_t>;

template<typename T>
using ptr = std::shared_ptr<T>;

} // namespace elemental

// clang-format off
// vim: set foldmethod=syntax foldlevel=99 textwidth=80 ts=8 sts=0 sw=8  noexpandtab ft=cpp.doxygen :
