/* input.hpp
 * Copyright © 2024 Saul D. Beniquez
 * License: Mozilla Public License v. 2.0
 *
 * This Source Code Form is subject to the terms of the Mozilla Public License,
 * v.2.0. If a copy of the MPL was not distributed with this file, You can
 * obtain one at https://mozilla.org/MPL/2.0/.
 */

#pragma once

#include "util/bitwise.hpp"

#include <type_traits>

namespace elemental {
enum class InputDevices : unsigned int
{
	None = 0x0000,
	Keyboard = 0x0001,
	Mouse = 0x0010,
	Joystick = 0x0100,
	All = 0x0111 // Keyboard & Mouse & Joystick
};
// Enable bitwise operations on InputDevice
constexpr auto
operator|(InputDevices lhs, InputDevices rhs) -> InputDevices
{
	return static_cast<InputDevices>(
	    static_cast<std::underlying_type_t<InputDevices>>(lhs) |
	    static_cast<std::underlying_type_t<InputDevices>>(rhs));
}
constexpr auto
operator&(InputDevices lhs, InputDevices rhs) -> InputDevices
{
	return static_cast<InputDevices>(
	    static_cast<std::underlying_type_t<InputDevices>>(lhs) &
	    static_cast<std::underlying_type_t<InputDevices>>(rhs));
}

} // namespace elemental

// clang-format off
// vim: set foldmethod=syntax foldminlines=10 textwidth=80 ts=8 sts=0 sw=8 noexpandtab ft=cpp.doxygen :
