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
