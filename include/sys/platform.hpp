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
		kUNKNOWN = 0b0000,
		kWINDOWS = 0b0001,
		kUNIX = 0b0100,
		kLINUX = 0b0101,
		kFREEBSD = 0b0110,
		kMACOSX = 0b0111
	};

#ifdef __linux__
	static const platform_t kCURRENT_PLATFORM = kLINUX;
#endif

#ifdef _WIN32
	static const platform_t kCURRENT_PLATFORM = kWINDOWS;
#endif

#ifdef __APPLE__
	static const platform_t kCURRENT_PLATFORM = kMACOSX;
#endif

#ifdef __FreeBSD__
	static const platform_t kCURRENT_PLATFORM = kFREEBSD;
#endif

	// I don't own any AIX, Solaris, HP-UX, or pure Darwin systems, sorry :)
	// To be added at a later date.
}; // namespace platform
} // namespace elemental

// clang-format off
// vim: set foldmethod=syntax  textwidth=80 ts=8 sts=0 sw=8  noexpandtab ft=cpp.doxygen :
