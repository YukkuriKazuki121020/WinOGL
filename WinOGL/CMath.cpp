#include "pch.h"
#include "CMath.h"

#define _USE_MATH_DEFINES
#include <math.h>
#define CONNECT_DISTANCE 0.05

CMath::CMath()
{
}

CMath::~CMath()
{
}

double CMath::CalcDistance(CVector* v)
{
	return sqrt(pow(v->CalcPositionVector()->GetX(), 2) + pow(v->CalcPositionVector()->GetY(), 2));
}

bool CMath::JudgeDistance(CVertex* Ver, CVertex* clickPoint)
{
	return CVector(Ver, clickPoint).CalcVectorSize() <= CONNECT_DISTANCE;
}

bool CMath::JudgeSelectedForPoint(CVertex* Ver, CVertex* clickPoint)
{
	if (JudgeDistance(Ver, clickPoint)) {
		Ver->SetSelectedFlag(true);
		return true;
	}
	return false;
}

bool CMath::JudgeSelectedForLine(CVector* Vec, CVertex* clickPoint)
{
	if (JudgeSelectedForPoint(Vec->GetStartPoint(), clickPoint)) {// 始点の選択判定
		return false;
	}
	if (JudgeSelectedForPoint(Vec->GetEndPoint(), clickPoint)) {// 終点の選択判定
		return false;
	}
	CVector va(Vec->GetStartPoint(), clickPoint);
	CVector vb(Vec->GetEndPoint(), Vec->GetStartPoint());
	double d = CMath::CalcCrossProduct2d(va, vb) / vb.CalcVectorSize();

	va = CVector(Vec->GetStartPoint(), clickPoint);
	double s = CalcInnerProduct2d(va, *Vec) / CalcInnerProduct2d(*Vec, *Vec);
	if ((abs(d) <= CONNECT_DISTANCE) && (s >= 0 && s <= 1)) {
		Vec->SetSelectedFlag(true);
		return true;
	}
	else {
		return false;
	}
}

bool CMath::JudgeSelectedForShape(CShape* Shape, CVertex* clickPoint)
{
	if (Shape->GetEndVertexSet()) {//形状が閉じているとき
		for (CVector* nowVec = Shape->GetVectorHead(); nowVec != NULL; nowVec = nowVec->GetNext()) {
			if (JudgeSelectedForPoint(nowVec->GetStartPoint(), clickPoint)) {// 始点の選択判定
				Shape->SetSelectedFlag(false);
				return false;
			}
			if (JudgeSelectedForPoint(nowVec->GetEndPoint(), clickPoint)) {// 終点の選択判定
				Shape->SetSelectedFlag(false);
				return false;
			}
			if (JudgeSelectedForLine(nowVec, clickPoint)) {//辺の選択判定
				Shape->SetSelectedFlag(false);
				return false;
			}
		}
		if (IsInside(Shape, clickPoint)) {
			Shape->SetSelectedFlag(true);
			return true;
		}
		else {
			Shape->SetSelectedFlag(false);
			return false;
		}
	}
	return false;
}

CVertex* CMath::GetSelectedPoint(CShape* targetShape, CVertex* clickPoint)
{
	for (CVertex* nowVer = targetShape->GetVertexHead(); nowVer != NULL; nowVer = nowVer->GetNext()) {
		if (JudgeSelectedForPoint(nowVer, clickPoint)) {
			if (nowVer == targetShape->GetVertexHead()) {
				nowVer = targetShape->GetVertexTail();
			}
			return nowVer;
		}
	}
	return NULL;
}

CVector* CMath::GetSelectedLine(CShape* targetShape, CVertex* clickPoint)
{
	for (CVector* nowVec = targetShape->GetVectorHead(); nowVec != NULL; nowVec = nowVec->GetNext()) {
		if (JudgeSelectedForLine(nowVec, clickPoint)) {
			return nowVec;
		}
	}
	return NULL;
}

CShape* CMath::GetSelectedShape(CShape* shape_head, CVertex* clickPoint)
{
	for (CShape* nowShape = shape_head; nowShape != NULL; nowShape = nowShape->GetNext()) {
		if (JudgeSelectedForShape(nowShape, clickPoint)) {
			return nowShape;
		}
	}
	return NULL;
}

