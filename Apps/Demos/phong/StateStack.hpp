/*
 * Copyright © 2024 Saul D. Beniquez
 * License: Mozilla Public License v. 2.0
 *
 * This Source Code Form is subject to the terms of the Mozilla Public License,
 * v.2.0. If a copy of the MPL was not distributed with this file, You can
 * obtain one at https://mozilla.org/MPL/2.0/.
 */

#pragma once

#include "IObserver.hpp"

#include "IOCore/Exception.hpp"
#include <any>

namespace elemental {

struct IStateMachine {
	virtual void step() = 0;
	virtual ~IStateMachine() = default;
};

struct StateStack
    : public IObserver
    , public IStateMachine {
	StateStack() = default;
	~StateStack() override = default;

	void step() override { throw IOCore::NotImplementedException(); }

	void recieveMessage(
	    const Observable& sender, std::any message = std::any()
	) override
	{
		throw IOCore::NotImplementedException();
	}
};

} // namespace elemental

// clang-format off
// vim: set foldmethod=syntax foldminlines=10 textwidth=80 ts=8 sts=0 sw=8 noexpandtab ft=cpp.doxygen :
