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

	void SetFlags(const char* mode);// フラグを設定する関数

	void SetVertex(double x,double y ,double ratio);// 頂点を追加する関数
	void Select(double x, double y);// 選択の関数
	void Draw();// 描画の関数
	void Move(double x, double y);// 移動の関数
	void DeformationDecide();
	void MEV(double x, double y);// Make Edge Vertex：辺に頂点を追加する関数
	void KEV(double x, double y);// Kill Edge Vertex：辺から(?)頂点を削除する関数
	void Scale(const char* mode);// 拡大縮小をする関数
	void Rotate(double x, double y, const char* mode);// 回転をする関数
	void SetAllSelectedFlag(bool selected_flag);// 全ての形状を一括でSetAPartSelectabeする関数
	void Debug();// デバッグ用の関数（続行すると抜け出せるように設定してある）
	void DrawAxis();

	bool editFlag;
	bool axisFlag;
	bool lButtonClicking;
	bool dragging;
	bool moving;
	bool mevFlag;
	bool kevFlag;
	bool scaleFlag;
	bool rotateFlag;
	bool debugFlag;
	double ratio;

private:
	CShape* shape_head;
	CShape* shape_tail;
	CShape* shape_for_deformation;// 移動予定の形状を描画するための形状
	void FreeShape();
};

