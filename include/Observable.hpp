/* IObservable.hpp
 * Copyright © 2019-2023 Saul D. Beniquez
 * License: Mozilla Public License v. 2.0
 *
 * This Source Code Form is subject to the terms of the Mozilla Public License,
 * v.2.0. If a copy of the MPL was not distributed with this file, You can
 * obtain one at https://mozilla.org/MPL/2.0/.
 */

#pragma once

#include <any>
#include <functional>
#include <list>

namespace elemental {

class IObserver;

class Observable
{
  public:
	typedef std::reference_wrapper<IObserver> observer_ref;
	typedef std::list<observer_ref> observer_list;

	virtual ~Observable() {}

	void RegisterObserver(observer_ref);
	virtual void Notify() = 0;

  protected:
	void NotifyAll(std::any message = std::any());
	observer_list observers;
};

}
