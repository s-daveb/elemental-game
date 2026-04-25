# Elemental Engine Coding Standards
====================================

> **Note**: This is a living document. Update it as conventions evolve.
> Last updated: April 2026

Purpose
-------
This document establishes coding standards for the Elemental engine codebase. These standards ensure consistency across human-written and AI-generated code, improve readability, and reduce cognitive load when switching between files.

These standards complement the automated checks enforced by `.clang-format` and `.clang-tidy`. Follow both the automated rules and the human-judgment guidelines below.

Scope
-----
Applies to all C++ source code in:
- `Modules/elemental/` — Core engine code
- `Modules/EditorWidgets/` — Qt-based editor components
- `Apps/Demos/` — Demo applications (e.g., Phong, Pong)
- `Apps/Editors/` — Editor applications
- `Tests/` — Test code (with some exceptions noted below)

Does **not** apply to:
- `Modules/External/` — Third-party libraries
- Auto-generated code (e.g., Doxygen output, build artifacts)

Naming Conventions
------------------

### Classes and Structs
**Rule**: `CamelCase` starting with uppercase letter

```cpp
// ✓ CORRECT
class GameScene;
struct EntityMetadata;
class BallPositionComponent;

// ✗ INCORRECT
class gameScene;        // wrong case
struct entity_metadata; // snake_case not allowed
```

**Rationale**: Matches `.clang-tidy` rules for `ClassCase` and `StructCase`. Consistent with C++ standard library conventions.

### Interfaces (Abstract Base Classes)
**Rule**: Prefix with `I` followed by `CamelCase`

```cpp
// ✓ CORRECT
struct IComponent;
class Renderer;

// ✗ INCORRECT
struct ComponentInterface;  // too verbose
class RendererBase;         // ambiguous (could be CRTP base)
```

**Rationale**: Clear visual distinction between interfaces and concrete classes. Follows C# convention, widely recognized in C++.

### Functions and Methods
**Rule**: `lower_case` or `camelCase` for public methods

```cpp
// ✓ CORRECT (free functions)
auto createEntity(const std::string& name) -> Entity&;
void initializeRenderer();

// ✓ CORRECT (public methods - camelCase)
auto getPosition() const -> Point;
void setPosition(Point pos);

// ✓ CORRECT (private/protected methods - lower_case)
void updatePhysics();
auto calculateBounds() -> Rectangle;

// ✗ INCORRECT
auto GetPosition() -> Point;     // PascalCase
void set_position(Point pos);    // snake_case
```

**Rationale**: `.clang-tidy` enforces `PublicMethodCase: camelBack` and `PrivateMethodCase: lower_case`. Free functions use `lower_case` consistently.

#### Pointer-Processing Methods
**Rule**: Methods that accept or return pointers should end with `Ptr`

```cpp
// ✓ CORRECT
void processEntityPtr(const Entity* entity);
auto getComponentPtr() const -> const IComponent*;
void renderEntitiesPtr(const std::vector<const Entity*>& entities);
auto findEntityPtr(EntityId id) -> const Entity*;
auto createRendererPtr() -> std::unique_ptr<IRenderer>;

// ✗ INCORRECT
void processEntity(Entity* entity);    // missing Ptr suffix
auto getComponent() -> IComponent*;    // ambiguous - is it a pointer or reference?
```

**Rationale**: Makes pointer ownership and mutation semantics explicit at call sites. The `Ptr` suffix clearly signals that the function deals with pointer types, helping developers reason about ownership and nullability. Also distinguishes from reference-returning methods (`getComponent()` vs `getComponentPtr()`).

### Variables
**Rule**: `lower_case` with underscores for multi-word names

```cpp
// ✓ CORRECT
int player_score;
float ball_velocity_x;
std::vector<std::unique_ptr<Entity>> entities_ptr;

// ✗ INCORRECT
int playerScore;      // camelCase
int PlayerScore;      // PascalCase
```

**Rationale**: Matches `.clang-tidy` `VariableCase: lower_case`. Improves readability for multi-word names.

### Pointer Variables
**Rule**: Variables that hold pointers should end with `Ptr` (for raw/const pointers) or `_ptr` (for smart pointer wrappers like `SdlPtr`)

