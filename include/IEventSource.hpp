/* IEventSource.hpp
 * Copyright © 2023 Saul D. Beniquez
 * License: Mozilla Public License v. 2.0
 *
 * This Source Code Form is subject to the terms of the Mozilla Public License,
 * v.2.0. If a copy of the MPL was not distributed with this file, You can
 * obtain one at https://mozilla.org/MPL/2.0/.
 */

#pragma once

#include "INonCopyable.hpp"
#include "Observable.hpp"
#include "Singleton.thpp"

namespace elemental {
enum DeviceFlags : int
{
	UNDEFINED = 0x0,
	KEYBOARD = 0x1,                   // 0001 in binary
	MOUSE = 0x2,                      // 0010 in binary
	JOYSTICK = 0x4,                   // 0100 in binary
	ALL = KEYBOARD | MOUSE | JOYSTICK // 0111 in binary
};

struct IEventSource
    : public INonCopyable
    , public Observable
{

	SINGLETON_INTERFACE(IEventSource);

	virtual ~IEventSource() {}
	virtual void InitDevices(DeviceFlags flags = ALL) = 0;

	virtual void PollEvents() = 0;

  protected:
	IEventSource() : INonCopyable(), Observable() {}
};
}

// clang-format off
// vim: set foldmethod=syntax textwidth=80 ts=8 sts=0 sw=8 foldlevel=99 noexpandtab ft=cpp.doxygen :
