--- @class Queue
function Queue()
    local out = meta._new("Queue")
    out.__meta.first_element = 0
    out.__meta.last_element = 1
    out.__meta.n_elements = 0
end

--- @brief add element to start of queue
--- @param queue Queue
--- @param x any
function push_front(queue, x)
    
    if not meta.isa(queue, Queue) then
        error("[ERROR] In push_front: Argument #1 has to be a Queue")
    end
    
    local current = queue.__meta.first_element
    queue[current] = x
    
    queue.__meta.first_element = current - 1
    queue.__meta.n_elements = queue.__meta.n_elements + 1
end

--- @brief add element to end of queue
--- @param queue Queue
--- @param x any
function push_back(queue, x)

    if not meta.isa(queue, Queue) then
        error("[ERROR] In push_back: Argument #1 has to be a Queue")
    end

    local current = queue.__meta.last_element
    queue[current] = x

    queue.__meta.last_element = current + 1
    queue.__meta.n_elements = queue.__meta.n_elements + 1
end

--- @brief remove element at start of queue
--- @param queue Queue
--- @return any nil if queue is empty
function pop_front(queue)

    if not meta.isa(queue, Queue) then
        error("[ERROR] In pop_front: Argument #1 has to be a Queue")
    end

    if (queue.__meta.n_elements == 0) then
        return nil
    end

    local i = queue.__meta.first_element + 1
    local out = queue[i]
    queue[i] = nil
    queue.first_element = i
    queue.n_elements = queue.n_elements - 1

    return out
end

--- @brief remove element at end of queue
--- @param queue Queue
--- @return any nil if queue is empty
function pop_back(queue)

    if not meta.isa(queue, Queue) then
        error("[ERROR] In pop_front: Argument #1 has to be a Queue")
    end

    if (queue.__meta.n_elements == 0) then
        return nil
    end

    local i = queue.__meta.last_element - 1
    local out = queue[i]
    queue[i] = nil
    queue.last_element = i
    queue.n_elements = queue.n_elements - 1

    return out
end

--- @brief get element at start of queue
--- @param queue Queue
--- @return any nil if queue is empty
function front(queue)
    if not meta.isa(queue, Queue) then
        error("[ERROR] In pop_front: Argument #1 has to be a Queue")
    end

    local i = queue.__meta.first_element + 1
    return queue[i]
end

--- @brief get element at end of queue
--- @param queue Queue
--- @return any nil if queue is empty
function back(queue)
    if not meta.isa(queue, Queue) then
        error("[ERROR] In pop_front: Argument #1 has to be a Queue")
    end

    local i = queue.__meta.last_element - 1
    return queue[i]
end
