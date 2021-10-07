#include "pch.h"
#include "CVertex.h"

CVertex::CVertex()
{
	x = 0;
	y = 0;
	next_vertex = NULL;
}

CVertex::CVertex(int new_x, int new_y, CVertex* new_next)
{
	SetXY(new_x, new_y);
	SetNext(new_next);
}

CVertex::~CVertex()
{

}

//　頂点のX座標を書き込む
void CVertex::SetX(int new_x)
{
	if (new_x < 0) {
		new_x = new_x * (-1);
	}
	x = new_x;
}

//　頂点のY座標を書き込む
void CVertex::SetY(int new_y)
{
	if (new_y < 0) {
		new_y = new_y * (-1);
	}
	y = new_y;
}

//　頂点のX・Y座標の両方を書き込む
void CVertex::SetXY(int new_x, int new_y)
{
	if (new_x < 0) {
		new_x = new_x * (-1);
	}
	x = new_x;
	if (new_y < 0) {
		new_y = new_y * (-1);
	}
	y = new_y;
}

//　頂点のX座標を読み込む込む
int CVertex::GetX()
{
	return x;
}

//　頂点のY座標を読み込む
int CVertex::GetY()
{
	return y;
}

//　頂点のX・Y座標の両方を読み込む
void CVertex::GetXY(int* get_x, int* get_y)
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