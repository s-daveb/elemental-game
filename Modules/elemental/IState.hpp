/*
 * Copyright © 2024 Saul D. Beniquez
 * License: Mozilla Public License v. 2.0
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v.2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/.
 */

#pragma once

#include <any>
#include <list>
#include <memory>

#include "IDrawCommand.hpp"

#include "IObserver.hpp"
#include "Observable.hpp"

namespace elemental {

struct IState : public IObserver {
	IState() = default;
	~IState() override = default;

	auto recieveMessage(const Observable& sender, std::any message)
	    -> void override = 0;

	virtual auto step() -> void = 0;
	virtual auto getDrawCommands()
	    -> std::list<std::shared_ptr<IDrawCommand>> = 0;
};

}

// clang-format off
// vim: set  textwidth=80 ts=8 sts=0 sw=8 noexpandtab ft=cpp.doxygen :
