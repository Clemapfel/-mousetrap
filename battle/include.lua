-- RESOURCE_PATH = ""
package.path = package.path .. ";" .. RESOURCE_PATH .. "include/lua/?.lua"

require "common"
require "meta"
require "test"

--- @module rat_game battle module
rt = {}

require "battle_log"
require "status_ailment"
require "stat_modifier"
require "entity"

println(math.abs(rt.StatModifier.MINUS_2 - rt.StatModifier.MINUS_1))