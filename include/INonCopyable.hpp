/* INonCopyable.hpp
 * Copyright © 2018-2023 Saul D. Beniquez
 * License: Mozilla Public License v. 2.0
 *
 * This Source Code Form is subject to the terms of the Mozilla Public License,
 * v.2.0. If a copy of the MPL was not distributed with this file, You can
 * obtain one at https://mozilla.org/MPL/2.0/.
 */

#pragma once
namespace elemental {

class INonCopyable
{
  protected:
	INonCopyable() = default;

  private:
	INonCopyable(const INonCopyable&) = default;
	INonCopyable(INonCopyable&&) = default;
	INonCopyable& operator=(const INonCopyable&) = default;
	INonCopyable& operator=(INonCopyable&&) = default;
};
} // namespace elemental

// clang-format off
// vim: set foldmethod=marker foldmarker=#region,#endregion textwidth=80 ts=8 sts=0 sw=8 noexpandtab ft=cpp.doxygen :