```cpp
// ✓ CORRECT
const Entity* entity_ptr;                      // raw const pointer
IComponent* component_ptr;                     // raw pointer (avoid - use reference instead)
std::unique_ptr<Texture> texture_ptr;          // smart pointer
SdlPtr<SDL_Window> window_ptr;                  // custom smart pointer wrapper

// ✗ INCORRECT
const Entity* entity;           // missing Ptr suffix
std::unique_ptr<Texture> tex;   // unclear that this is a pointer
```

**Rationale**: Makes pointer ownership and nullability explicit at declaration sites. The `Ptr` suffix matches method naming convention for consistency. Smart pointer wrappers like `SdlPtr` already use `_ptr` suffix in the existing codebase (e.g., `sdl_window_ptr` in `SdlRenderer.hpp`).

### Member Variables (CRITICAL)
**Rule**: `lower_case` — **NO trailing underscores**

```cpp
// ✓ CORRECT
class Renderer {
    bool is_initialized;
    int frame_count;
    float update_delta;
};

// ✗ INCORRECT (DO NOT USE)
class Renderer {
    bool is_initialized_;     // WRONG - trailing underscore
    int frame_count_;
    float update_delta_;
};
```

**Rationale**: The existing codebase (Phong.cpp, LoopRegulator.hpp, SdlRenderer.hpp) does **not** use trailing underscores. This convention is common in Google-style guides but **not** used in this project.

**Examples from existing code**:
```cpp
// Phong.hpp
bool is_running;
IRenderer& video_renderer;
StateStack state_stack;

// LoopRegulator.hpp
uint32_t desired_rate_per_second;
steady_clock::time_point start_time;
milliseconds elapsed_ms;

// SdlRenderer.hpp
bool is_initialized;
SdlPtr<SDL_Window> sdl_window_ptr;
```

### Constants and Constexpr
**Rule**: Prefix with `k` followed by `CamelCase`

```cpp
// ✓ CORRECT
constexpr int kMaxEntities = 1024;
constexpr float kPi = 3.14159f;
constexpr auto kDefaultFramerate = 60_Hz;

// ✗ INCORRECT
constexpr int MAX_ENTITIES = 1024;  // ALL_CAPS (reserved for macros)
constexpr int maxEntities = 1024;   // missing k prefix
```

**Rationale**: Matches `.clang-tidy` `ConstantPrefix: k` and `ConstantCase: CamelCase`. Distinguishes constants from variables while avoiding ALL_CAPS (which is reserved for preprocessor macros).

### Template Parameters
**Rule**: Prefix with `T` followed by `CamelCase`

```cpp
// ✓ CORRECT
template<typename TComponent>
template<typename T, std::size_t Capacity>
template<typename TArgs...>

// ✓ ALSO ACCEPTABLE (single letter)
template<typename T>
template<typename U>

// ✗ INCORRECT
template<typename ComponentType>  // too verbose
template<typename t>              // lowercase
```

**Rationale**: Matches `.clang-tidy` `TypeTemplateParameterPrefix: T`. Clear visual distinction from concrete types.

### Type Aliases
**Rule**: `CamelCase` with optional `_t` suffix for clarity

```cpp
// ✓ CORRECT
using EntityId = uint32_t;
using ComponentFactoryRef = std::reference_wrapper<IComponentFactory>;
using TypeInfo = std::type_index;

// ✓ ALSO ACCEPTABLE (with _t suffix)
using EntityId_t = uint32_t;

// ✗ INCORRECT
using entity_id = uint32_t;      // snake_case
using ENTITY_ID = uint32_t;      // ALL_CAPS
```

**Rationale**: Matches `.clang-tidy` `TypeAliasCase: CamelCase`. `_t` suffix is optional but can improve clarity for primitive type aliases.

### Enum Values
**Rule**: `CamelCase` (no prefix)

```cpp
// ✓ CORRECT
enum class ShapeType : uint8_t {
    FilledCircle,
    FilledRectangle,
    OutlineCircle
};

// ✗ INCORRECT
enum class ShapeType : uint8_t {
    filledCircle,      // camelCase
    FILLED_CIRCLE,     // ALL_CAPS
    kFilledCircle      // k-prefix
};
```

**Rationale**: Matches `.clang-tidy` `EnumConstantCase: CamelCase`. `enum class` provides strong typing, so prefixes are unnecessary.

### File Names
**Rule**: Match the primary class/interface name with `.hpp` or `.cpp` extension

