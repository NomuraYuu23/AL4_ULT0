#pragma once
#include <variant>

class Player;
class Enemy;

using ColliderParentObject = std::variant<Player*, Enemy*>;
