#include "pch.h"
#include <gl/GL.h>
#define _USE_MATH_DEFINES
#include <math.h>
#include <string.h>
#include "CShape.h"
#include "CMath.h"
#define NORMAL_POINT_SIZE 8
#define SELECTED_POINT_SIZE_MAGNIFICATION 2
#define NORMAL_LINE_WIDTH 1
#define SELECTED_LINE_WIDTH_MAGNIFICATION 4

CShape::CShape()
{
	vertex_cnt = 0;
	vector_cnt = 0;
	vertex_head = NULL;
	vertex_tail = NULL;
	next_shape = NULL;
	vector_head = NULL;
	vector_tail = vector_head;
	end_vertex_set = false;
	selected_flag = false;
	a_part_selectable = true;
	for_deformation_flag = false;
}

CShape::~CShape()
{
	FreeVertex();
}


void CShape::SetVertex(double x, double y)
{
	CVertex* newVer = new CVertex(x, y, NULL);
	if (vertex_head == NULL) {
		vertex_head = newVer;
	}
	else {
		vertex_tail->SetNext(newVer);
	}
	vertex_cnt++;
	if (vertex_cnt >= 2) {
		SetVector(newVer);
	}
	vertex_tail = newVer;
}

void CShape::DrawVertices()
{
	if (!for_deformation_flag){
		glPointSize(NORMAL_POINT_SIZE);
		glBegin(GL_POINTS);
		if (vertex_head != NULL) {
			for (CVertex* nowVer = vertex_head; nowVer != NULL; nowVer = nowVer->GetNext()) {
				glColor3f(1.0, 1.0, 1.0);
				glVertex2f(nowVer->GetX(), nowVer->GetY());
			}
			if (vertex_head->GetNext() != NULL) {

				double SkCogX = 0;
				double SkCogY = 0;
				double sumArea = 0;
				/*if (vertex_cnt >= 3) {
					for (CVertex* nowVer = vertex_head->GetNext(); nowVer->GetNext() != vertex_tail; nowVer = nowVer->GetNext()) {
						double cogX = 0;
						double cogY = 0;
						double area = 0;
						cogX = (vertex_head->GetX() + nowVer->GetX() + nowVer->GetNext()->GetX()) / 3;
						cogY = (vertex_head->GetY() + nowVer->GetY() + nowVer->GetNext()->GetY()) / 3;
						CVector va(vertex_head, nowVer);
						CVector vb(vertex_head, nowVer->GetNext());
						area = CMath::CalcCrossProduct2d(va, vb) / 2;
						sumArea += area;
						SkCogX += sumArea * cogX;
						SkCogY += sumArea * cogY;
					}
				}
				SkCogX /= sumArea;
				SkCogY /= sumArea;
				CVertex* cog = new CVertex(SkCogX, SkCogY);
				glVertex2d(cog->GetX(), cog->GetY());*/
			}
		}
		glEnd();
	}
	
}

void CShape::DrawLines()
{
	if(!for_deformation_flag){
		glLineWidth(NORMAL_LINE_WIDTH);
		glBegin(GL_LINES);
		for (CVector* nowVec = vector_head; nowVec != NULL; nowVec = nowVec->GetNext()) {
			glColor3f(1.0, 1.0, 1.0);
			glVertex2f(nowVec->GetStartPoint()->GetX(), nowVec->GetStartPoint()->GetY());
			glVertex2f(nowVec->GetEndPoint()->GetX(), nowVec->GetEndPoint()->GetY());
		}
		glEnd();
	}
	
}

void CShape::DrawSelectedVertices(CVertex* Ver)
{
	//点の描画
	if (Ver != NULL && !for_deformation_flag && Ver->GetSelectedFlag()) {
		glPointSize(NORMAL_POINT_SIZE * SELECTED_POINT_SIZE_MAGNIFICATION);
		glBegin(GL_POINTS);
		if (for_deformation_flag) {
			glColor3f(0.8, 0.5, 0.6);
		}
		else {
			glColor3f(1.0, 0, 0);
		}
		glVertex2f(Ver->GetX(), Ver->GetY());
		glEnd();
	}
}

