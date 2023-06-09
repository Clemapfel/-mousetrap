
--- @module meta basic type system & introspection
meta = {}

--- @brief default value for function member
function Function()
    return function() error("[ERROR] In meta.Function: Attempting to call an uninitialized function") end
end

--- @brief default value for string member
function meta.String()
    return ""
end

--- @brief default value for number member
function meta.Number(n)
    if n == nil then return 0 else return n end
end

--- @brief default value for table member
function meta.Table()
    return {}
end

--- @brief default value for boolean
function meta.Boolean()
    return false
end

--- @brief Is x a lua string?
--- @param x any
--- @returns boolean
function meta.is_string(x)
    return type(x) == "string"
end

--- @brief Is x a lua table?
--- @param x any
--- @returns boolean
function meta.is_table(x)
    return type(x) == "table"
end

--- @brief Is x a lua number?
--- @param x any
--- @returns boolean
function meta.is_number(x)
    return type(x) == "number"
end

--- @brief Is x a lua boolean?
--- @param x any
--- @returns boolean
function meta.is_boolean(x)
    return type(x) == "boolean"
end

--- @brief Is x nil?
--- @param x any
--- @returns boolean
function meta.is_nil(x)
    return type(x) == "nil"
end

---@brief Is callable
---@param x and
---@returns boolean
function meta.is_function(x)
    if type(x) == "function" then
        return true
    elseif getmetatable(x) ~= nil then
        return meta.is_function(getmetatable(x).__call)
    end
end

--- @brief check if id can be used as a valid lua variable name
--- @param id string
function meta.is_valid_name(str)

    local before = _G[str]  -- prevent accidentally override global var
    local out, _ = pcall(load(str .. "=nil"))
    _G[str] = before
    return out
end

--- @brief Get type of meta instantiated object
--- @param x any
--- @returns string
function meta.typeof(x)

    if not meta.is_table(x) or rawget(x, "__meta") == nil then
        return type(x)
    else
        return rawget(x, "__meta").typename
    end
end

--- @class meta.Enum
meta.Enum = "Enum"

--- @brief generate new enum
--- @param values table
--- @returns meta.Enum
function meta.new_enum(values)

    local out = meta._new("Enum")
    out.__meta.values = {}

    local used_values = {}

    for name, value in pairs(values) do

        if not (meta.is_number(value) or meta.is_string(value)) then
            error("In meta.new_enum: Value is not a number or string")
        end

        if not meta.is_string(name) then
            error("In meta.new_enum: Key `" .. tostring(name) .. "` is not a string")
        end

        if used_values[value] ~= nil then
            error("In meta.new_enum: Duplicate value, key `" .. name .. "` and `" .. used_values[value] .. "` both have the same value `" .. tostring(value) .. "`")
        end

        used_values[value] = name
        out.__meta.values[name] = value
    end

    out.__meta.__newindex = function(instance, key, value)
        error("In enum.__newindex: Cannot modify an enum or its values")
    end

    out.__meta.__index = function(instance, key)
        local res = instance.__meta.values[key]
        if res == nil then
            error("In Enum.__index: No key with name `" .. key .. "`")
        end
        return res
    end

    out.__meta.__pairs = function(this)
        return pairs(this.__meta.values)
    end

    out.__meta.__ipairs = function(this)
        return ipairs(this.__meta.values)
    end

    setmetatable(out, out.__meta)
    return out;
end

--- @brief check if object is an enum
--- @param object any
--- @return boolean
function meta.is_enum(enum)

    if not meta.is_table(enum) then
        return false
    end

    if rawget(enum, "__meta") == nil then
        return false
    end

    return rawget(enum, "__meta").typename == meta.Enum
end

--- @brief check if value is in enum
--- @param enum meta.Enum
--- @value any
--- @return boolean
function meta.is_enum_value(enum, value)

    if not meta.is_enum(enum) then
        error("[ERROR] In meta.is_enum_value: Argument #1 is not an enum")
    end

    if not (meta.is_number(value) or meta.is_string(value)) then
        return false
    end

    for _, enum_value in pairs(enum) do
        if value == enum_value then
            return true
        end
    end

    return false
end

--- @brief export all enum constants to table
--- @param enum meta.Enum
--- @param table table
function meta.export_enum(enum, table)

    if not meta.is_enum(enum) then
        error("[ERROR] In meta.export_enum: Argument #1 is not an enum")
    end

    if not meta.is_table(table) then
        error("[ERROR] In meta.export_enum: Argument #2 is not a table")
    end

    for name, value in pairs(enum) do

        if table[name] ~= nil then
            print("[WARNING] In meta.export_enum: Enum key `" .. name .. "` overrides an already existing assignment")
        end
        table[name] = value
    end
end

