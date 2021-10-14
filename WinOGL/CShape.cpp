#include "pch.h"
#include <gl/GL.h>
#define _USE_MATH_DEFINES
#include <math.h>
#include "CShape.h"


CShape::CShape()
{
	vertex_cnt = 0;
	vector_cnt = 0;
	vertex_head = NULL;
	vertex_tail = NULL;
	next_shape = NULL;
	vector_head = NULL;
	end_vertex_set = false;
}

CShape::~CShape()
{
	FreeVertex();
}


void CShape::SetVertex(double x, double y)
{
	CVertex* newV = new CVertex;
	newV->SetXY(x, y);
	if (vertex_head == NULL) {
		vertex_head = newV;
	}
	else {
		CVertex* v = vertex_head;
		while (v->GetNext() != NULL) {
			v = v->GetNext();
		}
		v->SetNext(newV);
	}
	vertex_cnt++;
	if (vertex_cnt > 1) {
		SetVector(newV);
	}
	vertex_tail = newV;
}

void CShape::DrawVertices()
{
	//点の描画
	glPointSize(10);
	glBegin(GL_POINTS);

	if (vertex_head != NULL) {
		for (CVertex* nowVer = vertex_head; nowVer != NULL; nowVer = nowVer->GetNext()) {
			glVertex2f(nowVer->GetX(), nowVer->GetY());
			if (nowVer->GetNext() != NULL) {
				glVertex2f(nowVer->GetNext()->GetX(), nowVer->GetNext()->GetY());
			}
		}
	}

	glEnd();
}

void CShape::DrawLines() {
	//辺の描画
	glBegin(GL_LINES);

	if (vertex_head != NULL && vertex_head->GetNext() != NULL) {
		for (CVertex* nowVer = vertex_head; nowVer->GetNext() != NULL; nowVer = nowVer->GetNext()) {
			glVertex2f(nowVer->GetX(), nowVer->GetY());
			if (nowVer->GetNext() != NULL) {
				glVertex2f(nowVer->GetNext()->GetX(), nowVer->GetNext()->GetY());
			}
		}
	}

	glEnd();
}

CVertex* CShape::GetVertexHead() {
	return vertex_head;
}

CVertex* CShape::GetVertexTail()
{
	return vertex_tail;
}

void CShape::SetNext(CShape* new_next) {
	next_shape = new_next;
}

CShape* CShape::GetNext()
{
	return next_shape;
}

void CShape::DecideEndPoint(double x, double y, double raito) {
	if (vertex_cnt >= 3) { // 点の個数が三つ以上なら（最後クリックする点が一つ目の点とすると、多角形は点が三つ以上であるため。）
		if (!IsCrossing(x, y)) {
			double distance = sqrt(pow(vertex_head->GetX() - x, 2) + pow(vertex_head->GetY() - y, 2)) * raito;
			if (distance <= 0.2) { // クリックした座標と一つ目の点との距離が10以下なら
				CVertex* nowVer = vertex_head;
				while (nowVer->GetNext() != NULL) {
					nowVer = nowVer->GetNext();
				}
				CVertex* endV = new CVertex(vertex_head->GetX(), vertex_head->GetY(), NULL);
				nowVer->SetNext(endV); // 終点を始点に
				end_vertex_set = true;
			}
		}
		
	}
}

bool CShape::IsPolygon() {
	return end_vertex_set;
}

int CShape::GetVertexCnt() {
	return vertex_cnt;
}

void CShape::SetVector(CVertex* newV) {
	CVector* newVec = new CVector(vertex_tail, newV);
	if (vector_head == NULL) {
		vector_head = newVec;
	}
	else {
		CVector* nowVec = vector_head;
		while (nowVec->GetNext() != NULL)
		{
			nowVec = nowVec->GetNext();
		}
		nowVec->SetNext(newVec);
	}
	vector_cnt++;
}

double CShape::CalcInnerProduct2d(CVector va, CVector vb) {

	double x = va.CalcPositionVector()->GetX();
	double y = va.CalcPositionVector()->GetY();
	CVertex a(x, y, NULL);

	x = va.CalcPositionVector()->GetX();
	y = va.CalcPositionVector()->GetY();
	CVertex b(x, y, NULL);
	return a.GetX() * b.GetX() + a.GetY() * b.GetY();
}

double CShape::CalcCrossProduct2d(CVector va, CVector vb)
{
	double x = va.CalcPositionVector()->GetX();
	double y = va.CalcPositionVector()->GetY();
	CVertex a(x, y, NULL);

	x = vb.CalcPositionVector()->GetX();
	y = vb.CalcPositionVector()->GetY();
	CVertex b(x, y, NULL);
	return a.GetX() * b.GetY() - a.GetY() * b.GetX();
}

bool CShape::IsCrossing(double x, double y) {
	if (vector_cnt > 1) {
		CVector tmpVec(vertex_tail, new CVertex(x, y, NULL));
		for (CVector* nowVec = vector_head; nowVec != NULL; nowVec = nowVec->GetNext()) {
			if (nowVec->GetEndPoint()->GetX() != tmpVec.GetStartPoint()->GetX()
				&& nowVec->GetEndPoint()->GetY() != tmpVec.GetStartPoint()->GetY()) {// 二つのベクトルの始点と終点が一致していなければ
				if (IsCrossingCore(*nowVec, tmpVec)) {
					return true;
				}
			}
		}
	}
	return false;
}

bool CShape::IsCrossing4OtherShape(CVector vec) {
	for (CVector* nowVec = vector_head; nowVec != NULL; nowVec = nowVec->GetNext()) {
		if (!vec.IsZeroVector()) {
			if (IsCrossingCore(*nowVec, vec)) {
				return true;
			}
		}
	}
	return false;
}

bool CShape::IsCrossingCore(CVector va, CVector vb)
{
	CVector a1(va.GetStartPoint(), vb.GetStartPoint());
	CVector a2(va.GetStartPoint(), vb.GetEndPoint());
	CVector b1(vb.GetStartPoint(), va.GetStartPoint());
	CVector b2(vb.GetStartPoint(), va.GetEndPoint());

	double ca1 = CalcCrossProduct2d(va, a1);
	double ca2 = CalcCrossProduct2d(va, a2);
	double cb1 = CalcCrossProduct2d(vb, b1);
	double cb2 = CalcCrossProduct2d(vb, b2);
	if (ca1 * ca2 <= 0 && cb1 * cb2 <= 0) {
		return true;
	}
	else {
		return false;
	}
}

bool CShape::IsInside(CVertex* ver)
{
	double theta = 0; // θ（角度）
	return abs(theta) == 2 * M_PI;
}

//　動的確保したVertexを解放する
void CShape::FreeVertex()
{
	CVertex* temp = NULL;
	CVertex* vp = vertex_head;
	while (vertex_head != NULL)
	{
		temp = vertex_head->GetNext();
		delete vertex_head;
		vertex_head = temp;
	}
}