
// WinOGLView.cpp : CWinOGLView クラスの実装
//

#include "pch.h"
#include "framework.h"
// SHARED_HANDLERS は、プレビュー、縮小版、および検索フィルター ハンドラーを実装している ATL プロジェクトで定義でき、
// そのプロジェクトとのドキュメント コードの共有を可能にします。
#ifndef SHARED_HANDLERS
#include "WinOGL.h"
#endif

#include "WinOGLDoc.h"
#include "WinOGLView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CWinOGLView

IMPLEMENT_DYNCREATE(CWinOGLView, CView)

BEGIN_MESSAGE_MAP(CWinOGLView, CView)
	ON_WM_LBUTTONDOWN()
	ON_WM_CREATE()
	ON_WM_DESTROY()
	ON_WM_ERASEBKGND()
	ON_WM_SIZE()
	ON_COMMAND(ID_XY, &CWinOGLView::OnXy)
	ON_COMMAND(ID_EDIT, &CWinOGLView::OnEdit)
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_COMMAND(ID_MAKE_EDGE_VERTEX, &CWinOGLView::OnMakeEdgeVertex)
	ON_UPDATE_COMMAND_UI(ID_XY, &CWinOGLView::OnUpdateXy)
	ON_UPDATE_COMMAND_UI(ID_EDIT, &CWinOGLView::OnUpdateEdit)
	ON_UPDATE_COMMAND_UI(ID_MAKE_EDGE_VERTEX, &CWinOGLView::OnUpdateMakeEdgeVertex)
	ON_COMMAND(ID_KILL_EDGE_VERTEX, &CWinOGLView::OnKillEdgeVertex)
	ON_UPDATE_COMMAND_UI(ID_KILL_EDGE_VERTEX, &CWinOGLView::OnUpdateKillEdgeVertex)
	ON_COMMAND(ID_DEBUG, &CWinOGLView::OnDebug)
	ON_UPDATE_COMMAND_UI(ID_DEBUG, &CWinOGLView::OnUpdateDebug)
END_MESSAGE_MAP()

// CWinOGLView コンストラクション/デストラクション

CWinOGLView::CWinOGLView() noexcept
{
	clickX = 0;
	clickY = 0;
}

CWinOGLView::~CWinOGLView()
{
}

BOOL CWinOGLView::PreCreateWindow(CREATESTRUCT& cs)
{

	return CView::PreCreateWindow(cs);
}

// CWinOGLView 描画

void CWinOGLView::OnDraw(CDC* pDC)
{
	CWinOGLDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	wglMakeCurrent(pDC->m_hDC, m_hRC);
	glClearColor(0.0, 0.0, 0.0, 1.0);
	glClear(GL_COLOR_BUFFER_BIT /* | GL_DEPTH_BUFFER_BIT */);
	
	AC.Draw(clickX, clickY);

	//glEnd();//ここまで描画の合図

	glFlush();
	SwapBuffers(pDC->m_hDC);

	wglMakeCurrent(pDC->m_hDC, NULL);
}


// CWinOGLView の診断

#ifdef _DEBUG
void CWinOGLView::AssertValid() const
{
	CView::AssertValid();
}

void CWinOGLView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CWinOGLDoc* CWinOGLView::GetDocument() const // デバッグ以外のバージョンはインラインです。
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CWinOGLDoc)));
	return (CWinOGLDoc*)m_pDocument;
}
#endif //_DEBUG


// CWinOGLView メッセージ ハンドラー


void CWinOGLView::OnLButtonDown(UINT nFlags, CPoint point)
{
	CRect rect;
	GetClientRect(rect); // 描画領域の大きさを取得

	int w = rect.Width();
	int h = rect.Height();

	// 正規化変換
	clickX = (double)point.x / (double)w;
	clickY = (double)point.y / (double)h;
	clickY = 1 - clickY;
	clickX = clickX * 2 - 1;
	clickY = clickY * 2 - 1;

	ratio = 1;

	// 縦長のとき
	if (w > h) {
		ratio = (double)w / h;
		clickX = clickX * ratio;
		glOrtho(-ratio, ratio, -1, 1, -100, 100);
	}
	// 横長のとき
	else {
		ratio = (double)h / w;
		clickY = clickY * ratio;
		glOrtho(-1, 1, -ratio, ratio, -100, 100);
	}
	AC.SetAllSelectedFlag(false);

	AC.MEV(clickX, clickY);
	AC.KEV(clickX, clickY);

	AC.lButtonClicking = true;
	AC.SetVertex(clickX, clickY ,ratio);

	RedrawWindow();

	CView::OnLButtonDown(nFlags, point);
}


