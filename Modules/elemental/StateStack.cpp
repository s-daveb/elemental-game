/*
 * Copyright © 2024 Saul D. Beniquez
 * License: Mozilla Public License v. 2.0
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v.2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/.
 */

#include "StateStack.hpp"

#include "IOCore/Exception.hpp"
#include "IOCore/types/errors.hpp"

#include "IState.hpp"
#include "Observable.hpp"

#include <any>
#include <memory>
#include <utility>

using elemental::StateStack;
using namespace IOCore;

auto StateStack::step() -> void
{
	ASSERT(this->stack.empty() == false);

	stack.top()->step();
}

void StateStack::pop()
{ this->stack.pop(); }

void StateStack::clear()
{ this->stack = std::stack<std::unique_ptr<IState>>(); }

auto StateStack::pushState(std::unique_ptr<IState>& state) -> void
{ this->stack.push(std::move(state)); }

auto StateStack::pushState(std::unique_ptr<IState>&& state) -> void
{ this->stack.push(std::move(state)); }

auto StateStack::receiveMessage(const Observable& sender, std::any message)
    -> void
{
	ASSERT(message.has_value());
	ASSERT(this->stack.empty() == false);

	this->stack.top()->receiveMessage(sender, message);
}

auto StateStack::draw() -> ErrorFlag
{
	auto return_value = kSuccess;
	for (auto& drawable: this->stack.top()->getDrawCommands()) {
		return_value &= drawable->draw();
	}
	return return_value;
}

// clang-format off
// vim: set  textwidth=80 ts=8 sts=0 sw=8 noexpandtab ft=cpp.doxygen :
