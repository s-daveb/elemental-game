/* pointers.hpp
 * Copyright © 2024 Saul D. Beniquez
 * License: Mozilla Public License v. 2.0
 *
 * This Source Code Form is subject to the terms of the Mozilla Public License,
 * v.2.0. If a copy of the MPL was not distributed with this file, You can
 * obtain one at https://mozilla.org/MPL/2.0/.
 */

#pragma once

#include <memory>

namespace elemental {

template<typename TData>
using Ptr = std::shared_ptr<TData>;

template<typename TData>
using UniquePtr = std::unique_ptr<TData>;

template<typename TData, typename... TArgs>
constexpr auto
Make_Ptr(TArgs&&... args) -> decltype(auto)
{
	return std::make_shared<TData>(std::forward<TArgs>(args)...);
}

template<typename TData, typename... TArgs>
constexpr auto
Make_Unique_Ptr(TArgs&&... args) -> decltype(auto)
{
	return std::make_unique<TData>(std::forward<TArgs>(args)...);
}

} // namespace elemental

// clang-format off
// vim: set foldmethod=syntax foldminlines=10 textwidth=80 ts=8 sts=0 sw=8 noexpandtab ft=cpp.doxygen :
