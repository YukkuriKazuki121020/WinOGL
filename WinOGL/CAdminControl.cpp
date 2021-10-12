#include "pch.h"
#include "CAdminControl.h"

CAdminControl::CAdminControl()
{
	vertex_head = NULL;
}

CAdminControl::~CAdminControl()
{
}

void CAdminControl::SetVertex(double x, double y)
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
}

void CAdminControl::Draw()
{

	glColor3f(1.0, 1.0, 1.0);
	
	glPointSize(10);
	glBegin(GL_POINTS);
	
	if (vertex_head != NULL) {
		for (CVertex* nowV = vertex_head; nowV != NULL; nowV = nowV->GetNext()) {
			glVertex2f(nowV->GetX(), nowV->GetY());
		}
	}

	glEnd();

}
