#include "PlayerCommand.h"

PlayerCommand* PlayerCommand::GetInstance()
{
	static PlayerCommand instance;
	return &instance;
}

uint32_t PlayerCommand::Command()
{
    return 0;
}
