--- @class Set
Set = meta.new_type("Set", {})
--- @constructor Set
Set:add_constructor(function()

    local out = meta.new(Set)
    getmetatable(out).n_elements = 0

    out.contains = Set.contains
    out.insert = Set.insert
    out.remove = Set.remove
    out.is_empty = Set.is_empty
    out.size = Set.size

    out.__meta._index = function(this, x)
        return Set.contains(this, x)
    end

    out.__meta.__newindex = function(this, x, new)
        Set.insert(this, new)
    end
end)

--- @brief check whether element is present in Set
--- @param set
--- @returm boolean
function Set.contains(set, x)

    if not meta.isa(set, Set) then
        error("[ERROR] In Set.contains: Argument #1 is not a Set")
    end

    return set[x] == true
end

--- @brief insert into set
--- @param set Set
--- @param x any
function Set.insert(set, x)

    if not meta.isa(set, Set) then
        error("[ERROR] In Set.insert: Argument #1 is not a Set")
    end

    if set[x] == true then
        return
    end

    set[x] = true
    getmetatable(set).n_elements = getmetatable(set).n_elements + 1
end

--- @brief remove from set
--- @param set Set
--- @param x any
function Set.erase(set, x)

    if not meta.isa(set, Set) then
        error("[ERROR] In Set.erase: Argument #1 is not a Set")
    end

    if set[x] ~= nil then
        set[x] = nil
        getmetatable(set).n_elements = getmetatable(set).n_elements - 1
    end
end

--- @brief get number of elements in set
--- @param set
--- @return number
function Set.size(set)

    if not meta.isa(set, Set) then
        error("[ERROR] In Set.size: Argument #1 is not a Set")
    end

    return getmetatable(set).n_elements
end

--- @brief get number of elements in set
--- @param set
--- @return number
function Set.is_empty(set)


    meta.assert_type(Set, set,  "Set.is_empty", 1)

    return set:size() == 0
end

Set.is_empty("abc")

