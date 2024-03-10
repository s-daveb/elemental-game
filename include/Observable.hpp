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
	using ObserverRef = std::reference_wrapper<IObserver>;
	using ObserverList = std::list<ObserverRef>;

	/** \name Deleteed Constructors & Operators
	 * \{ */
	Observable(const Observable&) = default;
	Observable(Observable&&) = delete;
	auto operator=(const Observable&) -> Observable& = default;
	auto operator=(Observable&&) -> Observable& = delete;
	/**  \} */

	virtual ~Observable() = default;

	void registerObserver(ObserverRef);

  protected:
	Observable() = default;
	void notify_all(std::any message = std::any());
	ObserverList observers;
};

} // namespace elemental
  // clang-format off
// vim: set foldmethod=syntax textwidth=80 ts=8 sts=0 sw=8  noexpandtab ft=cpp.doxygen :
