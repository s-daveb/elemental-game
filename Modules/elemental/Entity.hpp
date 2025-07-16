/* Entity.hpp
 * Copyright © 2024 Saul D. Beniquez
 * License: Mozilla Public License v2.0 (MPL2)
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v.2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/.
 */

#pragma once

#include <string>
#include <vector>

#include "types/entities.hpp"

namespace elemental {

class Entity {
    protected:
	EntityTypedata metadata;
	std::vector<int> position;
	std::vector<int> size;
	std::string layer;

    public:
	Entity(
	    const EntityTypedata& typedata, const std::vector<int>& size,
	    const std::vector<int>& position, uint8_t layer = 3
	);
	virtual ~Entity();

	const EntityTypedata& Metadata = metadata;

	friend auto operator<<(std::ostream&, const EntityTypedata&)
	    -> std::ostream&;
};

}

// clang-format off
// vim: set  textwidth=80 ts=8 sts=0 sw=8 noexpandtab ft=cpp.doxygen :
