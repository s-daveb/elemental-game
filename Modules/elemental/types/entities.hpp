/*
 * Copyright © 2024 Saul D. Beniquez
 * License: Mozilla Public License v. 2.0
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v.2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/.
 */

#pragma once

#include <functional>
#include <ostream>

#include "types.hpp"

namespace elemental {
using entity_type_t = uint16_t;

struct EntityTypedata {
	entity_type_t type_id;
	const char* type_name;
};

auto operator<<(std::ostream& stream, const EntityTypedata& data)
    -> std::ostream&
{
	stream << "[" << data.type_name << "]" << std::flush;
	return stream;
}

}

namespace std {
using elemental::EntityTypedata;

template<>
struct hash<EntityTypedata> {
	auto operator()(const EntityTypedata& data) const -> size_t
	{
		return std::hash<decltype(data.type_id)>{}(data.type_id);
	}
};
}

// clang-format off
// vim: set ts=8 sts=0 sw=8 noexpandtab ft=cpp.doxygen :
