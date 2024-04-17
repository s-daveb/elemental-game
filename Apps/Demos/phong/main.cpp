/* main.cpp
 * Copyright © 2023 Saul D. Beniquez
 * License: Mozilla Public License v. 2.0
 *
 * This Source Code Form is subject to the terms of the Mozilla Public License,
 * v.2.0. If a copy of the MPL was not distributed with this file, You can
 * obtain one at https://mozilla.org/MPL/2.0/.
 */

#include "./Phong.hpp"

#include <iostream>

#include "Exception.hpp"
#include "Singleton.hpp"

using namespace elemental;

auto main(int argc, c::const_string args[], c::const_string env[]) -> int
{
	try {
		auto game_instance = Phong(argc, args, env);
		return game_instance.run();
	} catch (IOCore::Exception& custom_exception) {
		std::cerr << custom_exception.what() << std::endl;
		return 1;
	} catch (std::exception& stl_exception) {
		std::cerr << stl_exception.what() << std::endl;
		return -1;
	} catch (...) {
		std::cerr << "Unknown exception" << std::endl;
		return -2;
	}
}

// clang-format off
// vim: set foldmethod=syntax foldminlines=10 textwidth=80 ts=8 sts=0 sw=8 noexpandtab ft=cpp.doxygen :
