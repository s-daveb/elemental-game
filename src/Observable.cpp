/* Observable.cpp
 * Copyright © 2020-2023 Saul D. Beniquez
 * License: MPL2
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "Observable.hpp"
#include "IObserver.hpp"

#include "IObserver.hpp"
#include <functional>

namespace elemental {

void
Observable::registerObserver(Observable::observer_ref observer)
{
	this->observers.push_back(observer);
}

void
Observable::notify_all(std::any message)
{
	for (auto& observer : this->observers) {
		observer.get().recieveMessage(*this, message);
	}
}
} // namespace elemental

// clang-format off
// clang-format off
// vim: set foldmethod=syntax textwidth=80 ts=8 sts=0 sw=8  noexpandtab ft=cpp.doxygen :
