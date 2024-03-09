/* IEventSource.hpp
 * Copyright © 2024 Saul D. Beniquez
 * License: Mozilla Public License v. 2.0
 *
 * This Source Code Form is subject to the terms of the Mozilla Public License,
 * v.2.0. If a copy of the MPL was not distributed with this file, You can
 * obtain one at https://mozilla.org/MPL/2.0/.
 */

#pragma once

#include <any>
#include <functional>

#include "INonCopyable.hpp"
#include "Observable.hpp"

namespace elemental {

enum class InputDevices : unsigned int;

struct IEventSource
    : public Observable
    , private INonCopyable
{
	~IEventSource() override = default;
	virtual auto pollEvents() -> void = 0;
	virtual auto transmitEvents() -> void = 0;

  protected:
	explicit IEventSource(InputDevices device_flags) {}
};

} // namespace elemental

// clang-format off
// vim: set foldmethod=syntax foldminlines=10 textwidth=80 ts=8 sts=0 sw=8 noexpandtab ft=cpp.doxygen :
