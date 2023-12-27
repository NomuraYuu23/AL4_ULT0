#pragma once
#include <cstdint>
class PlayerCommand
{

public:

	/// <summary>
	/// インスタンス取得
	/// </summary>
	/// <returns></returns>
	static PlayerCommand* GetInstance();

public:

	/// <summary>
	/// コマンド
	/// </summary>
	/// <returns>ステート</returns>
	uint32_t Command();

private:
	PlayerCommand() = default;
	~PlayerCommand() = default;
	PlayerCommand(const PlayerCommand&) = delete;
	const PlayerCommand& operator=(const PlayerCommand&) = delete;

};

