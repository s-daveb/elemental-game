/* platform.hpp
 * Copyright © 2023 Saul D. Beniquez
 * License: Mozilla Public License v. 2.0
 *
 * This Source Code Form is subject to the terms of the Mozilla Public License,
 * v.2.0. If a copy of the MPL was not distributed with this file, You can
 * obtain one at https://mozilla.org/MPL/2.0/.
 */
#pragma once

namespace elemental {

namespace platform {

enum platform_t
{
	UNKNOWN = 0b0000,
	WINDOWS = 0b0001,
	UNIX = 0b0100,
	LINUX = 0b0101,
	FREEBSD = 0b0110,
	MACOSX = 0b0111
};

#ifdef __linux__
static platform_t current_platform = LINUX;
#endif

#ifdef _WIN32
static platform_t current_platform = WINDOWS;
#endif

#ifdef __APPLE__
static platform_t current_platform = MACOSX;
#endif

#ifdef __FreeBSD__
static platform_t current_platform = FREEBSD;
#endif

// I don't own any AIX, Solaris, HP-UX, or pure Darwin systems, sorry :)
// To be added at a later date.
};
} // namespace elemental

// clang-format off
// vim: set foldmethod=syntax  textwidth=80 ts=8 sts=0 sw=8  noexpandtab ft=cpp.doxygen :
