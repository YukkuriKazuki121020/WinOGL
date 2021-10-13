#include "pch.h"
#include <gl/GL.h>
#include "CShape.h"

CShape::CShape()
{
	vertex_cnt = 0;
	next_shape = NULL;
	end_vertex_set = false;
	distance = 0;
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
}

void CShape::DrawVertices()
{
	//点の描画
	glPointSize(10);
	glBegin(GL_POINTS);

	if (vertex_head != NULL) {
		for (CVertex* nowV = vertex_head; nowV != NULL; nowV = nowV->GetNext()) {
			glVertex2f(nowV->GetX(), nowV->GetY());
			if (nowV->GetNext() != NULL) {
				glVertex2f(nowV->GetNext()->GetX(), nowV->GetNext()->GetY());
			}
		}
	}

	glEnd();
}

void CShape::DrawLines() {
	//辺の描画
	glBegin(GL_LINES);

	if (vertex_head != NULL && vertex_head->GetNext() != NULL) {
		for (CVertex* nowV = vertex_head; nowV->GetNext() != NULL; nowV = nowV->GetNext()) {
			glVertex2f(nowV->GetX(), nowV->GetY());
			if (nowV->GetNext() != NULL) {
				glVertex2f(nowV->GetNext()->GetX(), nowV->GetNext()->GetY());
			}
		}
	}

	glEnd();
}

CVertex* CShape::GetVertexHead() {
	return vertex_head;
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
		distance = sqrt(pow(vertex_head->GetX() - x, 2) + pow(vertex_head->GetY() - y, 2))*raito;
		if (distance <= 0.05) { // クリックした座標と一つ目の点との距離が10以下なら
			CVertex* nowV = vertex_head;
			while (nowV->GetNext() != NULL) {
				nowV = nowV->GetNext();
			}
			CVertex* endV = new CVertex(vertex_head->GetX(), vertex_head->GetY(), NULL);
			nowV->SetNext(endV); // 終点を始点に
			end_vertex_set = true;
		}
	}
}

bool CShape::IsPolygon() {
	return end_vertex_set;
}

int CShape::GetVertexCnt() {
	return vertex_cnt;
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