void CWinOGLView::OnMouseMove(UINT nFlags, CPoint point)
{
	if (AC.lButtonClicking) {
		AC.dragging = true;
	}
	else {
		AC.dragging = false;
	}

	if (AC.editFlag) {
		if(AC.dragging){
			double oldClickX = clickX;
			double oldClickY = clickY;
			CRect rect;
			GetClientRect(rect); // 描画領域の大きさを取得

			int w = rect.Width();
			int h = rect.Height();

			// 正規化変換
			clickX = (double)point.x / (double)w;
			clickY = (double)point.y / (double)h;
			clickY = 1 - clickY;
			clickX = clickX * 2 - 1;
			clickY = clickY * 2 - 1;

			ratio = 1;

			// 縦長のとき
			if (w > h) {
				ratio = (double)w / h;
				clickX = clickX * ratio;
				glOrtho(-ratio, ratio, -1, 1, -100, 100);
			}
			// 横長のとき
			else {
				ratio = (double)h / w;
				clickY = clickY * ratio;
				glOrtho(-1, 1, -ratio, ratio, -100, 100);
			}

			double dClickX = clickX - oldClickX;// clickXの移動量
			double dClickY = clickY - oldClickY;// clickYの移動量

			AC.Move(dClickX, dClickY);
			AC.Draw(clickX, clickY);
		}
	}
	RedrawWindow();

	CView::OnMouseMove(nFlags, point);
}


void CWinOGLView::OnLButtonUp(UINT nFlags, CPoint point)
{
	AC.lButtonClicking = false;
	AC.dragging = false;

	CView::OnLButtonUp(nFlags, point);
}


int CWinOGLView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CView::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO: ここに特定な作成コードを追加してください。

	PIXELFORMATDESCRIPTOR pfd =
	{
		sizeof(PIXELFORMATDESCRIPTOR),
		1,
		PFD_DRAW_TO_WINDOW |
		PFD_SUPPORT_OPENGL |
		PFD_DOUBLEBUFFER,
		PFD_TYPE_RGBA,
		32,
		0,0,0,0,0,0,
		0,0,0,0,0,0,0,
		24,
		0,0,
		PFD_MAIN_PLANE,
		0,
		0,0,0
	};

	CClientDC clientDC(this);
	int pixelFormat = ChoosePixelFormat(clientDC.m_hDC, &pfd);
	SetPixelFormat(clientDC.m_hDC, pixelFormat, &pfd);
	m_hRC = wglCreateContext(clientDC.m_hDC);

	return 0;
}


void CWinOGLView::OnDestroy()
{
	CView::OnDestroy();

	wglDeleteContext(m_hRC);
}


BOOL CWinOGLView::OnEraseBkgnd(CDC* pDC)
{
	return true;
}


void CWinOGLView::OnSize(UINT nType, int cx, int cy)
{
	CView::OnSize(nType, cx, cy);
	CClientDC clientDC(this);
	wglMakeCurrent(clientDC.m_hDC, m_hRC);

	glViewport(0, 0, cx, cy);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	// 問6.2
	ratio = 1;

	// 縦長のとき
	if (cx < cy) {
		ratio = (double)cy / cx;
		glOrtho(-1, 1, -ratio, ratio, -100, 100);
	}
	// 横長のとき
	else{
		ratio = (double)cx / cy;
		glOrtho(-ratio, ratio, -1, 1, -100, 100);
	}

	glMatrixMode(GL_MODELVIEW);

	RedrawWindow();
	wglMakeCurrent(clientDC.m_hDC,NULL);
	
}


void CWinOGLView::OnEdit()
{
	AC.editFlag = !AC.editFlag;
	AC.mevFlag = false;
	AC.kevFlag = false;
	clickX = 10;
	clickY = 10;
}


void CWinOGLView::OnUpdateEdit(CCmdUI* pCmdUI)
{
	// AC.editFlagがtrueのときボタンが沈む
	if (AC.editFlag) {
		pCmdUI->SetCheck(true);
	}
	else {
		pCmdUI->SetCheck(false);
	}
}


void CWinOGLView::OnXy()
{
	AC.axisFlag = !AC.axisFlag;

	RedrawWindow();
}


void CWinOGLView::OnUpdateXy(CCmdUI* pCmdUI)
{
	// AC.axisFlagがtrueのときボタンが沈む
	if (AC.axisFlag) {
		pCmdUI->SetCheck(true);
	}
	else {
		pCmdUI->SetCheck(false);
	}
}


void CWinOGLView::OnMakeEdgeVertex()
{
	AC.mevFlag = !AC.mevFlag;
	if (AC.mevFlag) {
		AC.kevFlag = false;
	}
}


void CWinOGLView::OnUpdateMakeEdgeVertex(CCmdUI* pCmdUI)
{
	// AC.mevFlagがtrueのときボタンが沈む
	if (AC.mevFlag) {
		pCmdUI->SetCheck(true);
	}
	else {
		pCmdUI->SetCheck(false);
	}
}


void CWinOGLView::OnKillEdgeVertex()
{
	AC.kevFlag = !AC.kevFlag;
	if (AC.kevFlag) {
		AC.mevFlag = false;
	}
}


void CWinOGLView::OnUpdateKillEdgeVertex(CCmdUI* pCmdUI)
{
	// AC.mevFlagがtrueのときボタンが沈む
	if (AC.kevFlag) {
		pCmdUI->SetCheck(true);
	}
	else {
		pCmdUI->SetCheck(false);
	}
}


void CWinOGLView::OnDebug()
{
	AC.debugFlag = true;
	AC.Debug();
}


void CWinOGLView::OnUpdateDebug(CCmdUI* pCmdUI)
{
	// AC.mevFlagがtrueのときボタンが沈む
	if (AC.debugFlag) {
		pCmdUI->SetCheck(true);
	}
	else {
		pCmdUI->SetCheck(false);
	}
}