```cpp
// ✓ CORRECT
BallPositionComponent.hpp
GameScene.cpp
IComponent.hpp

// ✗ INCORRECT
ball_position_component.hpp  // snake_case
Ballpositioncomponent.hpp    // missing separators
```

**Rationale**: Makes file discovery intuitive. Matches existing project structure.

### Namespaces
**Rule**: `lower_case` single word

```cpp
// ✓ CORRECT
namespace elemental {
namespace util {
namespace sys {

// ✗ INCORRECT
namespace Elemental {     // PascalCase
namespace element_lib {   // underscore (use nested namespaces)
```

**Rationale**: Matches C++ standard library convention (`std`, `fs`, `chrono`). Use nested namespaces for hierarchy: `namespace elemental::util::debug`.

Code Layout
-----------

### Indentation and Spacing
**Rule**: Follow `.clang-format` automatically

Key settings:
- **Indent width**: 8 spaces (use tabs)
- **Column limit**: 80 characters
- **Pointer alignment**: Left (`int* ptr`, not `int *ptr`)
- **Namespace indentation**: Inner namespaces are indented

**Action**: Run `clang-format` before committing. Configure your editor to format on save.

### Include Order
**Rule**: Grouped and sorted by category (enforced by `.clang-format`)

```cpp
// 1. Test utilities (if applicable)
#include "test-utils/common.hpp"

// 2. Internal utilities
#include "util/debug.hpp"
#include "util/testing.hpp"

// 3. IOCore
#include "IOCore/Application.hpp"
#include "IOCore/types.hpp"

// 4. Elemental (core engine)
#include "elemental/Entity.hpp"
#include "elemental/IComponent.hpp"

// 5. Qt (if applicable)
#include "QWidget"

// 6. Other quoted includes
#include "nonstd/span.hpp"

// 7. Qt angle-bracket includes
#include <QApplication>

// 8. C system headers
#include <stdlib.h>

// 9. C++ STL
#include <memory>
#include <vector>
```

**Rationale**: Improves dependency visibility. Catches missing includes early.

### Class Layout
**Rule**: Order: `public` → `protected` → `private`

```cpp
class GameScene : public IState
{
    public:
    // Constructors
    explicit GameScene(const SceneConfig& config);
    ~GameScene() override = default;

    // Deleted copy/move (if non-copyable)
    GameScene(const GameScene&) = delete;
    auto operator=(const GameScene&) -> GameScene& = delete;

    // Public interface methods
    auto step() -> void override;
    auto getDrawCommandsPtr()
        -> std::list<std::shared_ptr<IDrawCommand>> override;

    // Public data members (rare, use sparingly)
    EntityId id{ 0 };

    protected:
    // Protected methods (for derived classes)
    virtual auto onUpdate() -> void;

    // Protected data members
    SceneConfig config;

    private:
    // Private helper methods
    void updateEntities();
    auto syncViews() -> void;

    // Private data members
    ComponentFactory<CircleViewComponent> circle_factory;
    std::unordered_map<EntityId, Entity> entities;
};
```

**Rationale**: Most-commonly-used (public interface) first. Implementation details (private) last.

### Function Order in Classes
**Rule**: Within each access level, order:
1. Constructors/destructors
2. Deleted copy/move operations
3. Override methods (from base classes)
4. Public interface methods
5. Protected methods
6. Private helper methods

**Rationale**: Groups related functionality. Makes class capabilities clear at a glance.

### Braces and Control Flow
**Rule**: Follow `.clang-format` (Mozilla style)

```cpp
// ✓ CORRECT (Mozilla brace style)
void function()
{
    if (condition) {
        doSomething();
    } else {
        doOtherThing();
    }
}

// Short forms (allowed by .clang-format)
if (condition) return early();

for (int i = 0; i < 10; ++i) process(i);
```

**Rationale**: `.clang-format` enforces consistency. Short forms acceptable for simple single-statement blocks.

### Return Type Syntax
**Rule**: Trailing return type (enforced by `.clang-tidy`)

```cpp
// ✓ CORRECT
auto getPosition() const -> Point;
auto createEntity(const std::string& name) -> Entity&;

// ✗ INCORRECT
Point getPosition() const;
Entity& createEntity(const std::string& name);
```

**Rationale**: Matches `.clang-tidy` `modernize-use-trailing-return-type`. Improves consistency with lambdas and complex return types.

