/* types/configuration.hpp
 * Copyright © 2024 Saul D. Beniquez
 * License: Mozilla Public License v. 2.0
 *
 * This Source Code Form is subject to the terms of the Mozilla Public License,
 * v.2.0. If a copy of the MPL was not distributed with this file, You can
 * obtain one at https://mozilla.org/MPL/2.0/.
 */

#pragma once

#include "types/containers.hpp"

#include <string>

namespace elemental::configuration {

using dictionary = elemental::dictionary<std::string>;

namespace json {
	enum IndentMode
	{
		COMPACT = -1,
		NEWLINES = 0,
		INDENT = 1,
	};
	enum AsciiMode
	{
		DEFAULT = false,
		FORCE = true,
	};
} // namespace json
} // namespace elemental::configuration

// clang-format off
// vim: set foldmethod=syntax textwidth=80 ts=8 sts=0 sw=8 foldlevel=99 noexpandtab ft=cpp.doxygen :
