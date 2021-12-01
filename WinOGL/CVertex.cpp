#include "pch.h"
#include "CVertex.h"

CVertex::CVertex()
{
	//CVertex(0,0);
	SetXY(0, 0);
	SetNext(NULL);
	selected_flag = false;
}

CVertex::CVertex(double new_x, double new_y)
{
	//CVertex(new_x, new_y, NULL);
	SetXY(new_x, new_y);
	SetNext(NULL);
	selected_flag = false;
}

CVertex::CVertex(double new_x, double new_y, CVertex* new_next)
{
	SetXY(new_x, new_y);
	SetNext(new_next);
	selected_flag = false;
}

CVertex::~CVertex()
{

}

//　頂点のX座標を書き込む
void CVertex::SetX(double new_x)
{
	x = new_x;
}

//　頂点のY座標を書き込む
void CVertex::SetY(double new_y)
{
	y = new_y;
}

//　頂点のX・Y座標の両方を書き込む
void CVertex::SetXY(double new_x, double new_y)
{
	SetX(new_x);
	SetY(new_y);
}

//　頂点のX座標を読み込む込む
double CVertex::GetX()
{
	return x;
}

//　頂点のY座標を読み込む
double CVertex::GetY()
{
	return y;
}

//　頂点のX・Y座標の両方を読み込む
void CVertex::GetXY(double* get_x, double* get_y)
{
	get_x = &x;
	get_y = &y;
}

//　次の頂点リストを指すポインタを書き込む
void CVertex::SetNext(CVertex* new_next)
{
	next_vertex = new_next;
}

//　次の頂点リストを指すポインタを読み込む
CVertex* CVertex::GetNext()
{
	return next_vertex;
}

void CVertex::SetSelectedFlag(bool selected_flag)
{
	this->selected_flag = selected_flag;
}

bool CVertex::GetSelectedFlag()
{
	return selected_flag;
}

void CVertex::Insert(CVertex* insertVer)
{
	insertVer->SetNext(next_vertex);
	SetNext(insertVer);
}
