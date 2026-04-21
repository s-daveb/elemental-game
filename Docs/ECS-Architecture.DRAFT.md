# Elemental Engine ECS Architecture Design Doc
===============================================

> **Note**: This is a living document and will be updated as the architecture evolves.
> Last updated: April 2026

Context and motivation
----------------------
The Elemental engine uses a lightweight Entity-Component-System (ECS) architecture to enable flexible, data-driven game object composition. This design emerged from the need to support simple 2D games like Pong and Snake while maintaining clean separation between game logic, state, and rendering.

The current implementation prioritizes simplicity and ease-of-use over maximum performance, making it ideal for:
- Rapid prototyping of 2D game mechanics
- Educational purposes for understanding ECS patterns
- Small to medium-scale games with hundreds (not millions) of entities

Goals:
- Provide a simple, type-safe component model with compile-time checking
- Enable runtime component composition without complex inheritance hierarchies
- Support efficient component access through type-specific factories
- Integrate cleanly with the rendering pipeline via view components
- Allow game scenes to directly manipulate component data for game logic

Non-goals for first implementation (v1):
- Multi-threaded system execution or parallel processing
- Automatic system discovery or registration
- Component archetypes or sparse set storage
- Serialization/deserialization of entity-component graphs
- Visual entity-editor integration

Implementation considerations
-----------------------------
- **Identity-based Entities**: Entities are simple ID-backed containers holding references to components, not the components themselves
- **Compile-time Component Types**: Each component type requires explicit factory registration; no runtime type discovery
- **Fixed-capacity Factories**: ComponentFactory uses `std::array` with compile-time capacity (default 256) for simplicity
- **Singleton ComponentPool**: Global registry ensures single point of factory access across the application
- **Direct Component Access**: Game scenes hold direct pointers to components for performance-critical update loops
- **View Component Pattern**: Rendering is decoupled via IViewComponent interface that produces draw commands

High-level behavior
-------------------
The ECS operates in three phases each frame: component creation/registration, game logic update, and rendering synchronization.

```
 ┌──────────────────────────────────────────────────────────────────┐
 │                        GameScene (PongScene)                     │
 │  - Holds ComponentFactory instances (BallPos, PaddlePos, etc.)   │
 │  - Direct pointers to active components (ball_pos_, etc.)        │
 │  - onUpdate() loop: read components → compute → write back       │
 └────────────────────────┬─────────────────────────────────────────┘
                          │
                          │ registers factories with
                          v
 ┌──────────────────────────────────────────────────────────────────┐
 │                      ComponentPool (Singleton)                   │
 │  std::unordered_map<type_index, IComponentFactoryRef>            │
 │  - registerFactory<T>(factory)                                   │
 │  - getFactory<T>() → returns factory reference                   │
 └────────────────────────┬─────────────────────────────────────────┘
                          │
                          │ dispatches by type
                          v
 ┌──────────────────────────────────────────────────────────────────┐
 │                   ComponentFactory<T, Capacity>                  │
 │  std::array<std::optional<T>, Capacity>                          │
 │  - create(args...) → T& (returns reference to new component)     │
 │  - get(index) → T& (access by array index)                       │
 │  - begin()/end() → iteration support                             │
 └──────────────────────────────────────────────────────────────────┘
                          │
                          │ stores
                          v
 ┌──────────────────────────────────────────────────────────────────┐
 │                    Component (IComponent derived)                │
 │  - BallPositionComponent: x, y, vx, vy, radius                   │
 │  - PaddlePositionComponent: y, speed, up_pressed, down_pressed   │
 │  - RectangleViewComponent: position, width, height, color        │
 │  - CircleViewComponent: position, radius, color                  │
 └──────────────────────────────────────────────────────────────────┘
```

**Component Lifecycle:**
1. **Scene Construction**: GameScene creates ComponentFactory instances as member variables
2. **Factory Registration**: Scene registers factories with ComponentPool::getInstance()
3. **Component Creation**: Scene calls `factory.create(args...)` to instantiate components
4. **Entity Association**: Components are linked to entities via Entity struct in GameScene
5. **Update Loop**: Each frame, scene reads/writes component data directly
6. **View Synchronization**: Position components update view components for rendering
7. **Draw Command Generation**: View components produce IDrawCommand objects
8. **Cleanup**: Scene destruction automatically cleans up component factories

