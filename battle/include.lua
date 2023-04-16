-- RESOURCE_PATH = ""
package.path = package.path .. ";" .. RESOURCE_PATH .. "include/lua/?.lua"

require "common"
require "meta"
require "test"

--- @module rat_game battle module
rt = {}

require "battle_log"
require "status_ailment"

id = rt.BattleID({
    id = "id",
    name = "Name",
    gender = rt.GrammaticGender.FEMALE
})

print(string.capitalize(rt.possesive_pronoun(id)))