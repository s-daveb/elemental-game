/*
 * Copyright © 2024 Saul D. Beniquez
 * License: Mozilla Public License v. 2.0
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v.2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/.
 */

#include "ComponentFactory.hpp"
#include "test-utils/common.hpp"

namespace {
using namespace elemental;

class MockComponent : public Component {
    public:
	using Base = Component;

	auto getTypeIndex() -> TypeInfo override
	{
		return typeid(MockComponent);
	}

	MockComponent(ComponentFactory& owner) : Component(owner) {}
	~MockComponent() override = default;
};

} // namespace

BEGIN_TEST_SUITE("elemental::ComponentFactory")
{
	using namespace elemental;

	TEST_CASE("Basic ComponentFactory construction")
	{
		ComponentFactory factory;
	}

	TEST_CASE("ComponentFactory::createComponent")
	{
		ComponentFactory factory;

		auto result = factory.createComponent<MockComponent>();
		REQUIRE(result != nullptr);
	}

	TEST_CASE("ComponentFactory::getComponentVector")
	{
		ComponentFactory factory;

		auto new_component =
		    factory.createComponent<MockComponent>();

		REQUIRE_NOTHROW([&]() {
			auto result =
			    factory.getComponentVector<MockComponent>();
			REQUIRE(result.size() == 1);
		}());
	}

	TEST_CASE("ComponentFactory::getComponent")
	{
		ComponentFactory factory;

		REQUIRE_THROWS_AS(
		    [&]() { factory.getComponent<MockComponent>(0x0); }(),
		    std::out_of_range
		);
	}
}

// clang-format off
// vim: set  textwidth=80 ts=8 sts=0 sw=8 noexpandtab ft=cpp.doxygen :
