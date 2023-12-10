// MapEditorView.h : interface of the CMapEditorView class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_MAPEDITORVIEW_H__6DED62D4_2689_4A57_8ECF_3938A36551A1__INCLUDED_)
#define AFX_MAPEDITORVIEW_H__6DED62D4_2689_4A57_8ECF_3938A36551A1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


class CMapEditorView : public CView
{
protected: // create from serialization only
	CMapEditorView();
	DECLARE_DYNCREATE(CMapEditorView)

// Attributes
public:
	CMapEditorDoc* GetDocument();

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMapEditorView)
	public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	protected:
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CMapEditorView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CMapEditorView)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // debug version in MapEditorView.cpp
inline CMapEditorDoc* CMapEditorView::GetDocument()
   { return (CMapEditorDoc*)m_pDocument; }
#endif

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MAPEDITORVIEW_H__6DED62D4_2689_4A57_8ECF_3938A36551A1__INCLUDED_)
