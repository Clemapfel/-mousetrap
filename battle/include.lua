-- RESOURCE_PATH = ""
package.path = package.path .. ";" .. RESOURCE_PATH .. "include/lua/?.lua"

require "common"
require "meta"
require "test"
require "action_queue"

--- @module rat_game battle module
rt = {}

require "battle_log"
require "status_ailment"
require "stat_modifier"
--- require "entity"


q = Queue()

push_back(q, 1)
push_back(q, 2)
push_back(q, 3)

println(q[0])
println(q[-1])
println(q[3])

print(q)

os.exit()

routines = {}
cr = coroutine.create(function()
    local i = 0
    while true do
        if not is_empty(routines) then
            current = routines[1]
            current()
        end

        i = i + 1
        coroutine.yield(i)
    end
end)

function add_routine(f)
    table.insert(routines, coroutine.wrap())
end

function step()
    courinte.resume(cr)
end

cr()
cr()

