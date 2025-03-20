/*
 * Copyright © 2024 Saul D. Beniquez
 * License: Mozilla Public License v. 2.0
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v.2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/.
 */

#include "../Modules/elemental/types/input.hpp"
#include "test-utils/common.hpp"

BEGIN_TEST_SUITE("input-types")
{
	using namespace elemental;

	TEST_CASE("types/input.hpp: Bitwise OR operator")
	{
		// Test combining multiple devices
		REQUIRE(
		    (InputDevices::Keyboard | InputDevices::Mouse) ==
		    static_cast<elemental::InputDevices>(0x0011)
		);

		// Test combining any device with InputDevices::All should
		// result in InputDevices::All
		REQUIRE(
		    (InputDevices::Keyboard | InputDevices::All) ==
		    InputDevices::All
		);
		REQUIRE(
		    (InputDevices::Mouse | InputDevices::All) ==
		    InputDevices::All
		);
		REQUIRE(
		    (InputDevices::Joystick | InputDevices::All) ==
		    InputDevices::All
		);

		// Test InputDevices::Joystick and InputDevices::Mouse
		// combination
		REQUIRE(
		    (InputDevices::Joystick | InputDevices::Mouse) ==
		    static_cast<InputDevices>(0x0110)
		);
	}

	// Test cases for bitwise AND operator
	TEST_CASE("types/input.hpp: Bitwise AND operator")
	{
		// Test combining multiple devices using AND
		REQUIRE(
		    (InputDevices::Keyboard & InputDevices::Mouse) ==
		    InputDevices::None
		);

		// Test InputDevices::All with a subset should return that
		// subset
		REQUIRE(
		    (InputDevices::All & InputDevices::Keyboard) ==
		    InputDevices::Keyboard
		);
		REQUIRE(
		    (InputDevices::All & InputDevices::Mouse) ==
		    InputDevices::Mouse
		);
		REQUIRE(
		    (InputDevices::All & InputDevices::Joystick) ==
		    InputDevices::Joystick
		);
	}
}

// clang-format off
// vim: set foldmethod=syntax foldminlines=10 textwidth=80 ts=8 sts=0 sw=8 noexpandtab ft=cpp.doxygen :
