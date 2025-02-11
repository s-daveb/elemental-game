/* IDrawCommand.hpp
 * Copyright © 2024 Saul D. Beniquez
 * License: Mozilla Public License v. 2.0
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v.2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/.
 */

#pragma once

#include "IRenderer.hpp"
#include "types.hpp"

namespace elemental {

struct IDrawCommand {
	virtual ~IDrawCommand() = default;
	virtual auto rectangle() -> Rectangle& = 0;
	virtual auto imageData() -> std::shared_ptr<void>& = 0;
	virtual auto draw() -> ErrorFlag = 0;

    protected:
	IDrawCommand(Rectangle& rect, std::shared_ptr<void>& data) {}
};
}
// clang-format off
// vim: set foldmethod=syntax foldminlines=10 textwidth=80 ts=8 sts=0 sw=8 noexpandtab ft=cpp.doxygen :

