#include "pch.h"
#include "CAdminControl.h"
#include <math.h>
#include "CMath.h"

CAdminControl::CAdminControl()
{
	shape_head = new CShape;
	shape_tail = shape_head;
}

CAdminControl::~CAdminControl()
{
	FreeShape();
}

void CAdminControl::SetVertex(double x, double y, double ratio)
{
	/*
	if (shape_tail->GetEndVertexSet()) { // 多角形なら(点を打とうと思っていた形状がすでに閉じていたら)
		// 現在の点を一つ目の点と接続
		shape_tail->SetNext(new CShape()); // 次の形を生成
		shape_tail = shape_tail->GetNext(); // 末尾の形状を取得
	}
	// 自己交差していないなら
	
	if (!shape_tail->IsCrossing(x, y)) {
		CVertex* vertex_tmp;
		for (CShape* nowS = shape_head; nowS != NULL; nowS = nowS->GetNext()) {
			if (nowS == shape_tail) {
				if (shape_tail->DamnAimChecker(CVertex(x, y, NULL), ratio)) {
					shape_tail->DecideEndPoint(x, y, ratio);
				}
				else {
					shape_tail->SetVertex(x, y);
				}
				break;
			}
			// 一つも点が打たれていないなら
			if (shape_tail->GetVertexTail() == NULL) {
				vertex_tmp = new CVertex(x, y, NULL);
				// 内包判定
				if (CShape::IsInside(nowS, vertex_tmp)) {
					break;
				}
			}
			else {
				vertex_tmp = shape_tail->GetVertexTail();
			}

			// 他交差
			if (nowS->IsCrossingForOtherShape(CVector(vertex_tmp, new CVertex(x, y, NULL)))) {
				break;
			}
		}
	}
	*/
	if (shape_head->GetVectorCnt() >= 2) {
		for (CShape* nowS = shape_head; nowS != NULL; nowS = nowS->GetNext()) {
			if (shape_tail->GetEndVertexSet()) { // 多角形なら(点を打とうと思っていた形状がすでに閉じていたら)
			// 現在の点を一つ目の点と接続
				shape_tail->SetNext(new CShape()); // 次の形を生成
				shape_tail = shape_tail->GetNext(); // 末尾の形状を取得
			}
			if (shape_head->GetNext() != NULL && nowS != shape_tail) {// 形状が2つ以上あり、現在見ている形状が末尾の形状でないとき
				
				if (CMath::IsInside(nowS, new CVertex(x, y, NULL))) {
					break;
				}
				
			}
			if (shape_tail->GetVertexCnt() >= 1) {
				if (CMath::IsCrossing(nowS, CVector(shape_tail->GetVertexTail(), new CVertex(x, y, NULL)))) {
					break;
				}
				else {
					if (nowS == shape_tail) {
						if (CMath::DamnAimChecker(shape_tail, CVertex(x, y, NULL), ratio)) {
							shape_tail->DecideEndPoint(x, y, ratio);
						}
						else {
							shape_tail->SetVertex(x, y);
						}
						break;
					}
				}
			}
			else {
				shape_tail->SetVertex(x, y);
			}
		}
	}
	else {
		shape_head->SetVertex(x, y);
	}
			
	
}

void CAdminControl::Draw()
{
	if (shape_head != NULL) {
		if (shape_head->GetVertexHead() != NULL) {
			for (CShape* nowS = shape_head; nowS != NULL; nowS = nowS->GetNext()) {
				nowS->DrawVertices();

				nowS->DrawLines();
			}
		}
	}

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

