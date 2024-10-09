#pragma once

#include "System/MyMath/MYMATRIX.h"

// レンダーコンテキスト
struct RenderContext
{
	MYMATRIX view;			// ビュー行列
	MYMATRIX projection;	// プロジェクション行列
};
