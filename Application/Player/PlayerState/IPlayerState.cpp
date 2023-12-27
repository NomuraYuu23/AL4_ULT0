#include "IPlayerState.h"
#include "../Player.h"

uint32_t IPlayerState::plaryerStateNo_ = 0;

uint32_t IPlayerState::plaryerMotionNo_ = 0;

Player* IPlayerState::player_ = nullptr;

Input* IPlayerState::input_ = Input::GetInstance();

Vector3Calc* IPlayerState::v3Calc_ = Vector3Calc::GetInstance();

Matrix4x4Calc* IPlayerState::m4Calc_ = Matrix4x4Calc::GetInstance();
