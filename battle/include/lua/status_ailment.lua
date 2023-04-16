--- @class StatusAilment
rt.StatusAilment = meta.new_enum({
    DEAD = -1,
    KNOCKED_OUT = 0,
    NO_STATUS = 1,
    AT_RISK = 2,
    STUNNED = 3,
    ASLEEP = 4,
    POISONED = 5,
    BLINDED = 6,
    BURNED = 7,
    CHILLED = 8,
    FROZEN = 9
})

--- @brief message when object gains status ailment
--- @param subject BattleID name of the subject
--- @param status_ailment StatusAilment
function rt.status_ailment_gained_message(subject, status_ailment)

    if not meta.isa(subject, rt.BattleID) then
        error("[ERROR] In status_gained_message: Argument #1 is not a string")
    end

    if not meta.is_enum_value(StatusAilment, status_ailment) then
        error("[ERROR] In status_gained_message: Argument #2 is not a StatusAilment")
    end

    local out = subject.name .. " "

    if status_ailment == StatusAilment.DEAD then
        return out .. " died"
    elseif status_ailment == StatusAilment.KNOCKED_OUT then
        return out .. " was knocked out"
    elseif status_ailment == StatusAilment.AT_RISK then
        return out .. " is now at risk"
    elseif status_ailment == StatusAilment.STUNNED then
        return out .. " is stunned"
    elseif status_ailment == StatusAilment.ASLEEP then
        return out .. " fell asleep"
    elseif status_ailment == StatusAilment.POISONED then
        return out .. " was poisoned"
    elseif status_ailment == StatusAilment.BLINDED then
        return out .. " was blinded"
    elseif status_ailment == StatusAilment.BURNED then
        return out .. " started burning"
    elseif status_ailment == StatusAilment.CHILLED then
        return out .. " was chilled"
    elseif status_ailment == StatusAilment.FROZEN then
        return out .. " froze solid"
    else end
end

--- @brief message when object looses status ailment
--- @param subject string name of the subject
--- @param status_ailment StatusAilment
function rt.status_ailment_lost_message(subject, status_ailment)

    if not meta.isa(subject, rt.BattleID) then
        error("[ERROR] In status_gained_message: Argument #1 is not a string")
    end

    if not meta.is_enum_value(StatusAilment, status_ailment) then
        error("[ERROR] In status_gained_message: Argument #2 is not a StatusAilment")
    end

    local out = subject.name .. " "

    if status_ailment == StatusAilment.DEAD then
        return out .. " came back from the dead"
    elseif status_ailment == StatusAilment.KNOCKED_OUT then
        return out .. " is no longer knocked out"
    elseif status_ailment == StatusAilment.AT_RISK then
        return out .. " is no longer at risk"
    elseif status_ailment == StatusAilment.STUNNED then
        return out .. " is no longer stunned"
    elseif status_ailment == StatusAilment.ASLEEP then
        return out .. " woke up"
    elseif status_ailment == StatusAilment.POISONED then
        return out .. " is no longer poisoned"
    elseif status_ailment == StatusAilment.BLINDED then
        return out .. " is no longer blinded"
    elseif status_ailment == StatusAilment.BURNED then
        return out .. " is no longer burning"
    elseif status_ailment == StatusAilment.CHILLED then
        return out .. " is no longer chilled"
    elseif status_ailment == StatusAilment.FROZEN then
        return out .. " thawed"
    else end
end

--- @brief message when status is applied once per turn
--- @param subject string name of the subject
--- @param status_ailment StatusAilment
function rt.status_ailment_apply_message(subject, status_ailment)

    if not meta.isa(subject, rt.BattleID) then
        error("[ERROR] In status_gained_message: Argument #1 is not a string")
    end

    if not meta.is_enum_value(StatusAilment, status_ailment) then
        error("[ERROR] In status_gained_message: Argument #2 is not a StatusAilment")
    end

    local out = subject.name .. " "

    if status_ailment == StatusAilment.DEAD then
        return out .. " laid there motionlessly"
    elseif status_ailment == StatusAilment.KNOCKED_OUT then
        return out .. " is unconcious"
    elseif status_ailment == StatusAilment.AT_RISK then
        return out .. " is still at risk"
    elseif status_ailment == StatusAilment.STUNNED then
        return out .. " is stunned and can't move"
    elseif status_ailment == StatusAilment.ASLEEP then
        return out .. " woke up"
    elseif status_ailment == StatusAilment.POISONED then
        return out .. " was damaged by poison"
    elseif status_ailment == StatusAilment.BLINDED then
        return out .. " rubs " .. rt.possesive_pronoun(id) .. " eyes"
    elseif status_ailment == StatusAilment.BURNED then
        return out .. " is burning"
    elseif status_ailment == StatusAilment.CHILLED then
        return out .. " "
    elseif status_ailment == StatusAilment.FROZEN then
        return out .. " is frozen solid"
    else end
end

