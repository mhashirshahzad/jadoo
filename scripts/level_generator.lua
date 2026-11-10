
local W, H = 40, 40
local GAP = 2 -- Corridor width

math.randomseed(os.time())

local maze = {}

for y = 1, H do
    maze[y] = {}
    for x = 1, W do
        maze[y][x] = "*"
    end
end

local dirs = {
    { 2,  0},
    {-2,  0},
    { 0,  2},
    { 0, -2}
}

local function shuffle(t)
    for i = #t, 2, -1 do
        local j = math.random(i)
        t[i], t[j] = t[j], t[i]
    end
end

local function carve(x, y)
    for dy = 0, GAP - 1 do
        for dx = 0, GAP - 1 do
            if x + dx <= W and y + dy <= H then
                maze[y + dy][x + dx] = " "
            end
        end
    end
end

local stack = {{2, 2}}
carve(2, 2)

while #stack > 0 do
    local current = stack[#stack]
    local x, y = current[1], current[2]

    local available = {}

    for _, d in ipairs(dirs) do
        local nx = x + d[1] * GAP
        local ny = y + d[2] * GAP

        if nx >= 2 and nx + GAP - 1 <= W - 1
        and ny >= 2 and ny + GAP - 1 <= H - 1
        and maze[ny][nx] == "*" then
            table.insert(available, d)
        end
    end

    if #available == 0 then
        table.remove(stack)
    else
        shuffle(available)

        local dx = available[1][1] * GAP
        local dy = available[1][2] * GAP

        local nx, ny = x + dx, y + dy

        -- Carve corridor between rooms
        carve(x + dx // 2, y + dy // 2)
        carve(nx, ny)

        table.insert(stack, {nx, ny})
    end
end

print("char level[100][100] = {")

for y = 1, H do
    print('    "' .. table.concat(maze[y]) .. '",')
end

print("};")