void CShape::DrawSelectedLines(CVector* Vec) {
	//辺の描画
	if (Vec != NULL && !for_deformation_flag && Vec->GetSelectedFlag()) {
		glLineWidth(NORMAL_LINE_WIDTH * SELECTED_LINE_WIDTH_MAGNIFICATION);
		glEnable(GL_LINE_STIPPLE);
		glBegin(GL_LINES);
		if (for_deformation_flag) {
			glLineStipple(1, 0xF0F0);
			glColor3f(0.8, 0.5, 0.6);
		}
		else {
			glColor3f(1.0, 0, 0);
		}
		glColor3f(1.0, 0, 0);
		glVertex2f(Vec->GetStartPoint()->GetX(), Vec->GetStartPoint()->GetY());
		glVertex2f(Vec->GetEndPoint()->GetX(), Vec->GetEndPoint()->GetY());
		glEnd();
	}
}

void CShape::DrawSelectedShape()
{
	glLineWidth(NORMAL_LINE_WIDTH * SELECTED_LINE_WIDTH_MAGNIFICATION);
	glBegin(GL_LINES);
	for (CVector* nowVec = vector_head; nowVec != NULL; nowVec = nowVec->GetNext()) {
		if (for_deformation_flag) {
			glColor3f(0.8, 0.5, 0.6);
		}
		else {
			glColor3f(1.0, 0, 0);
		}
		glVertex2f(nowVec->GetStartPoint()->GetX(), nowVec->GetStartPoint()->GetY());
		glVertex2f(nowVec->GetEndPoint()->GetX(), nowVec->GetEndPoint()->GetY());
	}
	glEnd();
	glPointSize(NORMAL_POINT_SIZE * SELECTED_POINT_SIZE_MAGNIFICATION);
	glBegin(GL_POINTS);
	if (vertex_head != NULL) {
		for (CVertex* nowVer = vertex_head; nowVer != NULL; nowVer = nowVer->GetNext()) {
			if (for_deformation_flag) {
				glColor3f(0.8, 0.5, 0.6);
			}
			else {
				glColor3f(1.0, 0, 0);
			}
			glVertex2f(nowVer->GetX(), nowVer->GetY());
		}
	}
	glEnd();
}

void CShape::Draw(bool editFlag)
{
	CVector* selectedLine = NULL;
	CVertex* selectedVer = NULL;
	CShape* selectedShape = NULL;

	if (editFlag) {
		/*if (lButtonClicking && !dragging) {//マウスをドラッグしているときは選択されているかのフラグの初期化をしない
			for (CVector* nowVec = vector_head; nowVec != NULL; nowVec = nowVec->GetNext()) {
				nowVec->SetSelectedFlag(false);
				nowVec->GetStartPoint()->SetSelectedFlag(false);
				nowVec->GetEndPoint()->SetSelectedFlag(false);
				selected_flag = false;
			}
			//selectedLine = CMath::GetSelectedLine(this, clickPoint);
			//selectedVer = CMath::GetSelectedPoint(this, clickPoint);
			//selectedShape = CMath::GetSelectedShape(this, clickPoint);
		}*/
		for (CVector* nowVec = vector_head; nowVec != NULL; nowVec = nowVec->GetNext()) {
			if (nowVec->GetSelectedFlag()) {
				selectedLine = nowVec;
			}
		}
		for (CVertex* nowVer = vertex_head; nowVer != NULL; nowVer = nowVer->GetNext()) {
			if (nowVer->GetSelectedFlag()) {
				selectedVer = nowVer;
				break;
			}
		}
	}


	DrawLines();
	if (editFlag) {
		DrawSelectedLines(selectedLine);
	}
	
	DrawVertices();
	if (editFlag) {
		DrawSelectedVertices(selectedVer);
	}
	
	if (editFlag) {
		if (end_vertex_set) {
			if (selected_flag) {
				DrawSelectedShape();
			}
		}
	}
	
}

CVertex* CShape::GetVertexHead() {
	return vertex_head;
}

CVertex* CShape::GetVertexTail()
{
	return vertex_tail;
}

void CShape::SetNext(CShape* new_next) {
	next_shape = new_next;
}

CShape* CShape::GetNext()
{
	return next_shape;
}

CVector* CShape::GetVectorHead()
{
	return vector_head;
}

CVector* CShape::GetVectorTail()
{
	return vector_tail;
}

void CShape::DecideEndPoint(double x, double y) {
	CVertex* endV = new CVertex(vertex_head->GetX(), vertex_head->GetY());
	SetVertex(endV->GetX(), endV->GetY());// 終点を始点に
				
	end_vertex_set = true;
}

