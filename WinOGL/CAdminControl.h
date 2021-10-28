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

	// ’¸“_‚ğ’Ç‰Á‚·‚éŠÖ”
	void SetVertex(double x,double y ,double ratio);
	void Draw();

private:
	CShape* shape_head;
	CShape* shape_tail;
	void FreeShape();
};

