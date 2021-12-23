#include "pch.h"
#include "CAdminControl.h"
#include <math.h>
#include <string.h>
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
	scaleFlag = false;
	rotateFlag = false;
	debugFlag = false;
	moving = false;
	shape_head = new CShape();
	shape_tail = shape_head;
	shape_for_deformation = new CShape();
	shape_for_deformation->SetForDeformationFlag(true);
}

CAdminControl::~CAdminControl()
{
	FreeShape();
}

void CAdminControl::SetFlags(const char* mode)
{
	if (strstr(mode, "edit")) {
		editFlag = !editFlag;
	}
	if (!editFlag) {
		if (mevFlag || kevFlag || scaleFlag || rotateFlag) {
			mevFlag = false;
			kevFlag = false;
			scaleFlag = false;
			rotateFlag = false;
		}
	}
	else {
		if (strstr(mode, "mev")) {
			mevFlag = !mevFlag;
			kevFlag = false;
			scaleFlag = false;
			rotateFlag = false;
		}
		else if (strstr(mode, "kev")) {
			mevFlag = false;
			kevFlag = !kevFlag;
			scaleFlag = false;
			rotateFlag = false;
		}
		else if (strstr(mode, "scale")) {
			mevFlag = false;
			kevFlag = false;
			scaleFlag = !scaleFlag;
			rotateFlag = false;
		}
		else if (strstr(mode, "rotate")) {
			mevFlag = false;
			kevFlag = false;
			scaleFlag = false;
			rotateFlag = !rotateFlag;
		}
	}
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
					if (CMath::JudgeSelectedForPoint(nowS->GetVertexHead(), clickVertex)) {
						clickVertex->SetXY(nowS->GetVertexHead()->GetX(), nowS->GetVertexHead()->GetY());
					}
					if (CMath::IsCrossing(nowS, CVector(shape_tail->GetVertexTail(), clickVertex))) {// 自己または他の形状と交差していないとき
						break;
					}
					else {
						if (nowS == shape_tail) {
							if (CMath::DamnAimChecker(shape_tail, *clickVertex)) {// クリックした頂点と閉じようとしている形状の始点が近い距離にあるとき
								if (shape_head->GetNext() == NULL || (!CMath::IsInsideForAll(shape_tail, shape_head, clickVertex, "setVertex") && !CMath::IsCrossingForAll(shape_tail, shape_head, clickVertex, "setVertex"))) {
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
					break;
				}
			}
		}
		else {
			shape_head->SetVertex(x, y);
		}
	}
}

void CAdminControl::Select(double x, double y)
{
	CVertex* clickPoint = new CVertex(x, y);
	CShape* oldShape = NULL;
	if (editFlag && !dragging) {
		for (CShape* nowS = shape_head; nowS != NULL; nowS = nowS->GetNext()) {
			if (nowS->GetAPartSelectable()) {//どこか(頂点、辺、面のいずれか)ひとつでも選択できるとき
				if (oldShape != NULL) {
					if (!oldShape->GetAPartSelectable()) {
						break;
					}
				}
				for (CVector* nowVec = nowS->GetVectorHead(); nowVec != NULL; nowVec = nowVec->GetNext()) {
					nowVec->SetSelectedFlag(CMath::JudgeSelectedForLine(nowVec, clickPoint));
					if (nowVec->GetSelectedFlag()) {
						nowS->SetAPartSelectable(false);
					}
				}
				if (!nowS->GetAPartSelectable()) {
					//nowS->Clone(shape_for_deformation);
					break;
				}
				for (CVertex* nowVer = nowS->GetVertexHead(); nowVer != NULL; nowVer = nowVer->GetNext()) {
					nowVer->SetSelectedFlag(CMath::JudgeSelectedForPoint(nowVer, clickPoint));
					if (nowVer->GetSelectedFlag()) {
						nowS->SetAPartSelectable(false);
					}
				}
				if (!nowS->GetAPartSelectable()) {
					//nowS->Clone(shape_for_deformation);
					break;
				}
				if (nowS == CMath::GetSelectedShape(nowS, clickPoint)) {
					nowS->SetSelectedFlag(true);
					nowS->SetAPartSelectable(false);
				}
				else {
					nowS->SetSelectedFlag(false);
					nowS->SetAPartSelectable(true);
				}
				if (!nowS->GetAPartSelectable()) {
					//nowS->Clone(shape_for_deformation);
					break;
				}
			}
			oldShape = nowS;
		}
	}
}

void CAdminControl::Draw()
{
	if (axisFlag) {
		DrawAxis();
	}

	if (shape_head != NULL) {
		if (shape_head->GetVertexHead() != NULL) {
			CShape* old_shape = NULL;
			for (CShape* nowS = shape_head; nowS != NULL; nowS = nowS->GetNext()) {
				nowS->Draw(editFlag);
				shape_for_deformation->Draw(editFlag);
				old_shape = nowS;
			}
		}
	}

}