bool CShape::GetEndVertexSet()
{
	return end_vertex_set;
}

void CShape::SetSelectedFlag(bool selected_flag)
{
	this->selected_flag = selected_flag;
}

bool CShape::GetSelectedFlag()
{
	return selected_flag;
}

void CShape::SetAPartSelectable(bool a_part_selectable)
{
	this->a_part_selectable = a_part_selectable;
}

bool CShape::GetAPartSelectable()
{
	return a_part_selectable;
}

void CShape::SetForDeformationFlag(bool for_transformation_flag)
{
	this->for_deformation_flag = for_transformation_flag;
}

bool CShape::GetForDeformationFlag()
{
	return for_deformation_flag;
}

void CShape::SetAllSelectedFlag(bool all_selected_flag)
{
	for (CVertex* nowVer = vertex_head; nowVer != NULL; nowVer = nowVer->GetNext()) {
		nowVer->SetSelectedFlag(all_selected_flag);
	}
	for (CVector* nowVec = vector_head; nowVec != NULL; nowVec = nowVec->GetNext()) {
		nowVec->SetSelectedFlag(all_selected_flag);
	}
	selected_flag = all_selected_flag;
}

int CShape::GetVertexCnt() {
	return vertex_cnt;
}

int CShape::GetVectorCnt()
{
	return vector_cnt;
}

void CShape::MoveVertices(CVertex* clickPoint)
{
	for (CVector* nowVec = vector_head; nowVec != NULL; nowVec = nowVec->GetNext()) {
		if (nowVec->GetEndPoint()->GetSelectedFlag()) {
			if (CMath::IsCrossing(this, *nowVec)) {
				nowVec->GetEndPoint()->SetSelectedFlag(false);
				nowVec->GetEndPoint()->SetXY(nowVec->GetEndPoint()->GetX() - 4 * clickPoint->GetX(), nowVec->GetEndPoint()->GetY() - 4 * clickPoint->GetY());
				a_part_selectable = true;
				break;
			}
			if (nowVec == vector_tail) {
				if (CMath::IsCrossing(this, *vector_head)) {
					vector_head->GetStartPoint()->SetSelectedFlag(false);
					vector_tail->GetEndPoint()->SetXY(vector_tail->GetEndPoint()->GetX() - 4 * clickPoint->GetX(), vector_tail->GetEndPoint()->GetY() - 4 * clickPoint->GetY());
					vector_head->GetStartPoint()->SetXY(vector_head->GetStartPoint()->GetX() - 4 *clickPoint->GetX(), vector_head->GetStartPoint()->GetY() - 4 * clickPoint->GetY());
					a_part_selectable = true;
					break;
				}
			}
			else {
				if (CMath::IsCrossing(this, *nowVec->GetNext())) {
					nowVec->GetNext()->GetStartPoint()->SetSelectedFlag(false);
					nowVec->GetNext()->GetStartPoint()->SetXY(nowVec->GetNext()->GetStartPoint()->GetX() - 4 * clickPoint->GetX(), nowVec->GetNext()->GetStartPoint()->GetY() - 4 * clickPoint->GetY());
					a_part_selectable = true;
					break;
				}
			}
			nowVec->GetEndPoint()->SetXY(nowVec->GetEndPoint()->GetX() + clickPoint->GetX(), nowVec->GetEndPoint()->GetY() + clickPoint->GetY());
			if (nowVec->GetEndPoint() == vertex_tail) {
				vertex_head->SetXY(vertex_head->GetX() + clickPoint->GetX(), vertex_head->GetY() + clickPoint->GetY());
			}
			break;
		}
	}
}

