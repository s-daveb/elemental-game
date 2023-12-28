/* debuginfo.h
 * Copyright © 2020-2023 Saul D. Beniquez
 * License: Mozilla Public License v. 2.0
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */
#pragma once

#include <iostream>
#include <sstream>

#ifdef DEBUG // #region
#include <iostream>
#define debugprint(msg) std::cout << "[DBG]  " << msg << std::endl
#else
#define debugprint(msg) ;
#endif // #endregion

namespace elemental {

std::string generate_stacktrace(unsigned short framesToRemove = 1);

void print_cmdline(int argc, const char* argv[]);

} // namespace elemental

// clang-format off
// vim: set foldmethod=syntax foldlevel=99 textwidth=80 ts=8 sts=0 sw=8 noexpandtab ft=cpp.doxygen :
