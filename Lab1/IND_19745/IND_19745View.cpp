
// IND_19745View.cpp : implementation of the CIND19745View class
//

#include "pch.h"
#include "framework.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "IND_19745.h"
#endif

#include "IND_19745Doc.h"
#include "IND_19745View.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CIND19745View

IMPLEMENT_DYNCREATE(CIND19745View, CView)

BEGIN_MESSAGE_MAP(CIND19745View, CView)
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)
	ON_WM_KEYDOWN()
END_MESSAGE_MAP()

// CIND19745View construction/destruction

CIND19745View::CIND19745View() noexcept
{
	// TODO: add construction code here
	m_bHit = false;

	m_gridEnd = 500;
	m_gridStep = 25;

	m_bgEnd.x = 500;
	m_bgEnd.y = 500;

	m_greenTriangle = new CPoint[3]{
	{ m_gridStep * 1, m_gridStep * 4 },
	{ m_gridStep * 5, m_gridStep * 4 },
	{ m_gridStep * 3, m_gridStep * 6 }
	};

	m_redTriangle = new CPoint[3]{
	{ m_gridStep * 3, m_gridStep * 6 },
	{ m_gridStep * 5, m_gridStep * 8 },
	{ m_gridStep * 1, m_gridStep * 8 }
	};

	m_orangeTriangle = new CPoint[3]{
	{ m_gridStep * 10, m_gridStep * 3 },
	{ m_gridStep * 7, m_gridStep * 6 },
	{ m_gridStep * 13, m_gridStep * 6 }
	};

	m_purpleTriangle = new CPoint[3]{
	{ m_gridStep * 7, m_gridStep * 6 },
	{ m_gridStep * 17, m_gridStep * 6 },
	{ m_gridStep * 12, m_gridStep * 11 }
	};

	m_pinkTriangle = new CPoint[3]{
	{ m_gridStep * 12, m_gridStep * 11 },
	{ m_gridStep * 19, m_gridStep * 4 },
	{ m_gridStep * 19, m_gridStep * 11 }
	};

	m_yellowRectangle = new CPoint[4]{
	{ m_gridStep * 19, m_gridStep * 11 },
	{ m_gridStep * 19, m_gridStep * 15 },
	{ m_gridStep * 17, m_gridStep * 17 },
	{ m_gridStep * 17, m_gridStep * 13 },
	};

}

CIND19745View::~CIND19745View()
{
}

BOOL CIND19745View::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

// CIND19745View drawing

void CIND19745View::DrawRegularPolygon(CDC* pdc, int cx, int cy, int r, int n, float rotAngle) {
	if (n < 3)
		return;
	POINT *points = new CPoint[n];
	float alfa = (float)360 / n;
	float alfaRad = alfa * 3.14159 / 180;

	for (int i = 0; i < n; i++) {
		points[i].x = r * cos(i*alfaRad); 
		points[i].y = r * sin(i*alfaRad);
	}

	int prevMode = pdc->SetGraphicsMode(GM_ADVANCED);

	float rotRad = rotAngle * 3.14159 / 180;
	XFORM t;
	t.eM11 =  cos(rotRad), t.eM12 = sin(rotRad);
	t.eM21 = -sin(rotRad), t.eM22 = cos(rotRad);
	t.eDx = cx, t.eDy = cy;

	XFORM oldT;
	pdc->GetWorldTransform(&oldT);
	pdc->ModifyWorldTransform(&t, MWT_RIGHTMULTIPLY);
	
	pdc->Polygon(points, n);

	pdc->SetWorldTransform(&oldT);
	pdc->SetGraphicsMode(prevMode);

	delete[] points;
}

void CIND19745View::DrawPolygon(CDC* pdc, CPoint* points, int n) {
	if (n < 3)
		return;
	pdc->Polygon(points, n);
}