void CShape::MoveLines(CVertex* clickPoint)
{
	if (vertex_cnt >= 4) {
		for (CVector* nowVec = vector_head; nowVec != vector_tail; nowVec = nowVec->GetNext()) {
			CVertex* sp = NULL;
			CVertex* ep = NULL;
			if (nowVec == vector_head && vector_head->GetSelectedFlag()) {
				if (CMath::IsCrossing(this, *vector_tail)
					|| CMath::IsCrossing(this, *vector_head)
					|| CMath::IsCrossing(this, *vector_head->GetNext())) {
					vector_head->SetSelectedFlag(false);
					vector_head->SetStartPoint(vector_head->GetStartPoint()->GetX() - 4 * clickPoint->GetX(), vector_head->GetStartPoint()->GetY() - 4 * clickPoint->GetY());
					vector_head->SetEndPoint(vector_head->GetEndPoint()->GetX() - 4 * clickPoint->GetX(), vector_head->GetEndPoint()->GetY() - 4 * clickPoint->GetY());
					vector_tail->SetEndPoint(vector_head->GetEndPoint()->GetX() - 4 * clickPoint->GetX(), vector_head->GetEndPoint()->GetY() - 4 * clickPoint->GetY());
					a_part_selectable = true;
					break;
				}
				sp = vector_head->GetStartPoint();
				ep = vector_head->GetEndPoint();
			}
			else if (nowVec->GetNext() == vector_tail && vector_tail->GetSelectedFlag()) {
				if (CMath::IsCrossing(this, *nowVec)
					|| CMath::IsCrossing(this, *vector_tail)
					|| CMath::IsCrossing(this, *vector_head)) {
					vector_tail->SetSelectedFlag(false);
					vector_tail->SetStartPoint(vector_tail->GetStartPoint()->GetX() - 4 * clickPoint->GetX(), vector_tail->GetStartPoint()->GetY() - 4 * clickPoint->GetY());
					vector_tail->SetEndPoint(vector_tail->GetEndPoint()->GetX() - 4 * clickPoint->GetX(), vector_tail->GetEndPoint()->GetY() - 4 * clickPoint->GetY());
					vector_head->SetStartPoint(vector_head->GetStartPoint()->GetX() - 4 * clickPoint->GetX(), vector_head->GetStartPoint()->GetY() - 4 * clickPoint->GetY());
					a_part_selectable = true;
					break;
				}
				sp = vector_tail->GetStartPoint();
				ep = vector_tail->GetEndPoint();
			}
			else if (nowVec->GetNext()->GetSelectedFlag()) {
				if (CMath::IsCrossing(this, *nowVec)
					|| CMath::IsCrossing(this, *nowVec->GetNext())
					|| CMath::IsCrossing(this, *nowVec->GetNext()->GetNext())) {
					nowVec->GetNext()->SetSelectedFlag(false);
					nowVec->GetNext()->SetStartPoint(nowVec->GetNext()->GetStartPoint()->GetX() - 4 * clickPoint->GetX(), nowVec->GetNext()->GetStartPoint()->GetY() - 4 * clickPoint->GetY());
					nowVec->GetNext()->SetEndPoint(nowVec->GetNext()->GetEndPoint()->GetX() - 4 * clickPoint->GetX(), nowVec->GetNext()->GetEndPoint()->GetY() - 4 * clickPoint->GetY());
					a_part_selectable = true;
					break;
				}
				sp = nowVec->GetNext()->GetStartPoint();
				ep = nowVec->GetNext()->GetEndPoint();
			}
			if (sp != NULL && ep != NULL) {
				sp->SetXY(sp->GetX() + clickPoint->GetX(), sp->GetY() + clickPoint->GetY());
				if (sp == vertex_head) {
					vertex_tail->SetXY(vertex_tail->GetX() + clickPoint->GetX(), vertex_tail->GetY() + clickPoint->GetY());
				}
				ep->SetXY(ep->GetX() + clickPoint->GetX(), ep->GetY() + clickPoint->GetY());
				if (ep == vertex_tail) {
					vertex_head->SetXY(vertex_head->GetX() + clickPoint->GetX(), vertex_head->GetY() + clickPoint->GetY());
				}
				break;
			}
		}
	}
}

void CShape::MoveShape(CVertex* clickPoint)
{
	if (selected_flag) {
		for (CVertex* nowVer = vertex_head; nowVer != NULL; nowVer = nowVer->GetNext()) {
			nowVer->SetXY(nowVer->GetX() + clickPoint->GetX(), nowVer->GetY() + clickPoint->GetY());
		}
	}
}

void CShape::Move(CVertex* clickPoint)
{
	MoveVertices(clickPoint);
	MoveLines(clickPoint);
	MoveShape(clickPoint);
}

void CShape::MEV(CVertex* clickPoint)
{
	CVector* selectedVec = CMath::GetSelectedLine(this, clickPoint);
	if (selectedVec != NULL) {
		selectedVec->GetStartPoint()->Insert(clickPoint);
		CVector* insertVec = new CVector(clickPoint, selectedVec->GetEndPoint());
		selectedVec->Insert(insertVec);
		if (selectedVec == vector_tail) {
			vector_tail = insertVec;
		}
		vertex_cnt++;
		vector_cnt++;
	}
}

