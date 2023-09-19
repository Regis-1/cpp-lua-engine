# cpp-lua-engine
Simple game engine written in C++ aiming to to provide basic 2D capabilities and Lua scripting.

## Short description
This project is my attempt to make C++ game engine library (mostly for 2D video games). I want to provide the most common features of game engine such as rendering graphics, handling entities via ECS, having base systems like collision system etc. and also to provide a way to Lua scripting. For graphics, window management and other multimedia things I've decided to use __SFML__ library ([link](https://www.sfml-dev.org/)). ECS system has been implemented with __entt__ library ([link](https://github.com/skypjack/entt)) and Lua capabilities with official __Lua API__ library ([link](https://www.lua.org/download.html)).

## Current status
As for now the following features are available:
- rendering sprites on the scene
- adding Lua scripts to entities (with _init_ and _update_ callbacks)
- moving entities via Lua scripts
- handling user input via the mentioned Lua scripts

## Upcoming features
- playing animations (currently working on)
- handling collisions of entities (coming next)
- changing scenes

## Installation
I've tried to make this repository as concise and standalone as possible. To run the game you'll need __CMake__ and one of the make tools (like __make__ or __ninja__). If You have experience with building a __CMake__ projects there shouldn't be any nuance to it.\
\
In other case here are the instructions (with using __ninja__ as a make tool):
1. Make ```build``` directory somewhere in your file system and run ```cmake```
2. Select the repository folder as source directory and uncheck/set false the ```BUILD_SHARED_LIBS``` flag
![image](https://github.com/Regis-1/cpp-lua-engine/assets/59539717/25fe9f0d-c5c3-415e-8121-88ee2dafdadf)
3. Generate the makefiles for selected tool
4. Run the chosen tool
5. Run executable

# Usage of scripting
As for now the main (and only) scene is loaded from ```main_scene.lua``` file. There You can manage the entities, add components to them and also load textures to the texture catalog.
```lua
scene.LoadTexture("Cat01", "Textures/Animal/Cat 01-2.png")

local player_entity = scene.CreateEntity()
scene.SetComponent(player_entity, ComponentType.Behaviour, "Behaviours/player.lua")
```

Adding ```Behaviour``` component points to the scripts file that will be loaded for entity. In that file you need to create a Lua table that will be returned and it needs to include two functions: ```init``` and ```update```. Inside of those functions you can use the ```self``` to access the entity that the script is attached to.\
Scene's methods ```GetComponent``` and ```SetComponent``` are used to manage entity's components (creating, updating etc.) and Input's methods like ```IsKeyPressed``` are used to handling the input.
```lua
local player = {}

function player:init()
    local new_transform = {
        position = {x=50, y=50, z=0},
        rotation = {x=0, y=0, z=0},
        scale = {x=0, y=0, z=0}
    }

    scene.SetComponent(self.ID, ComponentType.Transform, new_transform)

    local sprite = {
        name = "Cat01",
        texture_rect = {32, 0, 32, 32}
    }

    scene.SetComponent(self.ID, ComponentType.Sprite, sprite)
end

function player:update()
    if (input.IsKeyPressed(Keys.E)) then 
        local transform = scene.GetComponent(self.ID, ComponentType.Transform)

        transform.position.x = transform.position.x + 1
        scene.SetComponent(self.ID, ComponentType.Transform, transform)
    end
end

return player
```
