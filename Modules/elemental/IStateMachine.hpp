/*
 * Copyright © 2024 Saul D. Beniquez
 * License: Mozilla Public License v. 2.0
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v.2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/.
 */

#pragma once

#include "IDrawable.hpp"
#include "types/rendering.hpp"

#include "IOCore/types/errors.hpp"

namespace elemental {

struct IStateMachine {
	virtual void step() = 0;
	virtual ~IStateMachine() = default;

	virtual auto draw() -> ErrorFlag = 0;
};

}

// clang-format off
// vim: set  textwidth=80 ts=8 sts=0 sw=8 noexpandtab ft=cpp.doxygen :
