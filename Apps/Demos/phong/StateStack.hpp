/*
 * Copyright © 2024 Saul D. Beniquez
 * License: Mozilla Public License v. 2.0
 *
 * This Source Code Form is subject to the terms of the Mozilla Public License,
 * v.2.0. If a copy of the MPL was not distributed with this file, You can
 * obtain one at https://mozilla.org/MPL/2.0/.
 */

#pragma once

#include "IDrawable.hpp"
#include "IObserver.hpp"
#include "IState.hpp"
#include "IStateMachine.hpp"

#include "IOCore/Exception.hpp"
#include "types/errors.hpp"

#include <any>
#include <memory>
#include <stack>

namespace elemental {

struct StateStack
    : public IObserver
    , public IStateMachine {
    public:
	StateStack() = default;
	~StateStack() override = default;

	void step() override;
	auto pushState(std::unique_ptr<IState> state) -> void;

	void recieveMessage(
	    const Observable& sender, std::any message = std::any()
	) override;

	auto draw() -> ErrorFlag override;

    private:
	std::stack<std::unique_ptr<IState>> stack;
};

} // namespace elemental

// clang-format off
// vim: set foldmethod=syntax foldminlines=10 textwidth=80 ts=8 sts=0 sw=8 noexpandtab ft=cpp.doxygen :
