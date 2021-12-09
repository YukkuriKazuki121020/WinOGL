#include "pch.h"
#include "CAdminControl.h"
#include <math.h>
#include "CMath.h"

CAdminControl::CAdminControl()
{
	editFlag = false;
	axisFlag = false;
	lButtonClicking = false;
	dragging = false;
	moving = false;
	mevFlag = false;
	kevFlag = false;
	debugFlag = false;
	shape_head = new CShape;
	shape_tail = shape_head;
}

CAdminControl::~CAdminControl()
{
	FreeShape();
}

void CAdminControl::SetVertex(double x, double y, double ratio)
{
	this->ratio = ratio;
	if (!editFlag && !mevFlag && !kevFlag) {
		CVertex* clickVertex = new CVertex(x, y);
		if (shape_head->GetVectorCnt() >= 2) {
			for (CShape* nowS = shape_head; nowS != NULL; nowS = nowS->GetNext()) {
				if (shape_tail->GetEndVertexSet()) { // 点を打とうと思っていた形状がすでに閉じていたとき
				// 現在の点を1つ目の点と接続
					shape_tail->SetNext(new CShape()); // 次の形を生成
					shape_tail = shape_tail->GetNext(); // 末尾の形状を取得
				}
				if (shape_tail->GetVertexCnt() >= 1) {// 末尾の形状を構成する頂点が1つ以上のとき
					if (CMath::IsCrossing(nowS, CVector(shape_tail->GetVertexTail(), clickVertex))) {// 自己または他の形状と交差していないとき
						break;
					}
					else {
						if (nowS == shape_tail) {
							if (CMath::DamnAimChecker(shape_tail, *clickVertex)) {// クリックした頂点と閉じようとしている形状の始点が近い距離にあるとき
								if (shape_head->GetNext() == NULL || !CMath::IsInsideForAll(shape_tail, shape_head, clickVertex, 's')) {
									// 形状が1つのみなら強制で追加
									// 形状が1つ以上なら、閉じようとしている形状が他の形状を内包していないとき
									shape_tail->DecideEndPoint(x, y);// 形状の終点を決定
								}
							}
							else {
								shape_tail->SetVertex(x, y);// 末尾の形状に頂点を追加
							}
							break;
						}
					}
				}
				else if (!CMath::IsInside(shape_head, clickVertex)) {
					shape_tail->SetVertex(x, y);// 末尾の形状に頂点を追加
				}
			}
		}
		else {
			shape_head->SetVertex(x, y);
		}
	}
}

void CAdminControl::Draw(double x, double y)
{
	CVertex* clickPoint = new CVertex(x, y);
	if (axisFlag) {
		DrawAxis();
	}

	if (shape_head != NULL) {
		if (shape_head->GetVertexHead() != NULL) {
			CShape* old_shape = NULL;
			for (CShape* nowS = shape_head; nowS != NULL; nowS = nowS->GetNext()) {
				if (editFlag) {
					if (!dragging) {// マウスをドラッグしているときは選択可否の更新をしない
						nowS->SetAPartSelectable(true);
						if (old_shape != NULL) {
							if (old_shape->GetAPartSelectable()) {
								nowS->SetAPartSelectable(false);
							}
						}
					}
				}
				
				nowS->Draw(clickPoint, editFlag, lButtonClicking, dragging);
				old_shape = nowS;
			}
		}
	}

}

void CAdminControl::Move(double x, double y)
{
	CVertex* clickPoint = new CVertex(x, y, NULL);
	if (editFlag && dragging) {
		if (shape_head != NULL) {
			if (shape_head->GetVertexHead() != NULL) {
				CShape* old_shape = NULL;
				for (CShape* nowS = shape_head; nowS != NULL; nowS = nowS->GetNext()) {
					if (old_shape == NULL) {
						nowS->Move(clickPoint);
					}
					else if(old_shape->GetAPartSelectable()){
						if (!nowS->GetAPartSelectable()) {
							nowS->Move(clickPoint);
						}
					}
					if (!nowS->GetAPartSelectable()) {
						old_shape = nowS;
					}
				}
			}
		}
	}
}

void CAdminControl::MEV(double x, double y)
{
	if (mevFlag) {
		CVertex* clickPoint = new CVertex(x, y);
		for (CShape* nowS = shape_head; nowS != NULL; nowS = nowS->GetNext()) {
			nowS->MEV(clickPoint);
		}
	}
	
}

void CAdminControl::KEV(double x, double y)
{
	if (kevFlag) {
		CVertex* clickPoint = new CVertex(x, y);
		for (CShape* nowS = shape_head; nowS != NULL; nowS = nowS->GetNext()) {
			if (nowS->GetVertexCnt() >= 5 && !CMath::IsInsideForAll(nowS,shape_head,clickPoint,'d')) {
				CShape* tmpShape = new CShape();
				nowS->Clone(tmpShape);
				tmpShape->KEV(clickPoint);
				for (CVector* nowVec = tmpShape->GetVectorHead(); nowVec != NULL; nowVec = nowVec->GetNext()) {
					if (!CMath::IsCrossing(nowS, *nowVec)) {
						nowS->KEV(clickPoint);
						break;
					}
				}
			}
		}
	}
}

void CAdminControl::SetAllSelectedFlag(bool selected_flag)
{
	for (CShape* nowS = shape_head; nowS != NULL; nowS = nowS->GetNext()) {
		nowS->SetAllSelectedFlag(selected_flag);
	}

}

void CAdminControl::Debug()
{
	if(debugFlag)
	debugFlag = false;
}



void CAdminControl::FreeShape() {
	CShape* temp = NULL;
	CShape* vp = shape_head;
	while (shape_head != NULL)
	{
		temp = shape_head->GetNext();
		delete shape_head;
		shape_head = temp;
	}
}

// 座標軸を描画する
void CAdminControl::DrawAxis(void) {
	glBegin(GL_LINES);
	// x軸
	glColor3f(1.0, 0.0, 0.0);
	glVertex3f(-1.0, 0.0, 0.0);
	glVertex3f(1.0, 0.0, 0.0);
	// y軸
	glColor3f(0.0, 1.0, 0.0);
	glVertex3f(0.0, -1.0, 0.0);
	glVertex3f(0.0, 1.0, 0.0);
	glEnd();
}
