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

struct Platform
{
#ifdef __linux__
	static constexpr bool LINUX = true;
#else
	static constexpr bool LINUX = false;
#endif

#ifdef _WIN32
	static constexpr bool WINDOWS = true;
#else
	static constexpr bool WINDOWS = false;
#endif

#ifdef __APPLE__
	static constexpr bool MACOS = true;
#else
	static constexpr bool MACOS = false;
#endif

#ifdef __FreeBSD__
	static constexpr bool FREEBSD = true;
#else
	static constexpr bool FREEBSD = false;
#endif

	// I don't own any AIX, Solaris, HP-UX, or pure Darwin systems, sorry :)
	// To be added at a later date.
};
} // namespace elemental

// clang-format off
// vim: set foldmethod=syntax foldlevel=99 textwidth=80 ts=8 sts=0 sw=8  noexpandtab ft=cpp.doxygen :
