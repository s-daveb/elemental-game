# Elemental Engine Architecture Design Doc
===========================================

Context and motivation
----------------------
The Elemental engine is designed as a modular, component-based framework to facilitate the development of top-down strategy games. It provides a robust foundation for entity management, state-driven execution, and hardware-accelerated rendering.

Goals:
- Provide a decoupled Entity-Component system for flexible object modeling.
- Implement a hierarchical state management system via a state stack.
- Establish a clean rendering pipeline that abstracts hardware-specific draw calls.
- Ensure modularity to allow for easier expansion of engine capabilities.

Non-goals for first implementation (v1):
- Full-scale ECS with multi-threaded system execution.
- Complex physics engine integration.
- Advanced spatial partitioning for massive entity counts.

Implementation considerations
-----------------------------
- **Composition over Inheritance**: Entities are collections of components rather than complex class hierarchies.
- **Dependency Management**: The engine relies on `IOCore` for base system primitives (threading, files, platform abstraction) and `SDL2` for multimedia-specific tasks.
- **Memory Safety**: Uses custom smart pointers (`SdlPtr`, `UniqueSdlPtr`) to manage SDL-specific resources.
- **Performance**: High-performance component access is achieved through specialized `ComponentFactory` and `ComponentPool` structures.

High-level behavior
-------------------
The engine follows a standard game loop execution model driven by a `LoopRegulator`. The lifecycle flows through a `StateStack` which manages the current active `IState` (e.g., a `GameScene`).

```
 ┌──────────────────────┐
 │ Application / IOCore │
 └──────────┬───────────┘
            │ initializes
            v
 ┌───────────────────┐      ┌────────────────┐
 │   LoopRegulator   │◄─────│ SdlEventSource  │
 └────────┬──────────┘      └────────────────┘
          │                        ▲
          │ tick                   │ input events
          v                        │
 ┌────────────────┐                │
 │   StateStack   │                │
 └───────┬────────┘                │
         │ delegates               │
         v                         │
    ┌───────────┐                  │
    │ GameScene │──────────────────┘
    └──┬────┬───┘
       │    │
  update│   │ generate
       │    │
       v    v
 ┌────────┐ ┌─────────────┐
 │ Entity │ │ DrawCommand │
 └───┬────┘ └──────┬──────┘
     │              │
     │ get/set      │ submit
     v              v
 ┌───────────────┐ ┌─────────────┐
 │ ComponentPool │ │ SdlRenderer │
 └───────┬───────┘ └─────────────┘
         │ dispatch
         v
 ┌──────────────────┐
 │ ComponentFactory │
 └──────────────────┘
```

ECS (Entity Component System) Model
----------------------------------
The engine uses a "Lite ECS" approach where entities are identity-based containers for components.

- **Entity**: A unique identifier that acts as a handle for grouping components.
- **Component**: Data-only structures (or logic-containing objects) that implement the `IComponent` interface.
- **ComponentFactory**: A type-specific pool that manages the allocation and lifecycle of specific component types.
- **ComponentPool**: A central registry that maps type IDs to their respective factories.

State Management
----------------
The `StateStack` manages a stack of `IState` objects. This allows for nested game logic (e.g., pausing the game by pushing a PauseState onto the stack).

- **Pushing a state**: Suspends the previous state and initializes the new one.
- **Popping a state**: Destroys the current state and returns control to the previous one.

Rendering Pipeline
------------------
Rendering is decoupled from game logic via a command-based system.

1. **Scene Update**: The current `IState` updates entity component data.
2. **Command Generation**: Entities/Components generate `IDrawCommand` objects (e.g., `ShapeDrawCommand`).
3. **Command Submission**: These commands are sent to the `IRenderer` (implemented by `SdlRenderer`).
4. **Final Render**: The renderer executes the commands to draw shapes, colors, and textures to the screen.

Error handling and UX
---------------------
- Errors are handled using `IOCore::Exception` and specialized exception classes.
- Resource loading failures are caught and surfaced through the engine's logging system.
- Fatal errors trigger exception dialogs (via `EditorWidgets`) in development builds.

Future-proofing
---------------
- **Component Swapping**: The factory-based component system allows replacing standard components with optimized versions without changing entity code.
- **Renderer Abstraction**: The `IRenderer` interface allows for potential future implementations (e.g., Vulkan or DirectX) without changing the game logic.

Implementation outline
----------------------
1. **Core Platform**: Initialize `IOCore` and setup platform-specific paths.
2. **Entity System**: Implement `Entity`, `IComponent`, and `ComponentFactory` logic.
3. **Execution Loop**: Implement `LoopRegulator` for stable heartbeats and `StateStack` for transitions.
4. **Rendering Engine**: Implement `SdlRenderer` and the `IDrawCommand` interface.
5. **Integration**: Connect `SdlEventSource` to the engine loop to bridge input and logic.

Testing approach
----------------
- **Unit Tests**: Each component and utility (e.g., `ComponentFactory`, `Entity`) is tested in isolation using Catch2.
- **Integration Tests**: Test the interaction between `GameScene` and the `StateStack`.
- **Manual Testing**: Visual verification of rendering output using sample scenes.

Acceptance criteria
-------------------
- [ ] Entities can successfully register and retrieve components from the `ComponentPool`.
- [ ] The `StateStack` can push and pop states without memory leaks.
- [ ] A `GameScene` can iterate through entities and issue `DrawCommands` to the renderer.
- [ ] The engine maintains a stable frame rate via the `LoopRegulator`.
