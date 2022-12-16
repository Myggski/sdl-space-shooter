# sdl-space-shooter

Simple Space Shooter made in SDL2 with ECS. 

## ECS Setup
You can see [here](https://github.com/Myggski/sdl-space-shooter/blob/40f5bc528f0584896efbe3dd516a79975d3fa82a/sdl_space_shooter/src/application/application.cpp#L60) how I setup the ECS-world in the run_game function. I first register components so the world can create a component array for each component. This is so the component can keep track of all the entities that will have that specific component. Then I create systems that contains game logic and will be running each frame. Then I pre-create background, score and player, then I let the systems do the rest. 

## Collision
I use spatial hash/grid to optimize the collision detection and a [collision system](https://github.com/Myggski/sdl-space-shooter/blob/main/sdl_space_shooter/src/ecs/systems/collision.cpp) that insert/removes an entity to the spatial grid whenever an entity with collision is being added/removed to/from the world. I have also a [physics system](https://github.com/Myggski/sdl-space-shooter/blob/40f5bc528f0584896efbe3dd516a79975d3fa82a/sdl_space_shooter/src/ecs/systems/physics_system.cpp#L47) that will update the spatial grid whenever the entity has changed its position. 

Collision checks happens in the [damage collision system](https://github.com/Myggski/sdl-space-shooter/blob/40f5bc528f0584896efbe3dd516a79975d3fa82a/sdl_space_shooter/src/ecs/systems/damage_collision.cpp#L37). 

## Build
To play the game you need to unzip the build.zip that you will find in the root folder of the project. You move the spaceship with WASD-keys and shoot lasers with Space-key. 