#pragma once
#include <variant>

class Player;
class Enemy;
class PlayerAttack;

using ColliderParentObject = std::variant<Player*, Enemy*, PlayerAttack*>;
