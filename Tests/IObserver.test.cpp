/*
 * Copyright © 2024 Saul D. Beniquez
 * License: Mozilla Public License v2.0 (MPL2)
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v.2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/.
 */

#include "elemental/IObserver.hpp"
#include "elemental/Observable.hpp"

#include "test-utils/common.hpp"

#include <any>
#include <fakeit.hpp>

BEGIN_TEST_SUITE("elemental::IObserver")
{
	using namespace elemental;

	class MockObservable : public Observable {
	    public:
		MockObservable()
		{
			using namespace fakeit;
			// In the test build these methods are virtual so we
			// can override them
			When(Method(mock, registerObserver))
			    .AlwaysDo([](elemental::Observable::ObserverRef
			              ) {});
			//			When(Method(mock,
			// notify_all))
			//    .AlwaysDo([](std::any) {});
		}

		fakeit::Mock<elemental::Observable> mock;
		elemental::Observable& get() { return mock.get(); }
	};

	struct DummyObserver : public IObserver {
		friend class IObserver;
		virtual ~DummyObserver() override = default;

		void recieveMessage(
		    const Observable& sender, std::any message = std::any()
		) override
		{
			toggled = true;
		}

		DummyObserver() : IObserver() {}
		bool toggled = false;
	};

	TEST("Constructor")
	{
		REQUIRE_NOTHROW([&]() { DummyObserver test; }());
	}

	TEST("recieveMessage is called correctly")
	{
		// Create a concrete observable that uses the real
		// implementation.
		class TestObservable : public elemental::Observable {
		    public:
			using elemental::Observable::notify_all;
		};

		TestObservable observable;
		DummyObserver dummy;
		// Register the observer using std::ref to wrap it as a
		// reference_wrapper.
		REQUIRE(dummy.toggled == false);
		observable.registerObserver(std::ref(dummy));
		// Trigger notification with any message.
		REQUIRE(dummy.toggled == false);
		observable.notify_all("test message");
		// Verify that DummyObserver's recieveMessage was invoked.
		REQUIRE(dummy.toggled == true);
	}
}

// clang-format off
// vim: set textwidth=80 ts=8 sts=0 sw=8 noexpandtab ft=cpp.doxygen :
// clang-format off
// vim: set  textwidth=80 ts=8 sts=0 sw=8 noexpandtab ft=cpp.doxygen :
