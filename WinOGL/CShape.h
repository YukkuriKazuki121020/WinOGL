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
	bool selected_flag;
	bool a_part_selectable;
	void DrawVertices();
	void DrawLines();
	void DrawSelectedVertices(CVertex* Ver);// 点を描画する関数
	void DrawSelectedLines(CVector* Vec);// 辺を描画する関数
	void DrawSelectedShape();// 形状を描画する関数
	void MoveVertices(CVertex* clickPoint);// 頂点を移動する関数
	void MoveLines(CVertex* clickPoint);// 辺を移動する関数
	void MoveShape(CVertex* clickPoint);// 形状を移動する関数
	void FreeVertex();

	// ベクトル関係の関数
	void SetVector(CVertex* newV);
public:
	void SetVertex(double x, double y);
	void Draw(CVertex* clickPoint, bool editFlag, bool lButtonClicking, bool dragging);
	CVertex* GetVertexHead();// 一つ目の点を返す関数
	CVertex* GetVertexTail();// 最後の点を返す関数
	void SetNext(CShape* new_next);// 次の形状を設定する関数
	CShape* GetNext();// 次の形状を返す関数
	CVector* GetVectorHead();// 一つ目の点を返す関数
	CVector* GetVectorTail();// 最後の点を返す関数
	void DecideEndPoint(double x, double y);// 終点を指定する関数
	bool GetEndVertexSet();// 終点が設定されたかを返す関数
	void SetSelectedFlag(bool selected_flag);// selected_flagを設定する関数
	bool GetSelectedFlag();// 形状が選択されたかを返す関数
	void SetAPartSelectable(bool a_part_selectable);// a_part_selectableを設定する関数
	bool GetAPartSelectable();// 形状のどこかの部位が選択されたかを返す関数
	void SetAllSelectedFlag(bool all_selected_flag);// 全ての形状を一括でSetAPartSelectabeする関数
	int GetVertexCnt();// 点の個数を返す関数
	int GetVectorCnt();// 辺の数を返す関数
	void Move(CVertex* clickPoint);// 移動する関数
	void MEV(CVertex* clickPoint);// Make Edge Vertex（辺に頂点を追加する関数）
	void KEV(CVertex* clickPoint);// Kill Edge Vertex（辺から頂点を削除する関数）
	void Clone(CShape* cloneShape);// 自分の情報を全て渡す関数

	//bool IsClockWise();// 時計回りかどうかを判定する
};

