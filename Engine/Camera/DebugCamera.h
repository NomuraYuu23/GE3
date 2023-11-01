#pragma once
#include "../3D/TransformStructure.h"

class DebugCamera
{

public:
	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();

	/// <summary>
	/// 更新
	/// </summary>
	TransformStructure Update(const TransformStructure& releaseCameratransform);

private:

	TransformStructure transform_;
	bool debugCameraUsed;

};

