/*
 * Copyright © 2024 Saul D. Beniquez
 * License: Mozilla Public License v. 2.0
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v.2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/.
 */

#pragma once

#include <mutex>
#include <string>

#include <fontconfig/fontconfig.h>

namespace elemental {

class FontConfig {
    public:
	virtual ~FontConfig();

	// Get the singleton instance of FontBook
	static auto getInstance() -> FontConfig&;

	// Retrieve the font path by name (e.g., "monospace")
	auto getFont(const std::string& font_name) -> std::string;

	FontConfig(const FontConfig&) = delete;
	auto operator=(const FontConfig&) -> FontConfig& = delete;

    protected:
	FontConfig();

	FcConfig* config;
	std::mutex access_mutex;
};
}

// clang-format off
// vim: set foldmethod=syntax foldminlines=10 textwidth=80 ts=8 sts=0 sw=8 noexpandtab ft=cpp.doxygen :
