/* main.cpp
 * Copyright © 2023 Saul D. Beniquez
 * License: Mozilla Public License v. 2.0
 *
 * This Source Code Form is subject to the terms of the Mozilla Public License,
 * v.2.0. If a copy of the MPL was not distributed with this file, You can
 * obtain one at https://mozilla.org/MPL/2.0/.
 */

#include <iostream>

#include "ElementalGame.hpp"
#include "Exception.hpp"
#include "IApplication.hpp"
#include "Singleton.template.hpp"

#include "any_ptr.hpp"

using namespace elemental;

int
main(int argc, c::const_string argv[], c::const_string envp[])
{
	try {
		auto& game_instance = Singleton<ElementalGame>::GetInstance();
		game_instance.Init(argc, argv, envp);

		return game_instance.Run();
	} catch (elemental::Exception& custom_exception) {
		std::cerr << custom_exception.what() << std::endl;
		return 1;
	} catch (std::exception& stl_exception) {
		std::cerr << stl_exception.what() << std::endl;
		return -1;
	}
}

// clang-format off
// vim: set foldmethod=syntax foldlevel=99 textwidth=80 ts=8 sts=0 sw=8  noexpandtab ft=cpp.doxygen :
