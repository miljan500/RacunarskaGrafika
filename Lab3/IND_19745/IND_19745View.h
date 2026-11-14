
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
	int gridStep;
	int gridEnd;
	DImage *images;

	void Translate(CDC* pdc, float dX, float dY, bool rightMultiply);
	void Scale(CDC* pdc, float sX, float sY, bool rightMultiply);
	void Rotate(CDC* pdc, float angle, bool rightMultiply);
	void Mirror(CDC* pdc, bool mx, bool my, bool rightMultiply);

	void DrawPuzzlePiece(CDC* pDC, DImage& img, float x, float y, float angle, float sx, float sy, bool mx, bool my);
	void TransparentPuzzlePiece(CDC* pdc, DImage& img);
	void FilterColor(CBitmap* bitMap, int w, int h);
	void DrawGrid(CDC* pdc);

// Generated message map functions
protected:
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // debug version in IND_19745View.cpp
inline CIND19745Doc* CIND19745View::GetDocument() const
   { return reinterpret_cast<CIND19745Doc*>(m_pDocument); }
#endif

