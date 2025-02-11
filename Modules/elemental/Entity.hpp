/* Entity.hpp
 * Copyright © 2024 Saul D. Beniquez
 * License: Mozilla Public License v2.0 (MPL2)
 *
 * This Source Code Form is subject to the terms of the Mozilla Public License,
 * v.2.0. If a copy of the MPL was not distributed with this file, You can
 * obtain one at https://mozilla.org/MPL/2.0/.
 */

#pragma once

#include <string>
#include <vector>

namespace elemental {
class Entity {
    public:
	Entity(
	    const std::string& type, const std::vector<int>& position,
	    const std::vector<int>& size, const std::string& layer
	);
	virtual ~Entity();

	// Placeholder method for behaviors
	void loadBehavior(const std::string& script);

    private:
	std::string type;
	std::vector<int> position;
	std::vector<int> size;
	std::string layer;
};

}

// clang-format off
// vim: set foldmethod=syntax foldminlines=10 textwidth=80 ts=8 sts=0 sw=8 noexpandtab ft=cpp.doxygen :
