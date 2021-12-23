#pragma once
#include "CVertex.h"
#include "CVector.h"
#include "CShape.h"


class CMath
{
public:
	CMath();
	~CMath();

	static double CalcDistance(CVector* v);

	static bool JudgeDistance(CVertex* Ver, CVertex* clickPoint);
	// 形状関係の関数
	static bool JudgeSelectedForPoint(CVertex* v1, CVertex* v2);// 点を選択したかを判定する関数
	static bool JudgeSelectedForLine(CVector* vec, CVertex* ver);// 線を選択したかを判定する関数
	static bool JudgeSelectedForShape(CShape* Shape, CVertex* ver);// 形状を選択したかを判定する関数
	static CVertex* GetSelectedPoint(CShape* targetShape, CVertex* clickPoint);// 選択した点を返す関数
	static CVector* GetSelectedLine(CShape* targetShape, CVertex* clickPoint);// 選択した辺を返す関数
	static CShape* GetSelectedShape(CShape* shape_head, CVertex* clickPoint);// 選択した形状を返す関数
	static bool DamnAimChecker(CShape* targetShape, CVertex clickPoint);// 始点をクリックしたかを判定する。始点をクリックしたときに

	// ベクトル関係の関数
	static double CalcInnerProduct2d(CVector va, CVector vb);// 内積（二次元）
	static double CalcCrossProduct2d(CVector va, CVector vb);// 外積（二次元）
	static bool IsCrossing(CShape* targetShape, CVector vec);// 交差判定（形状のすべての辺について）の関数
	static bool IsCrossingForAll(CShape* targetShape, CShape* shape_head, CVertex* clickVertex, const char* mode);// 交差判定（任意の形状を対象にした全ての形状について）の関数
	static bool IsInside(CShape* targetShape, CVertex* innerVertex);// 内包判定
	static bool IsInsideForAll(CShape* targetShape, CShape* shape_head, CVertex* clickVertex, const char* mode);// 内外判定

private:
	static bool IsCrossingCore(CVector va, CVector vb);// 交差判定の内部
};

