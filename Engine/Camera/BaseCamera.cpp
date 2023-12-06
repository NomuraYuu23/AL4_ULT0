#include "BaseCamera.h"

void BaseCamera::Initialize()
{

	//ビュープロジェクション
	viewProjection_.Initialize();

}

void BaseCamera::Update()
{

	//ビュープロジェクション
	viewProjection_.UpdateMatrix();

}
