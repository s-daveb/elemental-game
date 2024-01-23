/* JsonConfigFile.test.cpp
 * }
 * Copyright © 2024 Saul D. Beniquez
 * License: Mozilla Public License v. 2.0
 *
 * This Source Code Form is subject to the terms of the Mozilla Public License,
 * v.2.0. If a copy of the MPL was not distributed with this file, You can
 * obtain one at https://mozilla.org/MPL/2.0/.
 */

#include "JsonConfigFile.hpp"

#include "FileResource.hpp"
#include "types/configuration.hpp"

#include "private/debuginfo.hpp"
#include "test-utils/common.hpp"
#include "types/legible_ctypes.hpp"

#include "Exception.hpp"

#include <filesystem>
#include <fstream>
#include <iostream>
#include <string>

#include <nlohmann/json.hpp>

using namespace elemental;
namespace fs = std::filesystem;

constexpr c::const_string INPUT_FILE_PATH = "/tmp/test_config.json";
constexpr c::const_string BADFILE_PATH = "/tmp/test_bad_config.json";
constexpr c::const_string NON_EXISTENT_PATH = "/hades/tmp/dne/file.json";

BEGIN_TEST_SUITE("elemental::JsonConfigFile")
{
	namespace { // Test fixtures
	struct SampleFileGenerator
	{
		SampleFileGenerator()
		{
			if (!fs::exists(INPUT_FILE_PATH)) {
				std::ofstream f(INPUT_FILE_PATH,
				                std::ios::out |
				                    std::ios::trunc);
				f << R"({"key1": "value1", "key2": "value2"})"
				  << std::endl;
				f.close();
			}
		}
		~SampleFileGenerator()
		{
			try {
				if (fs::exists(INPUT_FILE_PATH)) {
					fs::remove(INPUT_FILE_PATH);
				}
			} catch (std::exception& e) {
				debugprint(e.what());
			}
		}
	};
	using TestFixture = SampleFileGenerator;
	}

	TEST("elemental::configuration::dictionary is serializablable like "
	     "std::map<std::string,std::string>")
	{
		configuration::dictionary test_data;
		nlohmann::json j_object;
		test_data["one"] = "1";
		test_data["resolution"] = "1280x720";
		test_data["Hello"] = "world";

		REQUIRE_NOTHROW([&]() { j_object = test_data; }());

		REQUIRE(test_data["one"] == j_object["one"]);
		REQUIRE(test_data["resolution"] == j_object["resolution"]);
		REQUIRE(test_data["Hello"] == j_object["Hello"]);
	}

	FIXTURE_TEST("JsonConfigFile construction")
	{
		SECTION("JsonConfigFile w/ valid path")
		{
			auto config = JsonConfigFile(INPUT_FILE_PATH);
			auto& config_data = config.GetData();

			REQUIRE(config_data.empty());
		}
		SECTION("JsonConfigFile w/ invalid path throws exception")
		{

			REQUIRE_THROWS_AS(JsonConfigFile(NON_EXISTENT_PATH),
			                  elemental::UnreachablePathException);
		}
	}
	FIXTURE_TEST("JsonConfigFile container wrappers and accessors")
	{
		auto config = JsonConfigFile(INPUT_FILE_PATH);

		SECTION("GetConfiguration() and the operator[] are working")
		{
			auto& internal_data = config.GetData();
			internal_data["key1"] = "test";

			REQUIRE(config["key1"] == internal_data["key1"]);

			SECTION("The operator[] returns a mutable reference "
			        "and allows object creation")
			{
				internal_data["key1"] = "test";

				// key 2 should not exist yet
				REQUIRE(internal_data.find("key2") ==
				        internal_data.end());
				config["key2"] = "test2";

				// key2 exists now and should have a particular
				// value
				REQUIRE(internal_data.find("key2") !=
				        internal_data.end());
				REQUIRE(config["key2"] ==
				        internal_data["key2"]);
				REQUIRE(config["key2"] == "test2");
			}
			SECTION(".at() operator throws exceptions when the key"
			        " does not exist")
			{
				REQUIRE(internal_data.at("key1") ==
				        internal_data["key1"]);

				REQUIRE_THROWS([&]() { config.at("key3"); }());
			}
		}
	}

	FIXTURE_TEST("JsonConfigFile::Read")
	{
		auto config = JsonConfigFile(INPUT_FILE_PATH);
		auto& internal_data = config.GetData();

		SECTION("JsonConfigFile::Read w/ valid file")
		{
			config.Read();
			REQUIRE_FALSE(internal_data.empty());
			REQUIRE(internal_data.size() == 2);
			REQUIRE(config["key1"] == "value1");
			REQUIRE(config["key2"] == "value2");
		}
		SECTION("JsonConfigFile::Read w/ bad file throws exception")
		{
			if (!fs::exists(BADFILE_PATH)) {
				std::ofstream f(BADFILE_PATH);
				f << R"({"Hello World"})" << std::endl;
				f.close();
			}
			REQUIRE_THROWS_AS(
			    [&]() {
				    auto bad_config =
					JsonConfigFile(BADFILE_PATH);

				    bad_config.Read();
			    }(),
			    Exception);
		}
		if (fs::exists(BADFILE_PATH)) {
			fs::remove(BADFILE_PATH);
		}
	}
	TEST("JsonConfigFile::Write()")
	{
		SECTION("Create File and Read It back In")
		{
			auto test_data = JsonConfigFile(INPUT_FILE_PATH);
			test_data["one"] = "1";
			test_data["resolution"] = "1280x720";
			test_data["Hello"] = "world";

			test_data.Write();

			std::ifstream resulting_file(INPUT_FILE_PATH);
			nlohmann::json jobject;

			resulting_file >> jobject;
			auto written_data =
			    jobject.get<configuration::dictionary>();

			REQUIRE(written_data["one"] == test_data["one"]);
			REQUIRE(written_data["resolution"] ==
			        test_data["resolution"]);
			REQUIRE(written_data["Hello"] == test_data["Hello"]);
		}
		fs::remove(INPUT_FILE_PATH);
	}
}
// clang-format off
// vim: set foldmethod=syntax textwidth=80 ts=8 sts=0 sw=8 foldlevel=99 noexpandtab ft=cpp.doxygen :