ECS (Entity Component System) Model
----------------------------------
The engine implements a "Lite ECS" where entities are identity-based handles and components are data containers with optional behavior.

### Core Interfaces

**IComponent** (`IComponent.hpp`)
Pure interface for all components:
```cpp
struct IComponent {
    using TypeInfo   = std::type_index;
    using InstanceID = unsigned int;

    virtual ~IComponent() = default;
    [[nodiscard]] virtual auto getInstanceId() const -> InstanceID = 0;
    [[nodiscard]] virtual auto getTypeIndex() const -> TypeInfo = 0;
    [[nodiscard]] virtual auto getFactory() const -> IComponentFactory& = 0;
};
```

**IComponentFactory** (`IComponentFactory.hpp`)
Interface for component factories:
```cpp
struct IComponentFactory {
    using TypeInfo = std::type_index;
    using SizeType = size_t;

    virtual ~IComponentFactory() = default;
    [[nodiscard]] virtual auto getTypeID() const -> TypeInfo = 0;
    [[nodiscard]] virtual auto getSize() const -> SizeType = 0;
    [[nodiscard]] virtual auto capacity() const -> SizeType = 0;
    [[nodiscard]] virtual auto empty() const -> bool = 0;
    virtual auto clear() -> void = 0;
};
```

**ComponentFactory<T, Capacity>** (`ComponentFactory.hpp`)
Template implementation storing components in fixed-size array:
```cpp
template<typename TComponent, std::size_t Capacity = 256>
class ComponentFactory : public IComponentFactory {
    std::array<std::optional<TComponent>, Capacity> components;
    size_t size{ 0 };

    template<typename... TArgs>
    auto create(TArgs&&... args) -> TComponent&;

    [[nodiscard]] auto get(size_t index) -> TComponent&;
    auto begin() -> decltype(components.begin());
    auto end() -> decltype(components.end());
};
```

**ComponentPool** (`ComponentPool.hpp`)
Singleton registry mapping type IDs to factories:
```cpp
class ComponentPool {
    std::unordered_map<std::type_index, IComponentFactoryRef> factories_;

    public:
    static auto getInstance() noexcept -> ComponentPool&;

    template<typename T>
    void registerFactory(IComponentFactoryRef factory) noexcept;

    template<typename T>
    IComponentFactoryRef getFactory() const&;
};
```

### Component Categories

**Position Components** (Game Logic)
- `BallPositionComponent`: x, y, vx, vy, radius
- `PaddlePositionComponent`: y, speed, up_pressed, down_pressed

**View Components** (Rendering)
- `CircleViewComponent`: position, radius, color → produces CircleDrawCommand
- `RectangleViewComponent`: position, width, height, color → produces RectangleDrawCommand

View components implement `IViewComponent` which extends `IComponent`:
```cpp
struct IViewComponent : public IComponent {
    [[nodiscard]] virtual auto produceDrawCommand(IRenderer& renderer)
        -> std::shared_ptr<IDrawCommand> = 0;

    virtual auto setPosition(Point pos) -> void = 0;
    [[nodiscard]] virtual auto getPosition() const -> Point = 0;
    [[nodiscard]] virtual auto getColor() const -> Color = 0;
    virtual auto setColor(Color color) -> void = 0;
};
```

Entity Model
------------
Entities in Elemental are simple structs containing metadata and component references:

```cpp
class GameScene : public IState {
    public:
    using EntityId = idtype_t;

    struct Entity {
        EntityId                     id{ 0 };
        std::string                  name;
        std::vector<IViewComponent*> views;
        VelocityConfig               velocity;
    };

    std::unordered_map<EntityId, Entity> entities_;

    auto getEntity(EntityId id) -> Entity&;
    auto getEntityByName(const std::string& name) -> Entity&;
};
```

**Entity Creation Flow:**
1. GameScene defines entities with unique IDs (kBallId, kPlayerId, kEnemyId)
2. Components are created via factories and stored separately
3. Entity struct holds pointers/references to associated components
4. Entity metadata (type_id, instance_id, type_name, instance_name) enables debugging

Pong Implementation Example
---------------------------
The Pong demo in `Apps/Demos/phong/` demonstrates the complete ECS workflow.

