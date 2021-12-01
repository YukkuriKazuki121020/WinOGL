#pragma once
#include <gl/GL.h>
#include "CVertex.h"
#include "CShape.h"
#include "CVector.h"

class CAdminControl
{
public :
	CAdminControl();
	~CAdminControl();

	// 頂点を追加する関数
	void SetVertex(double x,double y ,double ratio);
	void Draw(double x, double y);// 描画の関数
	void Move(double x, double y);// 移動の関数
	void MEV(double x, double y);// Make Edge Vertex：辺に頂点を追加する関数
	void KEV(double x, double y);// Kill Edge Vertex：辺から(?)頂点を削除する関数
	void SetAllSelectedFlag(bool selected_flag);// 全ての形状を一括でSetAPartSelectabeする関数
	void Debug();// デバッグ用の関数（続行すると抜け出せるように設定してある）
	void DrawAxis();

	bool editFlag;
	bool axisFlag;
	bool lButtonClicking;
	bool dragging;
	bool mevFlag;
	bool kevFlag;
	bool debugFlag;
	double ratio;

private:
	CShape* shape_head;
	CShape* shape_tail;
	void FreeShape();
};

