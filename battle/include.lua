-- RESOURCE_PATH = ""
package.path = package.path .. ";" .. RESOURCE_PATH .. "include/lua/?.lua"
require "meta"
require "test"

test_type = meta.new_type("TestType", {
    property_01 = 1234
})

println(test_type)
test_instance = meta.new(test_type)
println(test_instance)