void CIND19745View::OnDraw(CDC* pdc)
{
	CIND19745Doc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	CBrush greyBrush(RGB(221, 221, 211));

	CPen *oldPen =(CPen*) pdc->SelectStockObject(NULL_PEN);
	CBrush *oldBrush = pdc->SelectObject(&greyBrush);

	pdc->Rectangle(0, 0, m_bgEnd.x, m_bgEnd.y);

	int prevGraphicsMode = pdc->SetGraphicsMode(GM_ADVANCED);
	XFORM oldT;
	pdc->GetWorldTransform(&oldT);

	XFORM t;
	t.eM11 = 1, t.eM12 = 0;
	t.eM21 = 0, t.eM22 = 1;
	t.eDx = -2, t.eDy = -4;
	pdc->SetWorldTransform(&t);

	// Main pen
	CPen cyanPen(PS_SOLID, 6, RGB(21, 255, 255));
	pdc->SelectObject(&cyanPen);

	// Green triangle
	CBrush greenBrush(RGB(50, 205, 50));
	pdc->SelectObject(&greenBrush);
	DrawPolygon(pdc, m_greenTriangle, 3);

	// Red triangle
	CBrush redBrush(RGB(255, 0, 0));
	pdc->SelectObject(&redBrush);
	DrawPolygon(pdc, m_redTriangle, 3);

	// White square
	CBrush whiteHatchBrush(HS_BDIAGONAL,RGB(0, 0, 255));
	COLORREF oldBk = pdc->SetBkColor(RGB(255, 255, 255));
	pdc->SelectObject(&whiteHatchBrush);
	DrawRegularPolygon(pdc, 5.1*m_gridStep, 6* m_gridStep, 2* m_gridStep, 4, 0);
	pdc->SetBkColor(oldBk);

	// Orange triangle
	CBrush orangeBrush(RGB(255, 153, 51));
	pdc->SelectObject(&orangeBrush);
	DrawPolygon(pdc, m_orangeTriangle, 3);

	// Purple triangle
	CBrush purpleBrush(RGB(153, 0, 204));
	pdc->SelectObject(&purpleBrush);
	DrawPolygon(pdc, m_purpleTriangle, 3);

	// Pink triangle
	CBrush pinkBrush(RGB(255, 153, 204));
	pdc->SelectObject(&pinkBrush);
	DrawPolygon(pdc, m_pinkTriangle, 3);

	// Yellow rectangle
	CBrush yellowBrush(RGB(255, 255, 0));
	pdc->SelectObject(&yellowBrush);
	DrawPolygon(pdc, m_yellowRectangle, 4);

	// Regular polygons
	CPen cyanPen2(PS_SOLID, 3, RGB(0, 255, 255));
	pdc->SelectObject(&cyanPen2);
	pdc->SelectStockObject(NULL_BRUSH);

	// Green
	DrawRegularPolygon(pdc, m_gridStep * 3.1,    m_gridStep * 4.8, 12, 7, 30);

	// Red
	DrawRegularPolygon(pdc, m_gridStep * 3.1,   m_gridStep * 7.2, 12, 8, -15);

	// Orange
	DrawRegularPolygon(pdc, m_gridStep * 10, m_gridStep * 4.75, 18, 6, 0);

	// Purple
	DrawRegularPolygon(pdc, m_gridStep * 11.75, m_gridStep * 8, 27, 4, 90);
	
	// Pink
	DrawRegularPolygon(pdc, m_gridStep * 17, m_gridStep * 8.8, 27, 5, -7);

	pdc->SetWorldTransform(&oldT);
	pdc->SetGraphicsMode(prevGraphicsMode);

	if (m_bHit) {
		CPen grayPen(PS_SOLID, 2, RGB(230, 230, 230));
		pdc->SelectObject(&grayPen);

		for (int i = 1; i < m_gridEnd; i+=m_gridStep) {
			pdc->MoveTo(i, 0);
			pdc->LineTo(i, m_gridEnd);
			pdc->MoveTo(0, i);
			pdc->LineTo(m_gridEnd, i);
		}
	}

	pdc->SelectObject(oldPen);
	pdc->SelectObject(oldBrush);
}


// CIND19745View printing

BOOL CIND19745View::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CIND19745View::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CIND19745View::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}


// CIND19745View diagnostics

#ifdef _DEBUG
void CIND19745View::AssertValid() const
{
	CView::AssertValid();
}

void CIND19745View::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CIND19745Doc* CIND19745View::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CIND19745Doc)));
	return (CIND19745Doc*)m_pDocument;
}
#endif //_DEBUG


// CIND19745View message handlers

void CIND19745View::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	if (nChar == 'G') {
		m_bHit = !m_bHit;
		Invalidate();
	}

	CView::OnKeyDown(nChar, nRepCnt, nFlags);
}
