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

using namespace elemental;

using fakeit::Mock;
using fakeit::When;

BEGIN_TEST_SUITE("elemental::Observable")
{
	class ObservableSubject : public elemental::Observable
	{
	  public:
		ObservableSubject() : Observable() {}

		virtual void Notify() override { this->NotifyAll(); }
		const size_t GetObserverCount() const
		{
			return observers.size();
		}
	};

	TEST_CASE("Register IObservers", TEST_SUITE_NAME)
	{
		Mock<IObserver> observerType1;
		Mock<IObserver> observerType2;
		ObservableSubject subject;

		IObserver& observer1 = observerType1.get();
		IObserver& observer2 = observerType2.get();

		subject.RegisterObserver(observer1);
		subject.RegisterObserver(observer2);

		REQUIRE(2 == subject.GetObserverCount());
	}
	TEST_CASE("Can notify observers", TEST_SUITE_NAME)
	{
		Mock<IObserver> observerType1;
		Mock<IObserver> observerType2;
		ObservableSubject subject;

		static std::vector<std::string> buffer;

		When(Method(observerType1, OnNotice))
		    .Do([](const Observable& o, std::any m) {
			    buffer.push_back("First Observer");
		    })
		    .AlwaysReturn();

		When(Method(observerType2, OnNotice))
		    .Do([](const Observable& o, std::any m) {
			    buffer.push_back("Second Observer");
		    })
		    .AlwaysReturn();

		IObserver& observer1 = observerType1.get();
		IObserver& observer2 = observerType2.get();

		subject.RegisterObserver(observer1);
		subject.RegisterObserver(observer2);

		subject.Notify();

		REQUIRE(2 == buffer.size());
		CHECK(buffer.at(0) == "First Observer");
		CHECK(buffer.at(1) == "Second Observer");
	}
}

// clang-format off
// vim: set foldmethod=syntax  textwidth=80 ts=8 sts=0 sw=8  noexpandtab ft=cpp.doxygen :
