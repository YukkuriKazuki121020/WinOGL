#pragma once
#include "CVertex.h"

class CVector
{
public:
	CVector();
	CVector(CVertex* sp, CVertex* ep);
	~CVector();

	/* 始点と終点の設定と取得の関数 */
	// 始点
	void SetStartPoint(double x, double y);
	CVertex* GetStartPoint();
	// 終点
	void SetEndPoint(double x, double y);
	CVertex* GetEndPoint();

	/* 位置ベクトル計算の関数 */
	CVertex* CalcPositionVector();

	void SetNext(CVector* new_next);
	CVector* GetNext();

	bool IsZeroVector();

private:
	// 始点
	CVertex* sp;
	// 終点
	CVertex* ep;
	CVector* next_vector;
};