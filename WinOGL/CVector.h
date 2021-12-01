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
	void SetStartPoint(CVertex* sp);
	CVertex* GetStartPoint();
	// 終点
	void SetEndPoint(double x, double y);
	void SetEndPoint(CVertex* ep);
	CVertex* GetEndPoint();

	/* 位置ベクトル計算の関数 */
	CVertex* CalcPositionVector();

	double CalcVectorSize();

	void SetNext(CVector* new_next);
	CVector* GetNext();

	bool IsZeroVector();

	void SetSelectedFlag(bool selected_flag);// selected_flagを設定する関数
	bool GetSelectedFlag();// 辺が選択されたかを返す関数
	void Insert(CVector* insertVec);// 辺を追加する関数

private:
	// 始点
	CVertex* sp;
	// 終点
	CVertex* ep;
	CVector* next_vector;
	bool selected_flag;// 選択されたか
};