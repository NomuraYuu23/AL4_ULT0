#pragma once
#include <variant>

class Player;

using ColliderParentObject = std::variant<Player*>;
