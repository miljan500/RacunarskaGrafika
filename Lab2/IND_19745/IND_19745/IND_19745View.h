
// IND_19745View.h : interface of the CIND19745View class
//

#pragma once


class CIND19745View : public CView
{
protected: // create from serialization only
	CIND19745View() noexcept;
	DECLARE_DYNCREATE(CIND19745View)

// Attributes
public:
	CIND19745Doc* GetDocument() const;

// Operations
public:

// Overrides
public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// Implementation
public:
	virtual ~CIND19745View();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:
	bool m_bHit;
	int m_gridEnd;
	int m_gridStep;
	CPoint m_bgEnd;

	CPoint* m_pot1;
	CPoint* m_pot2;

	CPoint* m_cactusNodes;

	float m_rotStep;
	float m_rotBase;
	float m_rotLocal;

	void Translate(CDC* pDC, float dX, float dY, bool rightMultiply);
	void Scale(CDC* pDC, float sX, float sY, bool rightMultiply);
	void Rotate(CDC* pDC, float angle, bool rightMultiply);

	void RotateAt(CDC* pdc, float cX, float cY, float angle, bool rightMultiply);
	void ScaleAt(CDC* pdc, float cX, float cY, float sX, float sY, bool rightMultiply);

	void DrawCactusNode(CDC* pDC, CPoint& cactusNode,int radius);
	void DrawCactusPart(CDC* pDC, float cX, float cY, float sX, float angle, HENHMETAFILE cP);
	void DrawFigure(CDC* pDC);

// Generated message map functions
protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
};

#ifndef _DEBUG  // debug version in IND_19745View.cpp
inline CIND19745Doc* CIND19745View::GetDocument() const
   { return reinterpret_cast<CIND19745Doc*>(m_pDocument); }
#endif

