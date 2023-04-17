--- @class BattleEntity
rt._BattleEntity = meta.new_type("BattleEntity", {

    _id = "",
    _name = "",
    _gender = rt.GrammaticGender.NEUTRAL,

    _base_attack = meta.Number(0),
    _base_defense = meta.Number(0),
    _base_speed = meta.Number(0),

    _hp_base = meta.Number(1),
    _hp_current = meta.Number(0),

    _ap_base = meta.Number(1),
    _ap_current = meta.Number(0),

    _attack_level = rt.StatLevel.ZERO,
    _defense_level = rt.StatLevel.ZERO,
    _speed_level = rt.StatLevel.ZERO,

    _status_ailments = {
        rt.StatusAilment.NO_STATUS
    }
})

function rt.BattleEntity(id)
    -- @todo load from config
end

--- @brief get id
--- @return BattleID
function rt.get_id(entity)

    if not meta.isa(entity, "BattleEntity") then
        error("[ERROR] In get_id: Argument #1 is not a BattleEntity")
    end

    return rt.BattleID({
        id = entity._id,
        name = entity._name,
        gender = entity._gender
    })
end