Comments and Documentation
--------------------------

### File Headers
**Rule**: Include copyright and license at top of every file

```cpp
/* BallPositionComponent.hpp
 * Copyright © 2026 Saul D. Beniquez
 * License: Mozilla Public License v. 2.0
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v.2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/.
 */
```

**Rationale**: Legal requirement for MPL 2.0. Clearly states licensing terms.

### Doxygen Comments
**Rule**: Use for public APIs, classes, and non-obvious functions

```cpp
/// \brief Manages entity-component lifecycle
///
/// GameScene provides the primary interface for creating, updating,
/// and rendering entities in a game scene.
///
/// \tparam TComponent The component type to manage
class GameScene : public IState
{
    /// \brief Update game logic
    /// \param dt Delta time in seconds
    virtual auto onUpdate(float dt) -> void = 0;
};
```

**Rationale**: Generates Doxygen documentation. Helps developers understand API contracts.

### Implementation Comments
**Rule**: Explain **why**, not **what**

```cpp
// ✓ CORRECT (explains reasoning)
// Use 1/60s fixed timestep for deterministic physics
static constexpr float kDt = 1.0f / 60.0f;

// ✓ CORRECT (explains workaround)
// SDL2 requires texture recreation on context loss
if (texture_lost) { recreateTexture(); }

// ✗ INCORRECT (states obvious)
i++;  // increment i
```

**Rationale**: Code shows what it does. Comments should explain intent, constraints, or non-obvious decisions.

### FIXME and TODO Comments
**Rule**: Use standardized format

```cpp
// FIXME: Memory leak when scene is popped during render
// TODO: Add support for sprite sheets
// HACK: Temporary workaround for SDL2 bug #4521
// NOTE: This function is called from the render thread
```

**Rationale**: Makes technical debt visible. Searchable in code reviews.

Error Handling
--------------

### Exceptions
**Rule**: Use for unrecoverable errors only

```cpp
// ✓ CORRECT
if (factory_not_found) {
    throw std::runtime_error("ComponentFactory not registered");
}

// ✗ INCORRECT (don't use exceptions for control flow)
try {
    entity = getEntity(id);
} catch (...) {
    entity = createEntity();
}
```

**Rationale**: Exceptions are for exceptional conditions. Use return values or `std::optional` for expected failures.

### Error Returns
**Rule**: Use `std::optional` or `ErrorFlag` for recoverable errors

```cpp
// ✓ CORRECT
auto loadTexture(const std::string& path) -> std::optional<Texture>;
auto initialize() -> ErrorFlag;

// Usage
if (auto texture = loadTexture("sprite.png")) {
    useTexture(*texture);
} else {
    handleLoadFailure();
}
```

**Rationale**: Makes error handling explicit. Caller must check result.

### Assertions
**Rule**: Use for internal invariants, not input validation

```cpp
// ✓ CORRECT (invariant check)
assert(entity_id != 0 && "Entity ID must be non-zero");

// ✗ INCORRECT (input validation)
assert(ptr != nullptr);  // Use if (ptr) instead
```

**Rationale**: Assertions are disabled in release builds. Use for catching programmer errors, not user errors.

Memory Management
-----------------

### Smart Pointers
**Rule**: Use smart pointers for all owning references. Prefer `std::unique_ptr` for exclusive ownership, `std::shared_ptr` for shared ownership.

```cpp
// ✓ CORRECT
auto entity_ptr = std::make_unique<Entity>();
auto texture_ptr = std::make_shared<Texture>();

// ✓ CORRECT (factory return)
auto createEntityPtr() -> std::unique_ptr<Entity>;

// ✗ INCORRECT
Entity* entity = new Entity();  // raw new (manual delete required)
std::unique_ptr<Entity> e(entity);  // constructing from raw (use make_unique)
```

**Rationale**: Smart pointers automate memory management via RAII. `std::make_unique` and `std::make_shared` are exception-safe and clearer than raw `new`. Manual `delete` should never appear in application code.

### Raw Pointers
**Rule**: Avoid raw pointers. Use only for non-owning observation of const data or in low-level memory management code.

