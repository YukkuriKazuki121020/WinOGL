#pragma once
#include "CVertex.h"


class CShape
{
public:
	CShape();
	~CShape();
private:
	CVertex* vertex_head;
	CShape* next_shape;
	int vertex_cnt;
	bool end_vertex_set;
	double distance;
	void FreeVertex();
public:
	void SetVertex(double x, double y);
	void DrawVertices();// 点を描画する関数
	void DrawLines();// 辺を描画する関数
	CVertex* GetVertexHead();// 一つ目の点を返す関数
	void SetNext(CShape* new_next);// 次の多角形を設定する関数
	CShape* GetNext();// 次の多角形を返す関数
	void DecideEndPoint(double x, double y, double raito);// 終点を指定する関数
	bool IsPolygon();// 多角形であるか判定する関数
	int GetVertexCnt();// 点の個数を返す関数
};

