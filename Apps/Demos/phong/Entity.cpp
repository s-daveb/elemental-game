/* Entity.cpp
 * Copyright © 2024 Saul D. Beniquez
 * License: Mozilla Public License v2.0 (MPL2)
 *
 * This Source Code Form is subject to the terms of the Mozilla Public License,
 * v.2.0. If a copy of the MPL was not distributed with this file, You can
 * obtain one at https://mozilla.org/MPL/2.0/.
 */

#include "Entity.hpp"

#include "IOCore/Exception.hpp"

#include <string>
#include <vector>

namespace elemental {

Entity::Entity(
    const std::string& type, const std::vector<int>& position,
    const std::vector<int>& size, const std::string& layer
)
    : type(type), position(position), size(size), layer(layer)
{
}

Entity::~Entity() = default;

void Entity::loadBehavior(const std::string& script)
{
	throw IOCore::NotImplementedException();
}

} // namespace elemental