```cpp
// ✓ CORRECT (const observation - acceptable)
void processEntityPtr(const Entity* entity);
auto getComponentPtr() const -> const IComponent*;

// ✓ CORRECT (low-level memory management - rare)
template<typename T>
auto allocatePtr() -> T*;  // In memory pool implementation only

// ✗ INCORRECT (non-const raw pointer - use reference or smart pointer)
void processEntity(Entity* entity);  // missing Ptr suffix

// ✗ INCORRECT (owning raw pointer - use smart pointer)
Entity* entity;  // Who owns this? Who deletes it?
```

**Rationale**: Raw pointers to non-const data often indicate ownership ambiguity or potential mutation issues. Prefer:
- `std::unique_ptr<T>` for exclusive ownership
- `std::shared_ptr<T>` for shared ownership
- `T&` for required non-null references
- `const T*` for optional observation without mutation
- `std::span<T>` for array views

Manual `delete` should never appear in application code outside of custom allocators or memory pool implementations.

### References vs Pointers
**Rule**: Prefer references for required parameters, smart pointers for optional owning references, const pointers for optional observation.

```cpp
// ✓ CORRECT
void processEntity(Entity& entity);              // required (non-null) - reference, not pointer
void processEntityPtr(std::unique_ptr<Entity> e);   // takes ownership
auto getEntityPtr() -> const Entity*;               // optional observation (may be nullptr)
auto getEntityRef() -> IOCore::optional_ref<Entity>;    // optional reference

// ✗ INCORRECT
void processEntity(Entity* entity);  // raw non-const pointer (use reference or smart pointer)
```

**Rationale**: References cannot be null, making intent clear. Smart pointers express ownership semantics. Const raw pointers are acceptable for non-owning observation where nullptr is a valid "not found" result.

### Reference Wrapper Patterns for ECS Components
**Rule**: For component access in game scenes, use `IOCore::optional_ref` for optional references, and `std::reference_wrapper` directly for non-nullable references. Avoid raw pointers.

The Elemental ECS uses `IOCore::optional_ref` to provide pointer-like access to optional references with clean semantics. It wraps `std::optional<std::reference_wrapper<T>>` with pointer-style operators (`->` and `*`).

```cpp
// ✓ CORRECT - Direct reference when component always exists
std::reference_wrapper<BallPositionComponent> ball_pos_ref;

// ✓ CORRECT - Optional reference when entity might not have component
//             Use IOCore::optional_ref for pointer-like semantics
IOCore::optional_ref<IViewComponent> ball_view_ref;

// ✗ INCORRECT - Raw pointers for component access
const BallPositionComponent* ball_pos_ptr;

// ✗ INCORRECT - std::optional<std::reference_wrapper> (use IOCore::optional_ref instead)
std::optional<std::reference_wrapper<IViewComponent>> ball_view_ref;
```

**Accessing values**:
```cpp
// Direct reference: use .get() to access the referenced object
ball_pos_ref.get().setX(new_x);

// IOCore::optional_ref: use -> for pointer-style access, or * for dereference
ball_view_ref->setPosition({x, y});  // pointer-like access via operator->
(*ball_view_ref).setColor({255, 0, 0});  // dereference with operator*

// Check with has_value() or implicit bool
if (ball_view_ref) {
    ball_view_ref->setPosition({x, y});
}
```

**Construction and assignment**:
```cpp
// Direct assignment from lvalue reference
ball_pos_ref = some_ball_component;
ball_view_ref = entity.views_ref[0].get();  // extract ref from wrapper first

// Check before access
if (ball_view_ref.has_value()) {
    ball_view_ref->setPosition({x, y});
}
```

**Registering factories with ComponentPool**:
When registering a factory that is a member variable, use `std::ref()` to create the reference_wrapper:
```cpp
auto& pool = ComponentPool::getInstance();
pool.registerFactory<BallPositionComponent>(std::ref(ball_pos_factory));
```

**Rationale**: `IOCore::optional_ref` provides:
- Pointer-like operators (`->` and `*`) for clean syntax
- Prevents binding to temporaries at compile time
- Implicit conversion to `optional_ref<const T>` for const access
- Type safety through the type system
- Integration with `std::optional` semantics (has_value, operator bool)

Testing Standards
-----------------

### Test File Naming
**Rule**: Match source file with `.test.cpp` suffix

```cpp
// Source: Modules/elemental/Entity.hpp
// Test:   Tests/Entity.test.cpp

// Source: Modules/elemental/ComponentFactory.hpp
// Test:   Tests/ComponentFactory.test.cpp
```

