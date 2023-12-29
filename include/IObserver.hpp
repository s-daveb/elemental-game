/* IObservable.hpp
 * Copyright © 2020-2023 Saul D. Beniquez
 * License:  MPL2
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */
#pragma once

#include "INonCopyable.hpp"
#include <any>

namespace elemental {

class Observable;

class IObserver : INonCopyable
{
  public:
	virtual void OnNotice(const Observable& sender,
	                      std::any message = std::any()) = 0;

	virtual ~IObserver() = default;

  protected:
	IObserver() = default;
};

}
// clang-format off
// vim: set foldmethod=marker foldmarker=#region,#endregion textwidth=80 ts=8 sts=0 sw=8 noexpandtab ft=cpp.doxygen :
