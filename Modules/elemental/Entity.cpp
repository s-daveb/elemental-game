/* Entity.cpp
 * Copyright © 2024 Saul D. Beniquez
 * License: Mozilla Public License v2.0 (MPL2)
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v.2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/.
 */

#include "Entity.hpp"

#include "IOCore/Exception.hpp"

#include <string>
#include <vector>

namespace elemental {

Entity::Entity(
    const EntityTypedata& typedata, const std::vector<int>& position,
    const std::vector<int>& size, uint8_t layer)
)
    : metadata(typedata), position(position), size(size), layer(layer)
{
}

Entity::~Entity() = default;

std::ostream&
operator<<(std::ostream& outstream, const EntityTypedata& entityInfo)
{
}

}

} // namespace elemental