--- @brief instantiate a new object, for internal use only
--- @param typename string
function meta._new(typename)

    if not meta.is_string(typename) then
        error("[ERROR] In meta._new: typename has to be string")
    end

    local x = {}
    x.__meta = {}
    x.__meta.typename = typename
    x.__meta.__tostring = function(this)
        return serialize("(" .. this.__meta.typename .. ")", this, false)
    end

    setmetatable(x, x.__meta)
    return x
end

--- @class meta.Type
meta.Type = "Type"

--- @brief Create a new meta.Type
--- @param typename string
--- @returns meta.Type
function meta._new_type(typename)

    if typename == nil then
        error("[ERROR] In meta.new_type: typename cannot be nil")
    end

    if not meta.is_string(typename) then
        error("[ERROR] In meta.new_type: typename has to be string")
    end

    if not meta.is_valid_name(typename) then
        error("[ERROR] In meta.new_type: " .. typename .. " is not a valid variable identifier")
    end

    local x = meta._new("Type")
    x.__meta.__call = function(this, args)
        return meta.new(this, args)
    end

    x.has_property = meta.has_property
    x.add_property = meta.add_property
    x.add_constructor = meta.add_construt
    x.name = typename

    x.__meta.typename = "Type"
    x.__meta.properties = {}
    return x
end

--- @brief Is x a meta.Type
--- @param x any
--- @returns boolean
function meta.is_type(x)

    if not meta.is_table(x) then
        return false
    end

    return x.__meta ~= nil and x.__meta.typename == meta.Type
end

--- @brief is object an instance of type
--- @param entity any
--- @param type meta.Type
function meta.isa(entity, type)

    if not meta.is_type(type) then
        error("[ERROR] in meta.isa: Argument #2 is not a type")
    end

    if not meta.is_table(entity) then
        return false
    end

    local m = rawget(entity, "__meta")
    if not meta.is_table(m) then
        return false
    end

    return m.typename == type.__meta.name
end

--- @brief Add property to meta.Type
--- @param type meta.Type
--- @param property_name string
--- @param initial_value any
--- @param is_private boolean
--- @returns void
function meta.add_property(type, property_name, initial_value)

    if not meta.is_type(type) then
        error("[ERROR] In meta.add_property: Object is not a type")
    end

    type.__meta.properties[property_name] = initial_value
end

--- @brief Does meta instance have a property with given id?
--- @param x meta.Type
--- @returns boolean
function meta.has_property(type, property_name)

    if (type.__meta == nil or type.__meta.is_property_private == nil) then
        return false
    end

    return meta.is_boolean(type.__meta.is_property_private[property_name])
end

--- @brief create a constructor for a given type T, it is invoked by calling T()
--- @param type Type
--- @param function function
function meta.add_constructor(type, f)

    if not meta.is_type(type) then
        error("[ERROR] In meta.add_constructor: Argument #1 is not a type")
    end

    if not meta.is_function(f) then
        error("[ERROR] In meta.add_constructor: Argumnet #2 is not a callable")
    end

    getmetatable(type).__call = f
end

--- @brief Create a new meta.Type from a table, syntactically convenient
--- @param typename string Name of type
--- @param table table table with properties
--- @returns meta.Type
function meta.new_type(typename, table)

    if not meta.is_string(typename) then
        error("[ERROR] In meta.new_type_from: Argument #1 has to be string")
    end

    if not meta.is_table(table) then
        error("[ERROR] In meta.new_type_from: Argument #2 has to be table")
    end

    local x = meta._new_type(typename)
    for name, value in pairs(table) do
        meta.add_property(x, name, value)
    end

    x.add_property = meta.add_property
    x.remove_property = meta.remove_property
    x.has_property = meta.has_property
    x.add_constructor = meta.add_constructor

    return x
end

--- @brief Instantiate object from a meta.Type
--- @param type meta.Type
--- @param args table [optional] constructor arguments
--- @returns instance
function meta.new(type, args)

    if not meta.is_type(type) then
        error("[ERROR] In meta.new: Argument is not a type")
    end

    local x = meta._new(type.__meta.name)

    if args == nil then
        args = {}
    end

    for name, value in pairs(type.__meta.properties) do
        if (name == "__meta") then
            goto continue
        end

        local ctor_value = args[name]
        if ctor_value ~= nil then
            x[name] = ctor_value
        else
            x[name] = value
        end

        ::continue::
    end

    return x
end

--- @brief raise an error when input item is not as expected
function meta.assert_type(type, x, domain, arg_i)

    if not meta.is_type(type) then
        error("[ERROR] In meta.assert_type: Argument #1 is not a type")
    end

    if arg_i == nil then arg_i = "?" end

    if not meta.isa(x, type) then
        error("[ERROR] In " .. domain .. ": Argument Mismatch for parameter #" .. tostring(arg_i) .. ": Expected `" .. type.name .. "`, got `" .. tostring(meta.typeof(x)).. "`")
    end
end