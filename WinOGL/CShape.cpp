#include "pch.h"
#include <gl/GL.h>
#define _USE_MATH_DEFINES
#include <math.h>
#include "CShape.h"
#include "CMath.h"

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
	selected_flag = false;
	a_part_selectable = true;
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
	glPointSize(8);
	glBegin(GL_POINTS);
	if (vertex_head != NULL) {
		for (CVertex* nowVer = vertex_head; nowVer != NULL; nowVer = nowVer->GetNext()) {
			glColor3f(1.0, 1.0, 1.0);
			glVertex2f(nowVer->GetX(), nowVer->GetY());
		}
	}
	glEnd();
}

void CShape::DrawLines()
{
	glBegin(GL_LINES);
	for (CVector* nowVec = vector_head; nowVec != NULL; nowVec = nowVec->GetNext()) {
		glColor3f(1.0, 1.0, 1.0);
		glVertex2f(nowVec->GetStartPoint()->GetX(), nowVec->GetStartPoint()->GetY());
		glVertex2f(nowVec->GetEndPoint()->GetX(), nowVec->GetEndPoint()->GetY());
	}
	glEnd();
}

void CShape::DrawSelectedVertices(CVertex* Ver)
{
	//点の描画
	glPointSize(8);
	glBegin(GL_POINTS);
	glColor3f(1.0, 0, 0);
	if (Ver != NULL && Ver->GetSelectedFlag()) {
		glVertex2f(Ver->GetX(), Ver->GetY());
	}	
	glEnd();
}

void CShape::DrawSelectedLines(CVector* Vec) {
	//辺の描画

	glBegin(GL_LINES);
	glColor3f(1.0, 0, 0);
	if (Vec != NULL && Vec->GetSelectedFlag()) {
		glVertex2f(Vec->GetStartPoint()->GetX(), Vec->GetStartPoint()->GetY());
		glVertex2f(Vec->GetEndPoint()->GetX(), Vec->GetEndPoint()->GetY());
	}
	glEnd();
}

void CShape::DrawSelectedShape()
{
	glPointSize(8);
	glBegin(GL_POINTS);
	if (vertex_head != NULL) {
		for (CVertex* nowVer = vertex_head; nowVer != NULL; nowVer = nowVer->GetNext()) {
			glColor3f(1.0, 0, 0);
			glVertex2f(nowVer->GetX(), nowVer->GetY());
		}
	}
	glEnd();
	glBegin(GL_LINES);
	for (CVector* nowVec = vector_head; nowVec != NULL; nowVec = nowVec->GetNext()) {
		glColor3f(1.0, 0, 0);
		glVertex2f(nowVec->GetStartPoint()->GetX(), nowVec->GetStartPoint()->GetY());
		glVertex2f(nowVec->GetEndPoint()->GetX(), nowVec->GetEndPoint()->GetY());
	}
	glEnd();
}

void CShape::Draw(CVertex* clickPoint, bool editFlag, bool dragging)
{
	if (editFlag) {
		if (!dragging) {//マウスをドラッグしているときは選択されているかのフラグの初期化をしない
			for (CVector* nowVec = vector_head; nowVec != NULL; nowVec = nowVec->GetNext()) {
				nowVec->SetSelectedFlag(false);
				nowVec->GetStartPoint()->SetSelectedFlag(false);
				nowVec->GetEndPoint()->SetSelectedFlag(false);
			}
		}
	}

	CVector* selectedLine = CMath::GetSelectedLine(this, clickPoint);
	CVertex* selectedVer = CMath::GetSelectedPoint(this, clickPoint);
	CShape* selectedShape = CMath::GetSelectedShape(this, clickPoint);

	DrawLines();
	if (editFlag) {
		if (a_part_selectable) {//どこか(頂点、辺、面のいずれか)ひとつでも選択できるとき
			if (selectedLine != NULL) {
				a_part_selectable = false;
			}
		}
		DrawSelectedLines(selectedLine);
	}
	
	DrawVertices();
	if (editFlag) {
		if (a_part_selectable) {//どこか(頂点、辺、面のいずれか)ひとつでも選択できるとき
			if (selectedVer != NULL) {
				a_part_selectable = false;
			}
		}
		DrawSelectedVertices(selectedVer);
	}
	
	if (editFlag) {
		if (a_part_selectable) {//どこか(頂点、辺、面のいずれか)ひとつでも選択できるとき
			if (end_vertex_set) {
				if (selectedShape != NULL) {
					a_part_selectable = false;
					DrawSelectedShape();
				}
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

void CShape::SetAllSelectedFlag(bool all_selected_flag)
{
	for (CVertex* nowVer = vertex_head; nowVer != NULL; nowVer = nowVer->GetNext()) {
		nowVer->SetSelectedFlag(all_selected_flag);
	}
	for (CVector* nowVec = vector_head; nowVec != NULL; nowVec = nowVec->GetNext()) {
		nowVec->SetSelectedFlag(all_selected_flag);
	}
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
	for (CVertex* nowVer = vertex_head; nowVer != NULL; nowVer = nowVer->GetNext()) {
		if (nowVer->GetSelectedFlag()) {
			nowVer->SetXY(nowVer->GetX() + clickPoint->GetX(), nowVer->GetY() + clickPoint->GetY());
		}
	}
}

void CShape::Move(CVertex* clickPoint)
{
	MoveVertices(clickPoint);
}

void CShape::MEV(CVertex* clickPoint)
{
	CVector* selectedVec = CMath::GetSelectedLine(this, clickPoint);
	if (selectedVec != NULL) {
		selectedVec->GetStartPoint()->Insert(clickPoint);
		CVector* insertVec = new CVector(clickPoint, selectedVec->GetEndPoint());
		selectedVec->Insert(insertVec);
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
				preVec->SetEndPoint(nowVec->GetEndPoint());
				preVec->SetNext(nowVec->GetNext());
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

void CShape::Clone(CShape* cloneShape)
{
	for (CVertex* nowVer = vertex_head; nowVer != NULL; nowVer = nowVer->GetNext()) {// closingShapeの頂点をコピー（頂点を追加していくとベクトルも追加されるので、頂点のコピーのみでよい）
		cloneShape->SetVertex(nowVer->GetX(), nowVer->GetY());
	}
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