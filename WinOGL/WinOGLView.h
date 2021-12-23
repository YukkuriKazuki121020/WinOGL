
// WinOGLView.h : CWinOGLView クラスのインターフェイス
//

#pragma once
#include <gl/GL.h>
#include "CAdminControl.h"


class CWinOGLView : public CView
{
protected: // シリアル化からのみ作成します。
	CWinOGLView() noexcept;
	DECLARE_DYNCREATE(CWinOGLView)

// 属性
public:
	CWinOGLDoc* GetDocument() const;

// 操作
public:

// オーバーライド
public:
	virtual void OnDraw(CDC* pDC);  // このビューを描画するためにオーバーライドされます。
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:

// 実装
public:
	virtual ~CWinOGLView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// 生成された、メッセージ割り当て関数
protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);

private:
	HGLRC m_hRC;
	double clickX;
	double clickY;
	double ratio;
	CAdminControl AC;

public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnDestroy();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnSize(UINT nType, int cx, int cy);

	afx_msg void OnXy();
	afx_msg void OnEdit();
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnMakeEdgeVertex();
	afx_msg void OnUpdateXy(CCmdUI* pCmdUI);
	afx_msg void OnUpdateEdit(CCmdUI* pCmdUI);
	afx_msg void OnUpdateMakeEdgeVertex(CCmdUI* pCmdUI);
	afx_msg void OnKillEdgeVertex();
	afx_msg void OnUpdateKillEdgeVertex(CCmdUI* pCmdUI);
	afx_msg void OnDebug();
	afx_msg void OnUpdateDebug(CCmdUI* pCmdUI);
	afx_msg void OnRotate();
	afx_msg void OnUpdateRotate(CCmdUI* pCmdUI);
	afx_msg void OnScale();
	afx_msg void OnUpdateScale(CCmdUI* pCmdUI);
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
};

#ifndef _DEBUG  // WinOGLView.cpp のデバッグ バージョン
inline CWinOGLDoc* CWinOGLView::GetDocument() const
   { return reinterpret_cast<CWinOGLDoc*>(m_pDocument); }
#endif

