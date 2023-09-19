scene.LoadTexture("Cat01", "Textures/Animal/Cat 01-2.png")

local player_entity = scene.CreateEntity()
scene.SetComponent(player_entity, ComponentType.Behaviour, "Behaviours/player.lua")
