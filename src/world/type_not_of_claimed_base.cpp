#include "type_not_of_claimed_base.h"
#include "world/WorldInterface.h"

type_not_of_claimed_base::type_not_of_claimed_base(const std::type_info &claimed_type, const WorldInterface *actual_object) :
game_exception("Type passed to SwitchFactory is not convertible to claimed type: " + std::string(claimed_type.name())
+ ", actual type: " + std::string(typeid(*actual_object).name())) {}
