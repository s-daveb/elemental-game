/*
 * Copyright © 2024 Saul D. Beniquez
 * License: Mozilla Public License v. 2.0
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v.2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/.
 */

#include "test-utils/common.hpp"

#include "IEventSource.hpp"
#include "types/input.hpp"

using elemental::IEventSource;
using elemental::InputDevices;

struct MockEventSource : public elemental::IEventSource
{
	explicit MockEventSource(InputDevices device_flags = InputDevices::None)
	    : IEventSource(device_flags), device_flags(device_flags)
	{
	}

	void pollEvents() override final {}
	void sendEvents() override final {}

    private:
	elemental::InputDevices device_flags;
};

BEGIN_TEST_SUITE("IEventSource_Mocks")
{
	TEST("pollEvents should be called when requested")
	{
		MockEventSource mock;

		REQUIRE_NOTHROW(mock.pollEvents());
	}

	TEST("sendEvents should be called when requested")
	{
		MockEventSource mock;

		REQUIRE_NOTHROW(mock.sendEvents());
	}
}
// clang-format off
// vim: set  textwidth=80 ts=8 sts=0 sw=8 noexpandtab ft=cpp.doxygen :
