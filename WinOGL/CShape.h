#pragma once
#include "CVertex.h"
#include "CVector.h"

class CShape
{
public:
	CShape();
	~CShape();
private:
	CVertex* vertex_head;
	CVertex* vertex_tail;
	CShape* next_shape;
	CVector* vector_head;
	CVector* vector_tail;
	int vertex_cnt;
	int vector_cnt;
	bool end_vertex_set;
	void FreeVertex();

	// ベクトル関係の関数
	void SetVector(CVertex* newV);
	bool IsCrossingCore(CVector va, CVector vb);// 交差判定の内部
public:
	static bool IsInside(CShape* targetShape, CVertex* clickPoint);
	void SetVertex(double x, double y);
	void DrawVertices();// 点を描画する関数
	void DrawLines();// 辺を描画する関数
	CVertex* GetVertexHead();// 一つ目の点を返す関数
	CVertex* GetVertexTail();// 最後の点を返す関数
	void SetNext(CShape* new_next);// 次の形状を設定する関数
	CShape* GetNext();// 次の形状を返す関数
	CVector* GetVectorHead();// 一つ目の点を返す関数
	CVector* GetVectorTail();// 最後の点を返す関数
	bool DamnAimChecker(CVertex clickPoint, double ratio);// 始点をクリックしたかを判定する。始点をクリックしたときに
	void DecideEndPoint(double x, double y, double ratio);// 終点を指定する関数
	bool GetEndVertexSet();// 終点が設定されたかを返す
	int GetVertexCnt();// 点の個数を返す関数
	int GetVectorCnt();// 辺の数を返す関数

	// ベクトル関係の関数

	double CalcInnerProduct2d(CVector va, CVector vb);// 内積（二次元）
	double CalcCrossProduct2d(CVector va, CVector vb);// 外積（二次元）
	bool IsCrossing(double x, double y);// 交差判定（形状のすべての辺について）の関数
	bool IsCrossingForOtherShape(CVector vec);// 交差判定（他の形状との交差判定）の関数

};

