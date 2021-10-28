#pragma once
#include "CVertex.h"
#include "CVector.h"
#include "CShape.h"


class CMath
{
public:
	CMath();
	~CMath();

	// 形状関係の関数
	static bool DamnAimChecker(CShape* targetShape, CVertex clickPoint, double ratio);//始点をクリックしたかを判定する。始点をクリックしたときに

	// ベクトル関係の関数
	static double CalcInnerProduct2d(CVector va, CVector vb);// 内積（二次元）
	static double CalcCrossProduct2d(CVector va, CVector vb);// 外積（二次元）
	static bool IsCrossing(CShape* targetShape, CVector vec);// 交差判定（形状のすべての辺について）の関数
	static bool IsCrossingForOtherShape(CShape* targetShape, CVector vec);// 交差判定（他の形状との交差判定）の関数
	static bool IsInside(CShape* targetShape, CVertex* clickPoint);

private:
	static bool IsCrossingCore(CVector va, CVector vb);// 交差判定の内部
};

