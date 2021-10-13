#include "pch.h"
#include "CAdminControl.h"
#include <math.h>

CAdminControl::CAdminControl()
{
	shape_keeper = new CShape;
}

CAdminControl::~CAdminControl()
{
	FreeShape();
}

void CAdminControl::SetVertex(double x, double y, double raito)
{
	CShape* s = shape_keeper;
	if (s != NULL) {
		while (s->GetNext() != NULL) {
			s = s->GetNext();
		}
		s->DecideEndPoint(x, y, raito);
		if (s->IsPolygon()) { // 多角形なら
			// 現在の点を一つ目の点と接続
			CShape* new_shape = new CShape;
			s->SetNext(new_shape); // 次の形を生成
		}
		else {
			s->SetVertex(x, y);
		}
	}
	
}

void CAdminControl::Draw()
{
	if (shape_keeper != NULL) {
		if (shape_keeper->GetVertexHead() != NULL) {
			for (CShape* nowS = shape_keeper; nowS != NULL; nowS = nowS->GetNext()) {
				nowS->DrawVertices();

				nowS->DrawLines();
			}
		}
	}

}

void CAdminControl::FreeShape() {
	CShape* temp = NULL;
	CShape* vp = shape_keeper;
	while (shape_keeper != NULL)
	{
		temp = shape_keeper->GetNext();
		delete shape_keeper;
		shape_keeper = temp;
	}
}