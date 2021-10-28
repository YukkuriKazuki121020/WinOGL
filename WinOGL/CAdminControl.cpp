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
	CVertex* clickVertex = new CVertex(x, y, NULL);
	if (shape_head->GetVectorCnt() >= 2) {
		for (CShape* nowS = shape_head; nowS != NULL; nowS = nowS->GetNext()) {
			if (shape_tail->GetEndVertexSet()) { // 点を打とうと思っていた形状がすでに閉じていたとき
			// 現在の点を1つ目の点と接続
				shape_tail->SetNext(new CShape()); // 次の形を生成
				shape_tail = shape_tail->GetNext(); // 末尾の形状を取得
			}
			if (shape_head->GetNext() != NULL && nowS != shape_tail) {// 形状が2つ以上あり、現在見ている形状が末尾の形状でないとき
				
				if (CMath::IsInside(nowS, clickVertex)) {// 内外判定
					break;
				}
				
			}
			if (shape_tail->GetVertexCnt() >= 1) {// 末尾の形状を構成する頂点が1つ以上のとき
				if (CMath::IsCrossing(nowS, CVector(shape_tail->GetVertexTail(), clickVertex))) {// 自己または他の形状と交差していないとき
					break;
				}
				else {
					if (nowS == shape_tail) {
						if (CMath::DamnAimChecker(shape_tail, *clickVertex, ratio)) {// クリックした頂点と閉じようとしている形状の始点が近い距離にあるとき
							if (shape_head->GetNext() == NULL || !CMath::IsInsideForAll(shape_tail, shape_head, clickVertex)) {
								// 形状が1つのみなら強制で追加
								// 形状多1つ以上なら、閉じようとしているが他の形状を内包していないとき
								shape_tail->DecideEndPoint(x, y, ratio);// 形状の終点を決定
							}
						}
						else {
							shape_tail->SetVertex(x, y);// 末尾の形状に頂点を追加
						}
						break;
					}
				}
			}
			else {
				shape_tail->SetVertex(x, y);// 末尾の形状に頂点を追加
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