void CAdminControl::Move(double x, double y)
{
	CVertex* clickPoint = new CVertex(x, y, NULL);
	moving = true;
	if (editFlag && dragging) {
		if (shape_head != NULL) {
			if (shape_head->GetVertexHead() != NULL) {
				CShape* oldShape = NULL;
				for (CShape* nowS = shape_head; nowS != NULL; nowS = nowS->GetNext()) {
					if (oldShape != NULL) {
						if (!oldShape->GetAPartSelectable()) {
							break;
						}
					}
					if (!nowS->GetAPartSelectable()){
						if (!CMath::IsInsideForAll(nowS, shape_head, clickPoint, "move")
							&& !CMath::IsCrossingForAll(nowS, shape_head, clickPoint, "move")) {
							//shape_for_deformation->Move(clickPoint);
							nowS->Move(clickPoint);
						}
						else {
							//nowS->Move(new CVertex(-clickPoint->GetX(), -clickPoint->GetY()));
							nowS->SetAllSelectedFlag(false);
							nowS->SetAPartSelectable(true);
						}
					}
					
					oldShape = nowS;
				}
			}
		}
	}
}

void CAdminControl::DeformationDecide()
{
	/*
	bool is_crossing_or_inside = false;
	if (CMath::IsInsideForAll(shape_for_deformation, shape_head, NULL, "move")) {
		is_crossing_or_inside = true;
	}
	else {
		for (CVector* nowVec = shape_for_deformation->GetVectorHead(); nowVec != NULL; nowVec = nowVec->GetNext()) {
			for (CShape* nowS = shape_head; nowS != NULL; nowS = nowS->GetNext()) {
				if (nowS->GetAPartSelectable() && !CMath::IsCrossing(nowS, *nowVec)) {
					is_crossing_or_inside = true;
					break;
				}
			}
			if (is_crossing_or_inside) {
				break;
			}
		}
	}
	
	if (!is_crossing_or_inside) {
		for (CShape* nowS = shape_head; nowS != NULL; nowS = nowS->GetNext()) {
			if (!nowS->GetAPartSelectable()){
				shape_for_deformation->Clone(nowS);
				break;
			}
		}
	}
	moving = false;
	*/
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
			if (nowS->GetVertexCnt() >= 5){
				if (!CMath::IsInsideForAll(nowS, shape_head, clickPoint, "kev")
					&& !CMath::IsCrossingForAll(nowS, shape_head, clickPoint, "kev")) {
					nowS->KEV(clickPoint);
				}
				else{
					nowS->SetAllSelectedFlag(false);
					nowS->SetAPartSelectable(true);
				}
			}
		}
	}
}

void CAdminControl::Scale(const char* mode)
{
	char* unitedModeStr = "";
	if (strstr(mode, "expanding")) {
		unitedModeStr = "scale_expanding";
	}
	else if (strstr(mode, "shrinking")) {
		unitedModeStr = "scale_shrinking";
	}
	if (editFlag && scaleFlag) {
		for (CShape* nowS = shape_head; nowS != NULL; nowS = nowS->GetNext()) {
			if (nowS->GetSelectedFlag()){
				if (!CMath::IsInsideForAll(nowS, shape_head, NULL, unitedModeStr)
					&& !CMath::IsCrossingForAll(nowS, shape_head, NULL, unitedModeStr)) {
					nowS->Scale(mode);
				}
				else {
					/*
					char* inversionModeStr = "";
					if (strstr(mode, "expanding")) {
						inversionModeStr = "scale_shrinking";
					}
					else if (strstr(mode, "shrinking")) {
						inversionModeStr = "scale_expanding";
					}
					nowS->Scale(inversionModeStr);
					*/
					nowS->SetAllSelectedFlag(false);
					nowS->SetAPartSelectable(true);
				}
			}
		}
	}
}

void CAdminControl::Rotate(double x, double y, const char* mode)
{
	char* unitedModeStr = "";
	if (strstr(mode, "right")) {
		unitedModeStr = "rotate_right";
	}
	else if (strstr(mode, "left")) {
		unitedModeStr = "rotate_left";
	}
	if (editFlag && rotateFlag) {
		CVertex* clickPoint = new CVertex(x, y);
		for (CShape* nowS = shape_head; nowS != NULL; nowS = nowS->GetNext()) {
			if (!nowS->GetAPartSelectable()){
				if (!CMath::IsInsideForAll(nowS, shape_head, clickPoint, unitedModeStr)
					&& !CMath::IsCrossingForAll(nowS, shape_head, clickPoint, unitedModeStr)) {
					nowS->Rotate(clickPoint, mode);
				}
				else {
					/*
					char* inversionModeStr = "";
					if (strstr(mode, "right")) {
						inversionModeStr = "rotate_left";
					}
					else if (strstr(mode, "left")) {
						inversionModeStr = "rotate_right";
					}
					nowS->Rotate(clickPoint, inversionModeStr);
					*/
					nowS->SetAllSelectedFlag(false);
					nowS->SetAPartSelectable(true);
				}
			}
		}
	}
}

void CAdminControl::SetAllSelectedFlag(bool selected_flag)
{
	for (CShape* nowS = shape_head; nowS != NULL; nowS = nowS->GetNext()) {
		nowS->SetAllSelectedFlag(selected_flag);
		nowS->SetAPartSelectable(!selected_flag);
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