void CShape::KEV(CVertex* clickPoint)
{
	CVertex* selectedVer = CMath::GetSelectedPoint(this, clickPoint);
	CVector* preVec = NULL;
	for (CVector* nowVec = vector_head; nowVec != NULL; nowVec = nowVec->GetNext()) {
		CVector* deleteVec = nowVec;
		if (selectedVer == vertex_tail) {
			selectedVer = vertex_head;
		}
		if (deleteVec->GetStartPoint() == selectedVer) {
			if (deleteVec == vector_head) {
				// ↓選択された点を始点とする辺の削除↓
				vector_tail->SetEndPoint(vector_head->GetEndPoint()->GetX(), vector_head->GetEndPoint()->GetY());
				vector_head = deleteVec->GetNext();
				delete deleteVec;
				// ↑選択された点を始点とする辺の削除↑

				// ↓選択された点の削除↓
				vertex_head = selectedVer->GetNext();
				vertex_tail->SetXY(vertex_head->GetX(), vertex_head->GetY());
				delete selectedVer;
				// ↑選択された点の削除↑

				vertex_cnt--;
				vector_cnt--;
				break;
			}
			else {
				// ↓選択された点を始点とする辺の削除↓
				preVec->SetEndPoint(deleteVec->GetEndPoint());
				preVec->SetNext(deleteVec->GetNext());
				if (preVec->GetNext() == NULL) {
					vector_tail = preVec;
				}
				delete deleteVec;
				// ↑選択された点を始点とする辺の削除↑

				// ↓選択された点の削除↓
				preVec->GetStartPoint()->SetNext(selectedVer->GetNext());
				delete selectedVer;
				// ↑選択された点の削除↑

				vertex_cnt--;
				vector_cnt--;
				break;
			}
		}
		preVec = nowVec;
	}
}

void CShape::Scale(const char* mode)
{
	// cog：center of gravity（重心）
	/* 計算方法：
	http://izumi-math.jp/F_Nakamura/heso/heso3.htm
	https://examist.jp/mathematics/planar-vector/triangle-vector/
	*/

	/*double cogX = 0;
	double cogY = 0;
	double sumArea = 0;
	for (CVertex* nowVer = vertex_head->GetNext(); nowVer->GetNext() != NULL; nowVer = nowVer->GetNext()) {
		cogX += (vertex_head->GetX() + nowVer->GetX() + nowVer->GetNext()->GetX()) / 3;
		cogY += (vertex_head->GetY() + nowVer->GetY() + nowVer->GetNext()->GetY()) / 3;
		CVector va(vertex_head, nowVer);
		CVector vb(vertex_head, nowVer->GetNext());
		sumArea += CMath::CalcCrossProduct2d(va, vb) / 2;
	}
	cogX -= vertex_head->GetX();
	cogX /= sumArea;
	cogY -= vertex_head->GetY();
	cogY /= sumArea;
	*/
	CVertex* cog = new CVertex(0, 0);
	
	for (CVertex* nowVer = vertex_head; nowVer != NULL; nowVer = nowVer->GetNext()) {
		CVector vec(cog, nowVer);// 基点を始点、拡縮する形状の現在見ている頂点を終点としたベクトルを生成
		CVertex* pv = vec.CalcPositionVector();// 位置ベクトル
		if (strstr(mode, "expanding")) {
			//pv->SetXY(pv->GetX() * 0.1, pv->GetY() * 0.1);// 拡大用の増分の位置ベクトル
			nowVer->SetXY(nowVer->GetX() * 1.05, nowVer->GetY() * 1.05);
		}
		else if (strstr(mode, "shrinking")) {
			//pv->SetXY(pv->GetX() * -0.1, pv->GetY() * -0.1);// 縮小用の増分の位置ベクトル
			nowVer->SetXY(nowVer->GetX() * 0.95, nowVer->GetY() * 0.95);
		}
		

		// 頂点の座標を変更
		//nowVer->SetXY(nowVer->GetX() + pv->GetX(), nowVer->GetY() + pv->GetY());
	}
}

