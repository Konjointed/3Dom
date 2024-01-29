position = {x = 0, y = 0} -- table 
seq = { 0, 0, 0} -- array/sequence

function greetings(...)
    local result = "Hello"
    for i, v in ipairs{...} do
        result = result .. " " .. v .. ","
    end
    return result
end

function dump_params(...)
    local results = {}
    for i, v in ipairs{...} do
        results[i] = i .. ": " .. tostring(v) .. " [" .. type(v) .. "]"
    end
    return table.unpack(results)
end

Destinations = {places={}}
setmetatable(Destinations.places, {
    __newindex = function(t,k,v)
        print("You cannot use Destinations directly")
    end,
})

function Destinations.new(global_name)
    local obj = {places = {}}
    setmetatable(obj, {__index = Destinations})
    if global_name then _G[global_name] = obj end 
    return obj
end

function Destinations:wish(...)
    for _,place in ipairs{...} do 
        self.places[place] = false
    end
end

function Destinations:went(...)
    for _,place in ipairs{...} do 
        self.places[place] = true
    end
end

function Destinations:list_visited() 
    local result = {}
    for place, visited in pairs(self.places) do
        if visited then result[#result + 1] = place end
    end
    return table.unpack(result)
end

function Destinations:list_unvisited() 
    local result = {}
    for place, visited in pairs(self.places) do
        if not visited then 
        result[#result + 1] = place 
        end
    end
    return table.unpack(result)
end