--- @class Queue
Queue = meta.new_type("Queue", {})
Queue.__meta.__call = function()

    local out = meta._new("Queue")
    local start_i = 0
    out.__meta.first_element = start_i
    out.__meta.last_element = start_i
    out.__meta.n_elements = 0

    out.push_front = push_front
    out.push_back = push_back
    out.pop_front = pop_front
    out.front = front
    out.pop_back = pop_back
    out.back = back
    out.get_size = get_size
    out.is_empty = is_empty

    out.__meta.__index = function(this, i)
        if not meta.is_number(i) then
            return rawget(this, i)
        end
        return rawget(this, getmetatable(this).first_element + 1 + i)
    end

    out.__meta.__newindex = function(this, i, new)
        if not meta.is_number(i) then
            rawset(this, i, new)
        else
            rawset(this, getmetatable(this).first_element + 1 + i, new)
        end
    end

    return out
end

--- @brief add element to start of queue
--- @param queue Queue
--- @param x any
function push_front(queue, x)
    
    if not meta.isa(queue, Queue) then
        error("[ERROR] In push_front: Argument #1 has to be a Queue")
    end

    local q_meta = rawget(queue, "__meta")
    local current = q_meta.first_element - 1
    rawset(queue, current, x)

    q_meta.first_element = current
    q_meta.n_elements = q_meta.n_elements + 1
end

--- @brief add element to end of queue
--- @param queue Queue
--- @param x any
function push_back(queue, x)

    if not meta.isa(queue, Queue) then
        error("[ERROR] In push_back: Argument #1 has to be a Queue")
    end

    local q_meta = getmetatable(queue)
    local current = q_meta.last_element
    rawset(queue, current, x)

    q_meta.last_element = current + 1
    q_meta.n_elements = q_meta.n_elements + 1
end

--- @brief remove element at start of queue
--- @param queue Queue
--- @return any nil if queue is empty
function pop_front(queue)

    if not meta.isa(queue, Queue) then
        error("[ERROR] In pop_front: Argument #1 has to be a Queue")
    end

    local q_meta = getmetatable(queue)

    if (q_meta.n_elements == 0) then
        return nil
    end

    local i = q_meta.first_element
    local out = rawget(queue, i)
    rawset(queue, i, nil)
    q_meta.first_element = i + 1
    q_meta.n_elements = q_meta.n_elements - 1

    return out
end

--- @brief remove element at end of queue
--- @param queue Queue
--- @return any nil if queue is empty
function pop_back(queue)

    if not meta.isa(queue, Queue) then
        error("[ERROR] In pop_front: Argument #1 has to be a Queue")
    end

    local q_meta = getmetatable(queue)

    if (q_meta.n_elements == 0) then
        return nil
    end

    local i = q_meta.last_element - 1
    local out = rawget(queue, i)
    rawset(queue, i, nil)
    q_meta.last_element = i
    q_meta.n_elements = q_meta.n_elements - 1

    return out
end

--- @brief get element at start of queue
--- @param queue Queue
--- @return any nil if queue is empty
function front(queue)
    if not meta.isa(queue, Queue) then
        error("[ERROR] In pop_front: Argument #1 has to be a Queue")
    end

    return rawget(queue, getmetatable(queue).first_element + 1)
end

--- @brief get element at end of queue
--- @param queue Queue
--- @return any nil if queue is empty
function back(queue)
    if not meta.isa(queue, Queue) then
        error("[ERROR] In pop_front: Argument #1 has to be a Queue")
    end

    return rawget(queue, getmetatable(queue).last_element - 1)
end

--- @brief get number of elements in queue
--- @return number
function get_size(queue)
    if not meta.isa(queue, Queue) then
        error("[ERROR] In pop_front: Argument #1 has to be a Queue")
    end

    return rawget(queue, "__meta").n_elements
end

--- @brief check whether queue is empty
--- @return boolean
function is_empty(queue)
    if not meta.isa(queue, Queue) then
        error("[ERROR] In pop_front: Argument #1 has to be a Queue")
    end

    return get_size(queue) == 0
end
