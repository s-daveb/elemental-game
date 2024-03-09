/* Observable.test.cpp
 * Copyright © 2021-2023 Saul D. Beniquez
 * License: Mozilla Public License v. 2.0
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 License, v. 2.0. If a copy of the MPL was not distributed with this
 file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <any>
#include <iostream>
#include <list>
#include <string>

#include "INonCopyable.hpp"
#include "IObserver.hpp"
#include "Observable.hpp"

#include "test-utils/common.hpp"
#include <fakeit.hpp>

using elemental::Observable;
using fakeit::Mock;
using fakeit::When;

BEGIN_TEST_SUITE("elemental::Observable")
{
	class ObservableSubject : public Observable
	{
	  public:
		ObservableSubject() : Observable() {}

		auto notify()  { this->notify_all(); }
		auto getObserverCount() const -> const size_t
		{
			return observers.size();
		}
	};

	TEST("elemental::Observable::RegisterObserver works")
	{
		Mock<elemental::IObserver> observer_type1;
		Mock<elemental::IObserver> observer_type2;
		ObservableSubject subject;

		elemental::IObserver& observer1 = observer_type1.get();
		elemental::IObserver& observer2 = observer_type2.get();

		subject.registerObserver(observer1);
		subject.registerObserver(observer2);

		REQUIRE(2 == subject.getObserverCount());
	}
	TEST("elemental::Observabl::Notify - properly notifies observers")
	{
		Mock<elemental::IObserver> observer_type1;
		Mock<elemental::IObserver> observer_type2;
		ObservableSubject subject;

		static std::vector<std::string> buffer;

		When(Method(observer_type1, recieveMessage))
		    .Do([](const Observable& o, std::any m) {
			    buffer.emplace_back("First Observer");
		    })
		    .AlwaysReturn();

		When(Method(observer_type2, recieveMessage))
		    .Do([](const Observable& o, std::any m) {
			    buffer.emplace_back("Second Observer");
		    })
		    .AlwaysReturn();

		elemental::IObserver& observer1 = observer_type1.get();
		elemental::IObserver& observer2 = observer_type2.get();

		subject.registerObserver(observer1);
		subject.registerObserver(observer2);

		subject.notify();

		REQUIRE(2 == buffer.size());
		CHECK(buffer.at(0) == "First Observer");
		CHECK(buffer.at(1) == "Second Observer");
	}
}

// clang-format off
// vim: set foldmethod=syntax foldlevel=2 textwidth=80 ts=8 sts=0 sw=8  noexpandtab ft=cpp.doxygen :