### Component Definition
```cpp
// BallPositionComponent.hpp
class BallPositionComponent final : public IComponent {
    IComponentFactory& factory_;
    InstanceID         instance_id_{ 0 };
    float x_{ 0.0f }, y_{ 0.0f };
    float vx_{ 0.0f }, vy_{ 0.0f };
    float radius_{ 8.0f };

    public:
    BallPositionComponent(
        IComponentFactory& f,
        float x = 0.0f, float y = 0.0f,
        float vx = 300.0f, float vy = 180.0f,
        float r = 8.0f);

    [[nodiscard]] auto getInstanceId() const -> InstanceID override;
    [[nodiscard]] auto getTypeIndex() const -> TypeInfo override;
    [[nodiscard]] auto getFactory() const -> IComponentFactory& override;

    // Accessors
    [[nodiscard]] auto x() const -> float;
    auto setX(float val) -> void;
    // ... (y, vx, vy, radius accessors)
};
```

### Scene Setup (PongScene constructor)
```cpp
PongScene::PongScene(const SceneConfig& config) : GameScene(config)
{
    // 1. Get singleton pool and register factories
    auto& pool = ComponentPool::getInstance();
    pool.registerFactory<BallPositionComponent>(ball_pos_factory_);
    pool.registerFactory<PaddlePositionComponent>(paddle_pos_factory_);

    // 2. Create components via factories
    ball_pos_ = &ball_pos_factory_.create(
        court_width_ / 2.0f,    // x
        court_height_ / 2.0f,   // y
        kBallSpeed,             // vx
        kBallSpeed * 0.6f,      // vy
        kBallRadius_);          // radius

    player_pos_ = &paddle_pos_factory_.create(
        court_height_ / 2.0f - half_paddle,  // y
        kPaddleSpeed);                       // speed

    // 3. Get entity references
    auto& ball_entity   = getEntity(kBallId);
    auto& player_entity = getEntity(kPlayerId);

    // 4. Link view components (created by parent GameScene)
    if (!ball_entity.views.empty()) {
        ball_view_ = dynamic_cast<CircleViewComponent*>(
            ball_entity.views[0]);
    }
}
```

### Update Loop (PongScene::onUpdate)
```cpp
auto PongScene::onUpdate() -> void
{
    static constexpr float kDt = 1.0f / 60.0f;

    // Read component data, compute, write back
    updateBall(kDt);
    updatePlayer(kDt);
    updateEnemy(kDt);
    checkCollisions();

    // Sync position components with view components
    syncViewComponents();
}

auto PongScene::updateBall(float dt) -> void
{
    // Direct component access for performance
    ball_pos_->setX(ball_pos_->x() + ball_pos_->vx() * dt);
    ball_pos_->setY(ball_pos_->y() + ball_pos_->vy() * dt);

    // Wall collision (top/bottom)
    if (ball_pos_->y() - ball_pos_->radius() <= 0.0f) {
        ball_pos_->setVy(std::abs(ball_pos_->vy()));
    }

    // Scoring (left/right)
    if (ball_pos_->x() <= 0.0f) {
        enemy_score_++;
        resetBall();
    }
}

auto PongScene::syncViewComponents() -> void
{
    // Copy position component data to view component
    if (ball_view_ != nullptr) {
        ball_view_->setPosition(
            Point{ static_cast<uint32_t>(ball_pos_->x()),
                   static_cast<uint32_t>(ball_pos_->y()) });
    }
    // ... (player, enemy sync)
}
```

### Rendering Integration
```cpp
// GameScene::getDrawCommands()
auto GameScene::getDrawCommands()
    -> std::list<std::shared_ptr<IDrawCommand>>
{
    std::list<std::shared_ptr<IDrawCommand>> commands;

    for (auto& [id, entity] : entities_) {
        for (auto* view : entity.views) {
            // Each view component produces its own draw command
            commands.push_back(view->produceDrawCommand(renderer_));
        }
    }

    return commands;
}

// RectangleViewComponent::produceDrawCommand
auto RectangleViewComponent::produceDrawCommand(IRenderer& renderer)
    -> std::shared_ptr<IDrawCommand>
{
    return renderer.createRectangleCommand(
        position_, width_, height_, color_);
}
```

State Management
----------------
GameScene inherits from IState and integrates with the StateStack:

```cpp
class GameScene : public IState {
    // ECS management (factories, entities, renderer)

    auto step() -> void override {
        // Calls onUpdate() for game logic
        onUpdate();
    }

    auto getDrawCommands()
        -> std::list<std::shared_ptr<IDrawCommand>> override;

    protected:
    virtual auto onUpdate() -> void;  // Override in derived scenes
};
```

**State Stack Integration:**
- StateStack manages active GameScene instances
- Pushing a state: Calls scene initialization, registers component factories
- Active state: `step()` called each frame by LoopRegulator
- Popping a state: Scene destructor cleans up component factories

Rendering Pipeline
------------------
The ECS integrates with rendering through the View Component pattern:

```
Component Data (Position)     View Component          Renderer
     │                              │                     │
     │ ball_pos_->x(), y()          │                     │
     │─────────────────────────────>│                     │
     │                              │ produceDrawCommand()│
     │                              │────────────────────>│
     │                              │                     │
     │                              │                     │ createRectangleCommand()
     │                              │                     │
     │                              │<────────────────────│
     │                              │                     │
     │                              │<────────────────────│ std::shared_ptr<IDrawCommand>
     │                              │                     │
```

1. **Game Logic Update**: Position components modified by game logic
2. **View Sync**: Position data copied to view components
3. **Command Generation**: `ViewComponent::produceDrawCommand()` creates command
4. **Command Submission**: GameScene collects commands, sends to renderer
5. **Render Execution**: Renderer draws shapes based on commands

Error handling and UX
---------------------
- **Factory Not Registered**: `std::runtime_error` thrown if `getFactory<T>()` called for unregistered type
- **Component Access**: No bounds checking on `getUnchecked(index)`; use `get(index)` for safety
- **Null View Pointers**: View components may be null if entity has no views; null checks required
- **Dynamic Cast Failures**: View component retrieval uses `dynamic_cast`; returns nullptr on mismatch

**Development-time Checks:**
- Component type mismatches caught at compile-time via template constraints
- Factory capacity exceeded: No runtime check; undefined behavior if exceeded
- Entity ID collisions: No duplicate detection; caller must ensure unique IDs

Future-proofing
---------------
- **Component Swapping**: Factory-based design allows replacing component implementations without changing entity code
- **System Abstraction**: Current direct access pattern could evolve into formal System classes
- **Renderer Independence**: IViewComponent interface allows different renderer backends (SDL, OpenGL, Vulkan)
- **Entity Queries**: Entity storage could support filtering by component type for ECS-style queries
- **Component Events**: Components could emit events on change for reactive systems

**Anticipated Enhancements:**
1. Add component event signals for decoupled communication
2. Implement entity queries (get all entities with Position + View)
3. Add component serialization for save/load
4. Support component pooling with dynamic capacity
5. Add system registry for automatic update ordering

Implementation outline
----------------------
1. **Core Interfaces**: Define IComponent, IComponentFactory pure interfaces
2. **Template Factory**: Implement ComponentFactory<T, Capacity> with array storage
3. **Component Pool**: Create singleton registry with type-based lookup
4. **Entity Struct**: Define Entity container in GameScene
5. **View Components**: Implement IViewComponent interface and concrete shapes
6. **Game Integration**: PongScene demonstrates complete workflow
7. **Rendering Bridge**: Connect view components to draw command system

Testing approach
----------------
- **Unit Tests**: Component creation, factory operations, pool registration
  - `Tests/Component.test.cpp`: IComponent interface validation
  - `Tests/ComponentFactory.test.cpp`: Factory creation and access
  - `Tests/Entity.test.cpp`: Entity metadata and component association

- **Integration Tests**: Scene-level ECS workflows
  - PongScene: Full game loop with component updates
  - View synchronization: Position → View → DrawCommand pipeline

- **Manual Testing**: Visual verification via Pong demo
  - Ball movement and collision
  - Paddle control and bounds
  - Rendering correctness

Acceptance criteria
-------------------
- [ ] ComponentFactory creates components and returns valid references
- [ ] ComponentPool registers and retrieves factories by type
- [ ] GameScene can access components directly for game logic updates
- [ ] View components synchronize with position components each frame
- [ ] Draw commands generated from view components render correctly
- [ ] Entity struct associates multiple view components with single entity
- [ ] Component data persists across state stack transitions
- [ ] Factory destruction cleans up all created components
- [ ] Pong demo runs at stable frame rate with ECS-driven logic
- [ ] Adding new component type requires only: define class, create factory, register with pool
