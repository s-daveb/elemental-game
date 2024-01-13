/* Configuration.hpp
 * Copyright © 2024 Saul D. Beniquez
 * License: Mozilla Public License v. 2.0
 *
 * This Source Code Form is subject to the terms of the Mozilla Public License,
 * v.2.0. If a copy of the MPL was not distributed with this file, You can
 * obtain one at https://mozilla.org/MPL/2.0/.
 */

#pragma once

#include <filesystem>
#include <functional>
#include <string>

#include "type-aliases.hpp"

namespace elemental {
namespace configuration {

using dictionary = elemental::dictionary<std::string>;

dictionary load(const std::filesystem::path&);
void save(dictionary&, const std::filesystem::path&);
}

} // namespace elemental::configuration

// clang-format off
// vim: set foldmethod=syntax textwidth=80 ts=8 sts=0 sw=8 foldlevel=99 noexpandtab ft=cpp.doxygen :
