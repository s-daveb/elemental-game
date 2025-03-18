/*
 * Copyright © 2024 Saul D. Beniquez
 * License: Mozilla Public License v2.0 (MPL2)
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v.2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/.
 */

#include "External/IOCore/include/TomlTable.hpp"
#include "IOCore/TomlTable.hpp"
#include "test-utils/common.hpp"
#include "types/rendering.hpp"

#include <string>

using namespace std;
using namespace elemental;

BEGIN_TEST_SUITE("rendering-types")
{
	using IOCore::TomlTable;

	TEST_CASE("Point class")
	{
		Point test_point{ 10, 10 };
		IOCore::TomlTable table;

		table = test_point;

		REQUIRE(table.size() == 2);
	}
	TEST_CASE("Test Area serialization")
	{
		// Test basic functionality
		SECTION("Basic area")
		{
			Area test_area{ 10, 20 };

			TomlTable table = test_area;

			REQUIRE(table["width"].value<uint32_t>() == 10);
			REQUIRE(table["height"].value<uint32_t>() == 20);
		}
	}

	TEST_CASE("Test Rectangle functionality")
	{
		// Test construction and properties
		SECTION("Rectangle from Position2D and Area")
		{
			Point position{ 5, 10 };
			Area size{ 200, 300 };

			Rectangle rect(position, size);

			REQUIRE(rect.position.x == 5);
			REQUIRE(rect.position.y == 10);
			REQUIRE(rect.size.width == 200);
			REQUIRE(rect.size.height == 300);
		}

		SECTION("Rectangle from constructor with unsigned integers")
		{
			Rectangle rect(5u, 10u, 200u, 300u);

			REQUIRE(rect.position.x == 5);
			REQUIRE(rect.position.y == 10);
			REQUIRE(rect.size.width == 200);
			REQUIRE(rect.size.height == 300);
		}

		SECTION("Copy assignment")
		{
			Rectangle rect1(5, 10, 200, 300);
			Rectangle rect2 = rect1;

			REQUIRE(rect2.position.x == 5);
			REQUIRE(rect2.position.y == 10);
			REQUIRE(rect2.size.width == 200);
			REQUIRE(rect2.size.height == 300);
		}

		SECTION("Equality operator")
		{
			Rectangle rect1(5, 10, 200, 300);
			Rectangle rect2(5, 10, 200, 300);

			REQUIRE(rect1 == rect2);
		}

		SECTION("Modify via references")
		{
			Rectangle rect{ 5, 10, 200, 300 };
			rect.x = 10;
			rect.y = 20;

			REQUIRE(rect.position.x == 10);
			REQUIRE(rect.position.y == 20);
		}
	}

	TEST_CASE("Test WindowParameters serialization")
	{
		SECTION("Basic window parameters")
		{
			WindowParameters params{ "My Game",
				                 WindowMode::Fullscreen,
				                 WindowPlacement::Centered,
				                 { 100, 200 },
				                 { 800, 600 } };

			TomlTable table = params;

			REQUIRE(
			    table["title"].value<std::string>() == "My Game"
			);
			REQUIRE(
			    table["mode"].value<std::string>() ==
			    "Fullscreen"
			);
			REQUIRE(
			    table["placement"].value<std::string>() ==
			    "Centered"
			);
			REQUIRE(
			    table["position"]["x"].value<uint32_t>() == 100
			);
			REQUIRE(
			    table["position"]["y"].value<uint32_t>() == 200
			);
			REQUIRE(
			    table["size"]["width"].value<uint32_t>() == 800
			);
			REQUIRE(
			    table["size"]["height"].value<uint32_t>() == 600
			);
		}
	}

	TEST_CASE("Test RendererSettings serialization")
	{
		WindowParameters params{ "My Game",
			                 WindowMode::Fullscreen,
			                 WindowPlacement::Centered,
			                 { 100, 200 },
			                 { 800, 600 } };

		RendererSettings settings{ params,
			                   Resolution{
					       0,
					       0,
					   } };

		TomlTable table = settings;

		REQUIRE(table["window"].is_table());
		CHECK(
		    table["window"]["title"].value<std::string>() ==
		    "My Game"
		);
		CHECK(
		    table["window"]["mode"].value<std::string>() ==
		    "Fullscreen"
		);
		CHECK(
		    table["window"]["placement"].value<std::string>() ==
		    "Centered"
		);
		CHECK(
		    table["window"]["position"]["x"].value<uint32_t>() == 100
		);
		CHECK(
		    table["window"]["position"]["y"].value<uint32_t>() == 200
		);
		REQUIRE(
		    table["window"]["size"]["width"].value<uint32_t>() == 800
		);
		REQUIRE(
		    table["window"]["size"]["height"].value<uint32_t>() ==
		    600
		);
	}
}

// clang-format off
// vim: set foldmethod=syntax foldminlines=10 textwidth=80 ts=8 sts=0 sw=8 noexpandtab ft=cpp.doxygen :
