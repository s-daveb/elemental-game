/* testinh.hpp
 * Copyright © 2024 Saul D. Beniquez
 * License: Mozilla Public License v. 2.0
 *
 * This Source Code Form is subject to the terms of the Mozilla Public License,
 * v.2.0. If a copy of the MPL was not distributed with this file, You can
 * obtain one at https://mozilla.org/MPL/2.0/.
 */

#pragma once

namespace elemental::debug {

template<typename T_>
class Inspector;
}

/** \brief this macro adds a friend class to this class definition that
 * can be used in unit tests to view private variables */
#define TEST_INSPECTABLE(TClassName)                                           \
	friend class elemental::debug::Inspector<TClassName>;

/*
#define TEST_INSPECTABLE(TClassName) \
#if defined(UNIT_TEST)\
template<>\
friend class Inspector<TClassName>;\
#endif\
*/

// clang-format off
// vim: set foldmethod=syntax textwidth=80 ts=8 sts=0 sw=8  noexpandtab ft=cpp.doxygen :
