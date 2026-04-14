/* Component.test.cpp
 * Copyright © 2024 Saul D. Beniquez
 * License: Mozilla Public License v. 2.0
 */

#include "test-utils/common.hpp"

#include "util/testing.hpp"

#include "ComponentFactory.hpp"
#include "IComponent.hpp"

namespace {
using namespace elemental;

class TestComponent : public IComponent
{
	friend class ComponentFactory<TestComponent>;

	unsigned int       instance_id_{ 0 };
	IComponentFactory& factory_;

    public:
	TestComponent(IComponentFactory& f, unsigned int id = 0)
	    : instance_id_(id), factory_(f)
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

TEST_CASE("Test IComponent Base Interface", "[component]")
{
	using namespace elemental;

	ComponentFactory<TestComponent> factory;
	auto&                           comp = factory.create();

	REQUIRE(&comp != nullptr);
	REQUIRE(&comp.getFactory() == &factory);
	REQUIRE(comp.getInstanceId() >= 0);
}
