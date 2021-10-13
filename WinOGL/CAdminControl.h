#pragma once
#include <gl/GL.h>
#include "CVertex.h"
#include "CShape.h"

class CAdminControl
{
public :
	CAdminControl();
	~CAdminControl();

	// ’¸“_‚ğ’Ç‰Á‚·‚éŠÖ”
	void SetVertex(double x,double y ,double raito);
	void Draw();

private:
	CShape* shape_keeper;
	void FreeShape();
};

