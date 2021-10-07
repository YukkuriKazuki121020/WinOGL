#pragma once
#include <gl/GL.h>
#include "CVertex.h"

class CAdminControl
{
public :
	CAdminControl();
	~CAdminControl();

	// ’¸“_‚ğ’Ç‰Á‚·‚éŠÖ”
	void SetVertex(double x,double y);
	void Draw();

private:
	CVertex* vertex_head;
};

