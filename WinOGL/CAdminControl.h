#pragma once
#include <gl/GL.h>
#include "CVertex.h"

class CAdminControl
{
public :
	CAdminControl();
	~CAdminControl();

	void Draw();
private:
	CVertex* vertex_head;
};

