/* paths.hpp
 * Copyright © 2024 Saul D. Beniquez
 * License: Mozilla Public License v. 2.0
 *
 * This Source Code Form is subject to the terms of the Mozilla Public License,
 * v.2.0. If a copy of the MPL was not distributed with this file, You can
 * obtain one at https://mozilla.org/MPL/2.0/.
 */

#include "types/legible_ctypes.hpp"

#include "Exception.hpp"
#include "sys/paths.hpp"
#include "sys/platform.hpp"

#include <cstdlib>

#ifndef __WIN32__
#include <unistd.h>
#endif

#include <algorithm>
#include <filesystem>

namespace fs = std::filesystem;
namespace elemental::paths {

auto
get_home() -> fs::path
{
	c::string result;
#if defined(__unix__) || (defined(__APPLE__) && defined(__MACH__))
	result = getenv("HOME");
#elif defined(__WIN32__)
	env_value = getenv("USERPROFILE");
#endif
	return result;
}

auto
get_app_config_root() -> fs::path
{
	fs::path result;

	switch (platform::kCURRENT_PLATFORM) {
		case platform::kMACOS:
			result = get_home() / "Library" / "Application Support";
			break;
		case platform::kUNIX:
		case platform::kLINUX:
		case platform::kFREEBSD:
			result = get_home() / ".config";
			break;
		case platform::kWINDOWS:
			result = getenv("APPDATA");
			break;
		default:
			throw NotImplementedException();
			break;
	}
	return result;
}

auto
expand_path(const fs::path& location) -> fs::path
{
	try {
		fs::path result(location.root_path());

		for (auto path_iter = location.begin();
		     path_iter != location.end(); ++path_iter) {
			std::string token(path_iter->string());
			c::string env_val;
#if defined(__unix__) || (defined(__APPLE__) && defined(__MACH__))
			if (token == "~" || token == "$HOME") {
				env_val = getenv("HOME");
				result =
				    result / (env_val ? std::string(env_val)
				                      : std::string(""));
			} else if (token[0] == '$') {
				// remove the $ from the varaible
				token = token.substr(1, token.length());
				env_val = getenv(token.c_str());
				result =
				    result / (env_val ? std::string(env_val)
				                      : std::string(""));
			} else {
#elif defined(__WIN32__)
			if (token[0] == '%' && token[token.length()] == '%') {
				// Remove the % around the varaible
				token = token.substr(1, token.length() - 1);
				env_val = getenv(token.c_str());
				result =
				    result / (env_val ? std::string(env_val)
				                      : std::string(""));

			} else {
#endif
				result = result / path_iter->string();
			}
		}
		return fs::canonical(result);
	} catch (elemental::Exception& e) {
		throw e;
	} catch (std::exception& e) {
		throw Exception(e);
	}
}
} // namespace elemental::paths

// clang-format off
// vim: set foldmethod=syntax textwidth=80 ts=8 sts=0 sw=8  noexpandtab ft=cpp.doxygen :
