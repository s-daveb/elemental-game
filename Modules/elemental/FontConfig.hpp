/*
 * Copyright © 2024 Saul D. Beniquez
 * License: Mozilla Public License v. 2.0
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v.2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/.
 */

#pragma once

#include "IOCore/Exception.hpp"

#include <fontconfig/fontconfig.h>
#include <mutex>
#include <stdexcept>
#include <string>

class FontConfig {
    public:
	// Get the singleton instance of FontBook
	static auto getInstance() -> FontConfig&
	{
		static FontConfig instance;
		return instance;
	}

	// Retrieve the font path by name (e.g., "monospace")
	auto getFont(const std::string& name) -> std::string
	{
		std::lock_guard<std::mutex> lock(access_mutex);

		// Create a pattern for the font name
		FcPattern* pattern =
		    FcNameParse(reinterpret_cast<const FcChar8*>(name.c_str()
		    ));
		if (!pattern) {
			throw IOCore::Exception(
			    "Failed to create Fontconfig pattern."
			);
		}

		// Add a filter to prioritize TrueType fonts
		// FcPatternAddString(
		//    pattern,
		//    FC_FONTFORMAT,
		//    reinterpret_cast<const FcChar8*>("TrueType")
		//);

		// Match the pattern
		FcConfig* config = FcConfigGetCurrent();
		FcResult result;
		FcPattern* matched_font =
		    FcFontMatch(config, pattern, &result);

		FcPatternDestroy(pattern); // Free the pattern

		if (!matched_font) {
			throw IOCore::Exception(
			    "No matching font found for: " + name
			);
		}

		// Get the font file path
		FcChar8* font_path = nullptr;
		if (FcPatternGetString(
			matched_font, FC_FILE, 0, &font_path
		    ) != FcResultMatch) {
			FcPatternDestroy(matched_font
			); // Free the matched pattern
			throw IOCore::Exception(
			    "Failed to retrieve font path for: " + name
			);
		}

		std::string font_file_path(
		    reinterpret_cast<const char*>(font_path)
		);
		FcPatternDestroy(matched_font);

		return font_file_path;
	}

	~FontConfig() { FcFini(); }

	FontConfig(const FontConfig&) = delete;
	auto operator=(const FontConfig&) -> FontConfig& = delete;

    private:
	FontConfig()
	{
		if (!FcInit()) {
			throw IOCore::Exception(
			    "Failed to initialize Fontconfig."
			);
		}
	}

	std::mutex access_mutex;
};

// clang-format off
// vim: set foldmethod=syntax foldminlines=10 textwidth=80 ts=8 sts=0 sw=8 noexpandtab ft=cpp.doxygen :
