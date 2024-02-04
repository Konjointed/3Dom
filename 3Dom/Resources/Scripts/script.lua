input.Update:Connect(function(deltaTime)
    if input:IsKeyDown(119) then
        print("w key pressed")
    end
end)

input.KeyPress:Connect(function(key)
    print("Key pressed: " .. key)
end)

input.KeyRelease:Connect(function(key)
    print("Key released: " .. key)
end)