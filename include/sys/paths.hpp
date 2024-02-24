/* paths.hpp
 * Copyright © 2024 Saul D. Beniquez
 * License: Mozilla Public License v. 2.0
 *
 * This Source Code Form is subject to the terms of the Mozilla Public License,
 * v.2.0. If a copy of the MPL was not distributed with this file, You can
 * obtain one at https://mozilla.org/MPL/2.0/.
 */

#pragma once

#include "Exception.hpp"
#include "sys/platform.hpp"

#include <filesystem>

namespace elemental::paths {
std::filesystem::path GetHome();
std::filesystem::path GetAppConfigRoot();

std::filesystem::path ExpandPath(const std::filesystem::path&);
} // namespace elemental::paths

// clang-format off
// vim: set foldmethod=syntax textwidth=80 ts=8 sts=0 sw=8  noexpandtab ft=cpp.doxygen :
