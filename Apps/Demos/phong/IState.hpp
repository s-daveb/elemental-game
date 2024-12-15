/*
 * Copyright © 2024 Saul D. Beniquez
 * License: Mozilla Public License v. 2.0
 *
 * This Source Code Form is subject to the terms of the Mozilla Public License,
 * v.2.0. If a copy of the MPL was not distributed with this file, You can
 * obtain one at https://mozilla.org/MPL/2.0/.
 */

#pragma once

#include <list>
#include <memory>

#include "IDrawable.hpp"

#include "IObserver.hpp"
#include "Observable.hpp"

namespace elemental {

struct IState : public IObserver {
	IState() = default;
	~IState() override = default;

	auto recieveMessage(const Observable& sender, std::any message)
	    -> void override = 0;

	virtual auto step() -> void = 0;
	virtual auto getDrawables() -> std::list<std::unique_ptr<IDrawable>> = 0;
};

}

// clang-format off
// vim: set foldmethod=syntax foldminlines=10 textwidth=80 ts=8 sts=0 sw=8 noexpandtab ft=cpp.doxygen :
