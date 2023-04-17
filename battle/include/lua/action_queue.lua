--- @class Queue
Queue = meta.new_type("Queue", {})
Queue.__meta.__call = function()
    local out = meta._new("Queue")
    local start_i = 0
    out.__meta.first_element = start_i
    out.__meta.last_element = start_i + 1
    out.__meta.n_elements = 0
    out.__meta.__index = function(this, i)
        return rawget(this, rawget(this, "__meta").first_element + 1 + i)
    end
    out.__meta.__newindex = function(this, i, new)
        return rawset(this, rawget(this, "__meta").first_element + 1 + i, new)
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
    local current = q_meta.first_element
    rawset(queue, current, x)

    q_meta.first_element = current - 1
    q_meta.n_elements = queue.__meta.n_elements + 1
end

--- @brief add element to end of queue
--- @param queue Queue
--- @param x any
function push_back(queue, x)

    if not meta.isa(queue, Queue) then
        error("[ERROR] In push_back: Argument #1 has to be a Queue")
    end

    local q_meta = rawget(queue, "__meta")
    local current = q_meta.last_element
    rawset(queue, current, x)

    q_meta.last_element = current + 1
    q_meta.n_elements = queue.__meta.n_elements + 1
end

--- @brief remove element at start of queue
--- @param queue Queue
--- @return any nil if queue is empty
function pop_front(queue)

    if not meta.isa(queue, Queue) then
        error("[ERROR] In pop_front: Argument #1 has to be a Queue")
    end

    local q_meta = rawget(queue, __meta)

    if (q_meta.n_elements == 0) then
        return nil
    end

    local i = q_meta.first_element + 1
    local out = rawget(queue, i)
    rawset(queue, i, nil)
    q_meta.first_element = i
    q_meta.n_elements = queue.n_elements - 1

    return out
end

--- @brief remove element at end of queue
--- @param queue Queue
--- @return any nil if queue is empty
function pop_back(queue)

    if not meta.isa(queue, Queue) then
        error("[ERROR] In pop_front: Argument #1 has to be a Queue")
    end

    local q_meta = rawget(queue, __meta)

    if (q_meta.n_elements == 0) then
        return nil
    end

    local i = q_meta.last_element - 1
    local out = rawget(queue, i)
    rawset(queue, i, nil)
    q_meta.first_element = i
    q_meta.n_elements = queue.n_elements - 1

    return out
end

--- @brief get element at start of queue
--- @param queue Queue
--- @return any nil if queue is empty
function front(queue)
    if not meta.isa(queue, Queue) then
        error("[ERROR] In pop_front: Argument #1 has to be a Queue")
    end

    return rawget(queue, rawget(queue, __meta).first_element + 1)
end

--- @brief get element at end of queue
--- @param queue Queue
--- @return any nil if queue is empty
function back(queue)
    if not meta.isa(queue, Queue) then
        error("[ERROR] In pop_front: Argument #1 has to be a Queue")
    end

    return rawget(queue, rawget(queue, __meta).last_element - 1)
end
