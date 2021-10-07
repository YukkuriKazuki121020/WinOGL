#include "pch.h"
#include "CAdminControl.h"

CAdminControl::CAdminControl()
{
	vertex_head = new CVertex();
}

CAdminControl::~CAdminControl()
{
}

void CAdminControl::SetVertex(double x, double y)
{
	vertex_head->SetNext(new CVertex(x, y, NULL));
}

void CAdminControl::Draw()
{

	glColor3f(1.0, 1.0, 1.0);
	
	glPointSize(10);
	glBegin(GL_POINTS);
	
	if (vertex_head->GetNext() != NULL) {
		glVertex2f(vertex_head->GetNext()->GetX(), vertex_head->GetNext()->GetY());
	}

	glEnd();

}