bool CMath::DamnAimChecker(CShape* targetShape, CVertex clickPoint)
{
	if (targetShape->GetVertexCnt() >= 3) { // 点の個数が三つ以上なら（最後クリックする点が一つ目の点とすると、多角形は点が三つ以上であるため。）
		if (JudgeDistance(targetShape->GetVertexHead(), &clickPoint)) { // クリックした座標と一つ目の点との距離が0.2以下なら
			return true;
		}
	}
	return false;
}

double CMath::CalcInnerProduct2d(CVector va, CVector vb)
{
	double x = va.CalcPositionVector()->GetX();
	double y = va.CalcPositionVector()->GetY();
	CVertex a(x, y);

	x = vb.CalcPositionVector()->GetX();
	y = vb.CalcPositionVector()->GetY();
	CVertex b(x, y);
	return a.GetX() * b.GetX() + a.GetY() * b.GetY();
}

double CMath::CalcCrossProduct2d(CVector va, CVector vb)
{
	double x = va.CalcPositionVector()->GetX();
	double y = va.CalcPositionVector()->GetY();
	CVertex a(x, y);

	x = vb.CalcPositionVector()->GetX();
	y = vb.CalcPositionVector()->GetY();
	CVertex b(x, y);
	return a.GetX() * b.GetY() - b.GetX() * a.GetY();
}

bool CMath::IsCrossing(CShape* targetShape, CVector vec)
{	
	for (CVector* nowVec = targetShape->GetVectorHead(); nowVec != NULL; nowVec = nowVec->GetNext()) {
		if (nowVec->GetEndPoint()->GetX() != vec.GetStartPoint()->GetX()
			&& nowVec->GetEndPoint()->GetY() != vec.GetStartPoint()->GetY()) {// 二つのベクトルの始点と終点が一致していなければ（自己交差の場合、同じ頂点を共有する辺同士は必ず交差するため）
			if (IsCrossingCore(*nowVec, vec)) {
				return true;
			}
		}
	}
	return false;
}

bool CMath::IsInside(CShape* targetShape, CVertex* innerVertex)
{
	double theta = 0; // θ（角度）
	CVertex* nowVer = targetShape->GetVertexHead();
	CVector va;
	CVector vb;
	do {
		va = CVector(innerVertex,nowVer);
		nowVer = nowVer->GetNext();
		vb = CVector(innerVertex, nowVer);
		theta += atan2(CalcCrossProduct2d(va, vb), CalcInnerProduct2d(va, vb));
	} while (nowVer->GetNext() != NULL);
	
	return abs(2 * M_PI - abs(theta)) <= 0.1;
}

bool CMath::IsInsideForAll(CShape* closingShape, CShape* targetShapes, CVertex* clickVertex)
{
	CShape* tmpShape = new CShape();// closingShapeにクリックした座標を頂点として追加した形状
	for (CVertex* nowVer = closingShape->GetVertexHead(); nowVer != NULL; nowVer = nowVer->GetNext()) {// closingShapeの頂点をコピー（頂点を追加していくとベクトルも追加されるので、頂点のコピーのみでよい）
		tmpShape->SetVertex(nowVer->GetX(), nowVer->GetY());
	}
	tmpShape->SetVertex(clickVertex->GetX(), clickVertex->GetY());
	for (CShape* nowS = targetShapes; nowS != closingShape; nowS = nowS->GetNext()) {
		for (CVertex* nowTargetShapeVer = nowS->GetVertexHead(); nowTargetShapeVer != NULL; nowTargetShapeVer = nowTargetShapeVer->GetNext()) {
			// nowTargetShapeVer:現在見ているtargetShapesの点を格納。
			if (IsInside(tmpShape, nowTargetShapeVer)) {
				return true;
			}
		}
	}
	return false;
}

bool CMath::IsCrossingCore(CVector va, CVector vb)
{
	CVector a1(va.GetStartPoint(), vb.GetStartPoint());
	CVector a2(va.GetStartPoint(), vb.GetEndPoint());
	CVector b1(vb.GetStartPoint(), va.GetStartPoint());
	CVector b2(vb.GetStartPoint(), va.GetEndPoint());

	double ca1 = CalcCrossProduct2d(va, a1);
	double ca2 = CalcCrossProduct2d(va, a2);
	double cb1 = CalcCrossProduct2d(vb, b1);
	double cb2 = CalcCrossProduct2d(vb, b2);
	if (ca1 * ca2 <= 0 && cb1 * cb2 <= 0) {
		return true;
	}
	else {
		return false;
	}
}