### Test Case Naming
**Rule**: Describe behavior being tested

```cpp
// ✓ CORRECT
TEST_CASE("ComponentFactory creates components with default values", "[component]")
TEST_CASE("ComponentPool throws when factory not registered", "[pool]")

// ✗ INCORRECT
TEST_CASE("Test 1", "[component]")
TEST_CASE("Basic test", "[pool]")
```

### Test Structure
**Rule**: Arrange-Act-Assert pattern

```cpp
TEST_CASE("ComponentFactory tracks created components", "[factory]")
{
    // Arrange
    ComponentFactory<TestComponent> factory;

    // Act
    auto& component = factory.create();

    // Assert
    REQUIRE(factory.getSize() == 1);
    REQUIRE_FALSE(factory.empty());
}
```

AI-Specific Guidelines
----------------------

### When Generating Code
**Checklist for AI-generated code**:
1. ✓ Naming matches existing conventions (NO trailing underscores on members)
2. ✓ Includes file header with copyright/license
3. ✓ Uses trailing return types
4. ✓ Follows include order
5. ✓ Uses smart pointers, not raw `new`
6. ✓ Constants use `k` prefix
7. ✓ Member variables match existing style (e.g., `is_running`, not `is_running_`)
8. ✓ Pointer-processing methods end with `Ptr` (e.g., `processEntityPtr`, not `processEntity`)
9. ✓ Pointer variables end with `Ptr` or `_ptr` (e.g., `entity_ptr`, `texture_ptr`, `window_ptr`)

### Common AI Mistakes to Avoid
```cpp
// ✗ WRONG (trailing underscore - Google style, not ours)
class Component {
    float x_;
    float y_;
};

// ✓ CORRECT (matches existing codebase)
class Component {
    float x;
    float y;
};

// ✗ WRONG (ALL_CAPS constant - looks like macro)
constexpr int MAX_SIZE = 100;

// ✓ CORRECT
constexpr int kMaxSize = 100;

// ✗ WRONG (PascalCase method)
auto GetPosition() -> Point;

// ✓ CORRECT
auto getPosition() -> Point;
```

### Review Process for AI Code
1. Run `clang-format` to fix formatting
2. Run `clang-tidy` to catch naming violations
3. Manually review:
   - Member variable naming (no trailing underscores)
   - Constant naming (k-prefix, not ALL_CAPS)
   - Include order
   - File header present
4. Compare against similar existing files (e.g., `Phong.hpp`, `LoopRegulator.hpp`)

Quick Reference Card
--------------------

| Construct | Convention | Example |
|-----------|------------|---------|
| Class/Struct | `CamelCase` | `GameScene`, `EntityMetadata` |
| Interface | `I` + `CamelCase` | `IComponent`, `IRenderer` |
| Function (free) | `lowerCase` | `createEntity`, `initializeRenderer` |
| Method (public) | `camelCase` | `getPosition`, `setPosition` |
| Method (private) | `lower_case` | `updatePhysics`, `calculateBounds` |
| Method (pointer) | `Ptr` suffix | `processEntityPtr`, `getComponentPtr` |
| Variable | `lower_case` | `player_score`, `ball_velocity` |
| Variable (pointer) | `Ptr` or `_ptr` suffix | `entity_ptr`, `texture_ptr`, `window_ptr` |
| **Member Variable** | **`lower_case` (NO trailing `_`)** | **`is_running`, `frame_count`** |
| Constant | `k` + `CamelCase` | `kMaxEntities`, `kPi` |
| Template Param | `T` + `CamelCase` | `TComponent`, `TArgs` |
| Type Alias | `CamelCase` | `EntityId`, `ComponentFactoryRef` |
| Enum Value | `CamelCase` | `FilledCircle`, `OutlineRectangle` |
| Namespace | `lowerCase` | `elemental`, `util`, `sys` |
| File Name | Match class | `BallPositionComponent.hpp` |

Enforcement
-----------
- **Automated**: `.clang-format` and `.clang-tidy` run in CI
- **Manual**: Code review checks for conventions not enforced by tools
- **AI-generated code**: Must pass same standards as human code

When in Doubt
-------------
1. Look at similar existing code (e.g., `Phong.hpp`, `LoopRegulator.hpp`)
2. Run `clang-format` and `clang-tidy`
3. Ask in code review
4. Update this document if convention is unclear
