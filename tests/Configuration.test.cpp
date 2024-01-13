/* Configuration.test.cpp
 * Copyright © 2024 Saul D. Beniquez
 * License: Mozilla Public License v. 2.0
 *
 * This Source Code Form is subject to the terms of the Mozilla Public License,
 * v.2.0. If a copy of the MPL was not distributed with this file, You can
 * obtain one at https://mozilla.org/MPL/2.0/.
 */

#include "Configuration.hpp"
#include "Exception.hpp"
#include "test-utils/common.hpp"

#include <filesystem>
#include <fstream>
#include <iostream>
#include <string>

#include "private/debuginfo.hpp"

#include "type-aliases.hpp"

using namespace elemental;
namespace fs = std::filesystem;

constexpr c::const_string TEST_FILE_PATH = "/tmp/test_config.json";
constexpr c::const_string INVALID_FILE_PATH = "/tmp/test_bad_config.json";

BEGIN_TEST_SUITE("elemental::Configuration")
{

	struct SampleFileGenerator
	{
		SampleFileGenerator()
		{
			if (!fs::exists(TEST_FILE_PATH)) {
				std::ofstream f(TEST_FILE_PATH);
				f << R"({"key1": "value1", "key2": "value2"})"
				  << std::endl;
				f.close();
			}
		}
		~SampleFileGenerator()
		{
			try {
				if (!fs::exists(TEST_FILE_PATH)) {
					fs::remove(TEST_FILE_PATH);
				}
			} catch (std::exception& e) {
				debugprint(e.what());
			}
		}
	};
	using TestFixture = SampleFileGenerator;

	FIXTURE_TEST("Constructors")
	{
		SECTION("Empty Constructor, empty file_path, no stored data")
		{
			auto config = Configuration();

			REQUIRE(config.FilePath().empty());
			REQUIRE(config.empty());
		}
		SECTION("Parametrized constructor with valid file")
		{
			auto config = Configuration(TEST_FILE_PATH);

			REQUIRE_FALSE(config.FilePath().empty());
			REQUIRE_FALSE(config.empty());
		}
		SECTION(
		    "Parametrized constructor w/ invalid path throws exception")
		{

			REQUIRE_THROWS_AS(
			    [&]() {
				    auto config =
					Configuration("/tmp/does-not-exist");
			    }(),
			    Exception);
		}
		SECTION("Parametrized constructor w/ bad file throws exception")
		{

			if (!fs::exists(INVALID_FILE_PATH)) {
				std::ofstream f(INVALID_FILE_PATH);
				f << R"({"Hello World"})" << std::endl;
				f.close();
			}

			REQUIRE_THROWS_AS(
			    [&]() {
				    auto config =
					Configuration(INVALID_FILE_PATH);
			    }(),
			    Exception);
		}
	}
	FIXTURE_TEST("Configuration::LoadFile")
	{
		auto config = Configuration();
		SECTION("Configuration::LoadFile w/ valid file")
		{
			config.LoadConfig(TEST_FILE_PATH);

			REQUIRE_FALSE(config.empty());
			REQUIRE(config.size() == 2);
			REQUIRE(config["key1"] == "value1");
			REQUIRE(config["key2"] == "value2");
		}
		SECTION(
		    "Configuration::LoadFile w/ invalid path throws exception")
		{

			REQUIRE_THROWS_AS(
			    [&]() {
				    config.LoadConfig("/tmp/does-not-exists");
			    }(),
			    Exception);
		}
		SECTION("Configuration::LoadFile w/ bad file throws exception")
		{
			if (!fs::exists(INVALID_FILE_PATH)) {
				std::ofstream f(INVALID_FILE_PATH);
				f << R"({"Hello World"})" << std::endl;
				f.close();
			}

			REQUIRE_THROWS_AS(
			    [&]() { config.LoadConfig(INVALID_FILE_PATH); }(),
			    Exception);
		}
		SECTION("Configuration::LoadFile w/ no file path specified")
		{

			REQUIRE_THROWS_AS([&]() { config.LoadConfig(); }(),
			                  Exception);
		}
	}
	FIXTURE_TEST("LoadConfig dumps old data and reloads new file")
	{
		auto config = Configuration();
		config["bad_data"] = "false";

		config.LoadConfig(TEST_FILE_PATH);
		REQUIRE(config.size() > 1);
		REQUIRE_THROWS(config.at("bad_data"));
	}

} // END_TEST_SUITE("elemental::Configuration")

// clang-format off
// vim: set foldmethod=syntax textwidth=80 ts=8 sts=0 sw=8 foldlevel=99 noexpandtab ft=cpp.doxygen :
