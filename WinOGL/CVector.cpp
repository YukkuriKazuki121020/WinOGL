#include "pch.h"
#include "CVector.h"
#include "CMath.h"

CVector::CVector()
{
	sp = new CVertex;
	ep = new CVertex;
	next_vector = NULL;
	SetStartPoint(0, 0);
	SetEndPoint(0, 0);
}

CVector::CVector(CVertex* sp, CVertex* ep)
{
	this->sp = sp;
	this->ep = ep;
	SetStartPoint(this->sp->GetX(), this->sp->GetY());
	SetEndPoint(ep->GetX(), ep->GetY());
	next_vector = NULL;
	selected_flag = false;
}

CVector::~CVector()
{
}

/* 始点と終点の設定と取得の関数 */

// 始点
void CVector::SetStartPoint(double x, double y)
{
	sp->SetX(x);
	sp->SetY(y);
}

void CVector::SetStartPoint(CVertex* sp)
{
	this->sp = sp;
}

CVertex* CVector::GetStartPoint()
{
	return sp;
}

// 終点
void CVector::SetEndPoint(double x, double y)
{
	ep->SetX(x);
	ep->SetY(y);
}

void CVector::SetEndPoint(CVertex* ep)
{
	this->ep = ep;
}

CVertex* CVector::GetEndPoint()
{
	return ep;
}

/* 位置ベクトルの計算関数 */
CVertex* CVector::CalcPositionVector()
{
	CVertex* p = new CVertex;
	p->SetX(this->ep->GetX() - this->sp->GetX());
	p->SetY(this->ep->GetY() - this->sp->GetY());
	return p;
}

double CVector::CalcVectorSize()
{
	return CMath::CalcDistance(this);
}

void CVector::SetNext(CVector* new_next)
{
	next_vector = new_next;
}

CVector* CVector::GetNext()
{
	return next_vector;
}

bool CVector::IsZeroVector()
{
	CVertex* pv = CalcPositionVector();
	return pv->GetX() == 0 && pv->GetY() == 0;
}

void CVector::SetSelectedFlag(bool selected_flag)
{
	this->selected_flag = selected_flag;
}

bool CVector::GetSelectedFlag()
{
	return selected_flag;
}

void CVector::Insert(CVector* insertVec)
{
	SetEndPoint(insertVec->GetStartPoint());
	insertVec->SetNext(next_vector);
	SetNext(insertVec);
}