void CShape::Rotate(CVertex* clickPoint, const char* mode)
{
	double rad = 180.0 / M_PI;
	double theta = 0;
	if (strstr(mode, "right")) {
		theta = -rad / 10;
	}
	else if (strstr(mode, "left")) {
		theta = rad / 10;
	}
	CVertex* clickVer = CMath::GetSelectedPoint(this, clickPoint);
	if (clickVer != NULL) {
		for (CVertex* nowVer = vertex_head; nowVer != NULL; nowVer = nowVer->GetNext()) {
			double x = (nowVer->GetX() - clickPoint->GetX()) * cos(theta) - (nowVer->GetY() - clickPoint->GetY()) * sin(theta) + clickPoint->GetX();
			double y = (nowVer->GetX() - clickPoint->GetX()) * sin(theta) + (nowVer->GetY() - clickPoint->GetY()) * cos(theta) + clickPoint->GetY();
			nowVer->SetXY(x, y);
		}
	}
}

void CShape::Clone(CShape* cloneShape)
{
	CVector* cNowVec = NULL;
	for (CVector* nowVec = vector_head; nowVec != NULL; nowVec = nowVec->GetNext()) {// closingShapeの頂点をコピー（頂点を追加していくとベクトルも追加されるので、頂点のコピーのみでよい）
		if (vertex_cnt > cloneShape->GetVertexCnt()) {
			cloneShape->SetVertex(nowVec->GetStartPoint()->GetX(), nowVec->GetStartPoint()->GetY());
			cloneShape->GetVertexTail()->SetSelectedFlag(nowVec->GetStartPoint()->GetSelectedFlag());
			if (cloneShape->GetVectorCnt() >= 1) {
				cloneShape->GetVectorTail()->SetSelectedFlag(nowVec->GetSelectedFlag());
			}
			if (nowVec == vector_tail) {
				if (CMath::DamnAimChecker(cloneShape, *(nowVec->GetEndPoint()))) {
					cloneShape->DecideEndPoint(nowVec->GetEndPoint()->GetX(), nowVec->GetEndPoint()->GetY());
					cloneShape->GetVertexTail()->SetSelectedFlag(nowVec->GetEndPoint()->GetSelectedFlag());
				}
				else if (nowVec->GetNext() == NULL) {
					cloneShape->SetVertex(nowVec->GetEndPoint()->GetX(), nowVec->GetEndPoint()->GetY());
					cloneShape->GetVertexTail()->SetSelectedFlag(nowVec->GetEndPoint()->GetSelectedFlag());
				}
			}
		}
		else {
			if (nowVec == vector_head) {
				cNowVec = cloneShape->GetVectorHead();
			}
			cNowVec->GetStartPoint()->SetXY(nowVec->GetStartPoint()->GetX(), nowVec->GetStartPoint()->GetY());
			cNowVec->GetStartPoint()->SetSelectedFlag(nowVec->GetStartPoint()->GetSelectedFlag());
			cNowVec->SetSelectedFlag(nowVec->GetSelectedFlag());
			if (nowVec == vector_tail) {
				if (cNowVec == cloneShape->GetVectorTail()) {
					cNowVec->GetEndPoint()->SetXY(nowVec->GetEndPoint()->GetX(), nowVec->GetEndPoint()->GetY());
					cNowVec->GetEndPoint()->SetSelectedFlag(nowVec->GetEndPoint()->GetSelectedFlag());
				}
				else {
					while (cNowVec != cloneShape->GetVectorTail()) {
						cloneShape->KEV(cNowVec->GetEndPoint());
					}
					cloneShape->GetVertexTail()->SetXY(cloneShape->GetVertexHead()->GetX(), cloneShape->GetVertexHead()->GetY());
				}
			}
			cNowVec = cNowVec->GetNext();
		}
	}
	cloneShape->SetSelectedFlag(selected_flag);
}

void CShape::SetVector(CVertex* newVer) {
	CVector* newVec = new CVector(vertex_tail, newVer);
	if (vector_head == NULL) {
		vector_head = newVec;
		vector_tail = vector_head;
	}
	else {
		vector_tail->SetNext(newVec);
		vector_tail = vector_tail->GetNext();
	}
	vector_cnt++;
}

//　動的確保したVertexを解放する
void CShape::FreeVertex()
{
	CVertex* temp = NULL;
	CVertex* vp = vertex_head;
	while (vertex_head != NULL)
	{
		temp = vertex_head->GetNext();
		delete vertex_head;
		vertex_head = temp;
	}
}