--- @brief print, arguments are concatenated
--- @param vararg any
--- @return void
function print(...)
    for _, v in pairs({...}) do
        io.write(tostring(v))
    end
end

--- @brief print, arguments are concatenade with a newline in between each
--- @param vararg any
--- @return void
function println(...)

    for _, v in pairs({...}) do
        io.write(tostring(v))
        io.write("\n")
    end
end

--- @brief get number of elements in arbitrary object
--- @param x any
--- @return number
function sizeof(x)
    if type(x) == "table" then
        local n = 0
        for _ in pairs(x) do
            n = n + 1
        end
        return n
    elseif type(x) == "string" then
        return #x
    else
        return 1
    end
end

--- @brief is table empty
--- @param x any
--- @return boolean
function is_empty(x)
    if type(x) ~= "table" then
        return true
    else
        return next(x) == nil
    end
end

--- @brief clamp
--- @param x number
--- @param lower_bound number
--- @param upper_bound number
--- @return number
function clamp(x, lower_bound, upper_bound)

    if x < lower_bound then
        x = lower_bound
    end

    if x > upper_bound then
        x = upper_bound
    end

    return x
end

--- @brief convert arbitrary object to string
--- @param id string
--- @param object any
--- @return string
function serialize(object_identifier, object, inject_sourcecode)

    if inject_sourcecode == nil then
        inject_sourcecode = false
    end

    get_indent = function (n_indent_tabs)

        local tabspace = "    "
        local buffer = {""}

        for i = 1, n_indent_tabs do
            table.insert(buffer, tabspace)
        end

        return table.concat(buffer)
    end

    insert = function (buffer, ...)

        for i, value in pairs({...}) do
            table.insert(buffer, value)
        end
    end

    get_source_code = function (func)

        local info = debug.getinfo(func);

        if string.sub(info.source, 1, 1) ~= "@" then
            return "[" .. tostring(func) .. "]"
        end

        local file = io.open(string.sub(info.source, 2), "r");

        if file == nil then return "" end

        local str_buffer = {}
        local i = 1
        local end_i = 1

        local first_line = true
        local single_line_comment_active = false
        local multi_line_comment_active = false

        for line in file:lines("L") do

            if end_i == 0 then break end

            if (i >= info.linedefined) then

                if not first_line then

                    local first_word = true;
                    for word in line:gmatch("%g+") do

                        if string.find(word, "%-%-%[%[") then
                            multi_line_comment_active = true
                        elseif string.find(word, "%-%-]]") then
                            multi_line_comment_active = false
                        elseif string.find(word, "%-%-") then
                            single_line_comment_active = true
                        end

                        if not (single_line_comment_active or multi_line_comment_active) then

                            if word == "if" or word == "for" or word == "while" or word == "function" then
                                end_i = end_i + 1
                            elseif word == "do" and first_word then     -- do ... end block
                                end_i = end_i + 1
                            elseif word == "end" or word == "end," then
                                end_i = end_i - 1
                            end
                        end

                        first_word = false
                    end
                end

                table.insert(str_buffer, line)
                first_line = false
            end

            single_line_comment_active = false;
            i = i + 1
        end

        file:close()

        -- remove last newline
        local n = #str_buffer
        str_buffer[n] = string.sub(str_buffer[n], 1, string.len(str_buffer[n]) - 1)

        return table.concat(str_buffer)
    end

    serialize_inner = function (buffer, object, n_indent_tabs)

        if type(object) == "number" then
            insert(buffer, object)

        elseif type(object) == "boolean" then
            if (object) then insert(buffer, "true") else insert(buffer, "false") end

        elseif type(object) == "string" then
            insert(buffer, string.format("%q", object))

        elseif type(object) == "table" then

            if sizeof(object) > 0 then
                insert(buffer, "{\n")
                n_indent_tabs = n_indent_tabs + 1

                local n_entries = sizeof(object)
                local index = 0
                for key, value in pairs(object) do

                    if type(key) == "string" then
                        insert(buffer, get_indent(n_indent_tabs), key, " = ")

                    elseif type(key) == "number" then

                        if key ~= index+1 then
                            insert(buffer, get_indent(n_indent_tabs), "[", key, "] = ")
                        else
                            insert(buffer, get_indent(n_indent_tabs))
                        end
                    end

                    serialize_inner(buffer, value, n_indent_tabs)
                    index = index +1

                    if index < n_entries then
                        insert(buffer, ",\n")
                    else
                        insert(buffer, "\n")
                    end
                end

                insert(buffer, get_indent(n_indent_tabs-1), "}")
            else
                insert(buffer, "{}")
            end

        elseif type(object) == "function" and inject_sourcecode then
            insert(buffer, get_source_code(object))
        elseif type(object) == "nil" then
            insert(buffer, "nil")
        else
            insert(buffer, "[" .. tostring(object) .. "]")
        end
    end

    if object == nil then
        return serialize("", object_identifier)
    end

    local buffer = {""}

    if object_identifier ~= "" then
        table.insert(buffer, object_identifier .. " = ")
    end

    serialize_inner(buffer, object, 0)
    return table.concat(buffer, "")
end

--- @brief positive infinity
INFINITY = 1/0

--- @brief negative infinity
NEGATIVE_INFINITY = -1/0

--- ##########################################################

--- @brief Type-system, formalizes private and public fields
meta = {}

--- @brief default value for function member
function meta.Function()
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

    local out = {}
    out.__meta = {}
    out.__meta.typename = meta.Enum
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
        return instance.__meta.values[key]
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
    x.__meta.__call = function(this)
        return meta.new(this)
    end

    x.name = typename
    x.properties = {}
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

    return m.typename == type.name
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

    type.properties[property_name] = initial_value
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

    return x
end

--- @brief Instantiate object from a meta.Type
--- @param type meta.Type
--- @returns instance
function meta.new(type)

    if not meta.is_type(type) then
        error("[ERROR] In meta.new: Argument is not a type")
    end

    local x = meta._new(type.name)

    for name, value in pairs(type) do
        if (name == "__meta") then
            goto continue
        end

        x[name] = value
        ::continue::
    end

    return x
end
