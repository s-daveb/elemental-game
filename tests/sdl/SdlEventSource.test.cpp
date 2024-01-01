/*SdlEventSource.test.cpp
 * Copyright © 2023 Saul D. Beniquez
 * License: Mozilla Public License v. 2.0
 *
 * This Source Code Form is subject to the terms of the Mozilla Public License,
 * v.2.0. If a copy of the MPL was not distributed with this file, You can
 * obtain one at https://mozilla.org/MPL/2.0/.
 */

#include "IObserver.hpp"
#include "SdlEventSource.hpp"

#include "test-utils/SdlHelpers.hpp"
#include "test-utils/common.hpp"

BEGIN_TEST_SUITE("elemental::SdlEventSource")
{
	using namespace elemental;

	struct SdlEventSourceFixture : public SdlTestFixture
	{
		SdlEventSourceFixture() : SdlTestFixture()
		{
			test_subject =
			    IEventSource::GetInstance<SdlEventSource>();
		}

		~SdlEventSourceFixture() {}
		SdlEventSource* test_subject;
	};
	using TestFixture = SdlEventSourceFixture;

	class EventRecorder : public IObserver
	{
	  public:
		EventRecorder() : IObserver() {}

		virtual void OnNotice(const Observable& sender,
		                      std::any message)
		{
			auto event = std::any_cast<SDL_Event&>(message);
			received.push_back(event);
		}
		virtual ~EventRecorder() = default;
		std::vector<SDL_Event> received;
	};
	class SdlEventSourceViewer : public SdlEventSource
	{
	  public:
		static const std::queue<SDL_Event>& GetEventQueue(
		    SdlEventSource* other)
		{
			return other->event_queue;
		}
	}; // #endregion

	FIXTURE_TEST("SdlEventSource - Initialization and Enqueue")
	{

		// Your test code goes here
		test_subject->InitDevices(); // Example: Initialize devices
		auto& event_queue_ref =
		    SdlEventSourceViewer::GetEventQueue(test_subject);

		REQUIRE(event_queue_ref.size() == 0);
		// Enqueue an SDL_Event (replace this with your actual event
		// creation logic)
		SDL_Event testEvent;
		testEvent.type = SDL_KEYDOWN;
		test_subject->Enqueue(&testEvent);

		REQUIRE(event_queue_ref.size() == 1);
	}

	FIXTURE_TEST("SdlEventSource - Notify")
	{
		// Your test code goes here
		// Example: Call Notify method and add assertions to test its
		// functionality
		test_subject->Notify();

		// Add assertions as needed
		REQUIRE(true);
	}

	FIXTURE_TEST("SdlEventSource - PollEvents")
	{
		SdlEventSourceFixture fixture; // Instantiate the fixture

		// Your test code goes here
		// Example: Call PollEvents method and add assertions to test
		// its functionality
		fixture.test_subject->PollEvents();

		// Add assertions as needed
		REQUIRE(true);
	}

	// Add more tests as needed

} // End of test suite

// clang-format off
 // vim: set foldmethod=syntax textwidth=80 ts=8 sts=0 sw=8 foldlevel=99 noexpandtab ft=cpp.doxygen :
