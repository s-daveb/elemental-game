/* ComponentFactory.test.cpp
 * Copyright © 2024 Saul D. Beniquez
 * License: Mozilla Public License v. 2.0
 */

#include "test-utils/common.hpp"

#include "ComponentFactory.hpp"

namespace {
using namespace elemental;

class TestComponent : public IComponent
{
	friend class ComponentFactory<TestComponent>;
	IComponentFactory& factory_;
	unsigned int       instance_id_{ 0 };

    public:
	TestComponent(IComponentFactory& f, unsigned int id = 0)
	    : factory_(f), instance_id_(id)
	{
	}
	~TestComponent() override = default;

	auto getInstanceId() const -> InstanceID override
	{ return instance_id_; }
	auto getTypeIndex() const -> TypeInfo override
	{ return typeid(TestComponent); }
	auto getFactory() const -> IComponentFactory& override
	{ return factory_; }
};

}  // namespace

BEGIN_TEST_SUITE("elemental::ComponentFactory")
{
	using namespace elemental;

	TEST_CASE("Basic ComponentFactory construction")
	{
		ComponentFactory<TestComponent> factory;
		REQUIRE(factory.empty());
		REQUIRE(factory.getSize() == 0);
		REQUIRE(factory.capacity() == 256);
	}

	TEST_CASE("ComponentFactory::create returns reference to component")
	{
		ComponentFactory<TestComponent> factory;

		auto& comp = factory.create();

		REQUIRE(&comp != nullptr);
		REQUIRE(!factory.empty());
		REQUIRE(factory.getSize() == 1);
	}

	TEST_CASE("ComponentFactory::get retrieves created component by index")
	{
		ComponentFactory<TestComponent> factory;

		auto& comp = factory.create();

		REQUIRE_NOTHROW([&]() -> void {
			auto& retrieved = factory.get(0);
			REQUIRE(&comp == &retrieved);
		});
	}

	TEST_CASE("ComponentFactory::get throws on invalid index")
	{
		ComponentFactory<TestComponent> factory;

		REQUIRE_THROWS_AS([&]() -> void { factory.get(0); }(),
		                  std::out_of_range);
	}

	TEST_CASE("ComponentFactory::clear resets the component pool")
	{
		ComponentFactory<TestComponent> factory;

		factory.create();
		REQUIRE(factory.getSize() == 1);

		factory.clear();
		REQUIRE(factory.empty());
		REQUIRE(factory.getSize() == 0);
	}

	TEST_CASE(
	    "ComponentFactory stores factory reference in created components")
	{
		ComponentFactory<TestComponent> factory;

		auto& comp = factory.create();

		REQUIRE(&comp.getFactory() == &factory);
	}
}
