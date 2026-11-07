
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
	m_bHit = false;
	m_rotStep = 5.6;
	m_rotBase = 0;
	m_rotLocal = 0;

	m_gridEnd = 500;
	m_gridStep = 25;

	m_bgEnd.x = 500;
	m_bgEnd.y = 500;

	m_pot1 = new CPoint[4]{
		{ int(m_gridStep * 8.4),  int(m_gridStep * 19.9) },
		{ int(m_gridStep * 8),  int(m_gridStep * 18) },
		{ int(m_gridStep * 12), int(m_gridStep * 18) },
		{ int(m_gridStep * 11.7), int(m_gridStep * 19.9) },
	};

	m_pot2 = new CPoint[4]{
		{ int(m_gridStep * 12.6),  int(m_gridStep * 18) },
		{ int(m_gridStep * 7.4),  int(m_gridStep * 18) },
		{ int(m_gridStep * 7.4), int(m_gridStep * 17.2) },
		{ int(m_gridStep * 12.6), int(m_gridStep * 17.2) },
	};


	m_cactusNodes = new CPoint[6]{
		{ int(m_gridStep * 10),  int(m_gridStep * 17) },
		{ int(m_gridStep * 10),  int(m_gridStep * 14) },
		{ int(m_gridStep * 7.9), int(m_gridStep * 11.9) },
		{ int(m_gridStep * 12.1), int(m_gridStep * 11.9) },
		{ int(m_gridStep * 15), int(m_gridStep * 11.9) },
		{ int(m_gridStep * 7.9), int(m_gridStep * 9) },
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

#define ToRad(x) (float)x*3.14159/180

void CIND19745View::Translate(CDC* pdc, float dX, float dY, bool rightMultiply) {
	XFORM t = {
		1,0,
		0,1,
		dX,dY
	};
	if (rightMultiply)
		pdc->ModifyWorldTransform(&t, MWT_RIGHTMULTIPLY);
	else
		pdc->ModifyWorldTransform(&t, MWT_LEFTMULTIPLY);
}

void CIND19745View::Scale(CDC* pdc, float sX, float sY, bool rightMultiply) {
	XFORM t = {
		sX,0,
		0,sY,
		0,0
	};
	if (rightMultiply)
		pdc->ModifyWorldTransform(&t, MWT_RIGHTMULTIPLY);
	else
		pdc->ModifyWorldTransform(&t, MWT_LEFTMULTIPLY);
}
void CIND19745View::Rotate(CDC* pdc, float angle, bool rightMultiply) {
	angle = ToRad(angle);
	XFORM t = {
	cos(angle), sin(angle),
	sin(-angle), cos(angle),
	0, 0
	};
	if (rightMultiply)
		pdc->ModifyWorldTransform(&t, MWT_RIGHTMULTIPLY);
	else
		pdc->ModifyWorldTransform(&t, MWT_LEFTMULTIPLY);
}

void CIND19745View::RotateAt(CDC* pdc, float cX, float cY, float angle, bool rightMultiply) {
	Translate(pdc, cX, cY, rightMultiply);
	Rotate(pdc, angle, rightMultiply);
	Translate(pdc, -cX, -cY, rightMultiply);
}

void CIND19745View::ScaleAt(CDC* pdc, float cX, float cY, float sX, float sY, bool rightMultiply) {
	Translate(pdc, cX, cY, rightMultiply);
	Scale(pdc, sX, sY, rightMultiply);
	Translate(pdc, -cX, -cY, rightMultiply);
}

void CIND19745View::DrawCactusNode(CDC* pdc, CPoint &cactusNode, int radius) {
	pdc->Ellipse(cactusNode.x - radius, cactusNode.y - radius, cactusNode.x + radius, cactusNode.y + radius);
}

void CIND19745View::DrawCactusPart(CDC* pdc, float cX, float cY, float sX, float angle, HENHMETAFILE cP){
	XFORM temp;
	pdc->GetWorldTransform(&temp);

	RECT rect = {
			  cX - 12, cY - 75,
			  cX + 12, cY
	};

	Translate(pdc, cX, cY, false);
	Rotate(pdc, angle, false);
	Scale(pdc, sX, 1, false);
	Translate(pdc, -cX, -cY, false);

	PlayEnhMetaFile(pdc->m_hDC, cP, &rect);
	
	pdc->SetWorldTransform(&temp);
}


void CIND19745View::DrawFigure(CDC* pdc) {

	CPen mainBlackPen(PS_SOLID, 1, RGB(0, 0, 0));
	pdc->SelectObject(&mainBlackPen);

	CBrush greenBrush(RGB(0, 204, 0));
	pdc->SelectObject(&greenBrush);

	int prevGraphicsMode = pdc->SetGraphicsMode(GM_ADVANCED);
	XFORM oldT, temp,temp1;
	pdc->GetWorldTransform(&oldT);

	RotateAt(pdc, m_cactusNodes[0].x, m_cactusNodes[0].y, m_rotBase, false);

	HENHMETAFILE cactusPartLight = GetEnhMetaFile(_T("cactus_part_light.emf"));
	HENHMETAFILE cactusPart = GetEnhMetaFile(_T("cactus_part.emf"));

	// Node0
	DrawCactusPart(pdc, m_cactusNodes[0].x, m_cactusNodes[0].y, 2.75, 0, cactusPartLight);

	// Node1
	DrawCactusPart(pdc, m_cactusNodes[1].x, m_cactusNodes[1].y, 1, 45, cactusPart);
	DrawCactusPart(pdc, m_cactusNodes[1].x, m_cactusNodes[1].y, 1, -45, cactusPart);

	// Node3
	DrawCactusPart(pdc, m_cactusNodes[3].x, m_cactusNodes[3].y, 1.8, 90, cactusPart);
	
	// Node4
	DrawCactusPart(pdc, m_cactusNodes[4].x, m_cactusNodes[4].y, 1.8, 45, cactusPart);
	DrawCactusPart(pdc, m_cactusNodes[4].x, m_cactusNodes[4].y, 1.8, 135, cactusPart);

	// Node2
	DrawCactusPart(pdc, m_cactusNodes[2].x, m_cactusNodes[2].y, 1.8, 0, cactusPart);
	DrawCactusPart(pdc, m_cactusNodes[2].x, m_cactusNodes[2].y, 1.8, -90, cactusPart);

	// Node5
	DrawCactusPart(pdc, m_cactusNodes[5].x, m_cactusNodes[5].y, 2.5, 0, cactusPart);

	// Node3 Light
	DrawCactusPart(pdc, m_cactusNodes[3].x, m_cactusNodes[3].y, 1.8, m_rotLocal, cactusPartLight);
	
	// Node1 Center
	DrawCactusPart(pdc, m_cactusNodes[1].x, m_cactusNodes[1].y, 1, 0, cactusPart);

	DeleteEnhMetaFile(cactusPartLight);
	DeleteEnhMetaFile(cactusPart);

	// Nodes
	for(int i=0;i<6;i++)
		DrawCactusNode(pdc,m_cactusNodes[i],m_gridStep/2);

	pdc->SetWorldTransform(&oldT);
	pdc->SetGraphicsMode(prevGraphicsMode);

	CBrush brownBrush(RGB(222, 148, 0));
	pdc->SelectObject(&brownBrush);

	// Pot
	pdc->Polygon(m_pot1, 4);
	pdc->Polygon(m_pot2, 4);
}

void CIND19745View::OnDraw(CDC* pdc)
{
	CIND19745Doc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	CBrush blueBrush(RGB(135, 206, 235));

	CPen* oldPen = (CPen*)pdc->SelectStockObject(NULL_PEN);
	CBrush* oldBrush = pdc->SelectObject(&blueBrush);

	// Background
	pdc->Rectangle(0, 0, m_bgEnd.x, m_bgEnd.y);

	// Figure
	DrawFigure(pdc);

	// Grid
	if (m_bHit) {
		CPen grayPen(PS_SOLID, 2, RGB(230, 230, 230));
		pdc->SelectObject(&grayPen);

		for (int i = 1; i < m_gridEnd; i += m_gridStep) {
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
	switch (nChar) {
		case 'G': m_bHit = !m_bHit; break;

		case VK_LEFT:  m_rotBase -= m_rotStep; break;
		case VK_RIGHT: m_rotBase += m_rotStep; break;

		case 'A': m_rotLocal -= m_rotStep; break;
		case 'D': m_rotLocal += m_rotStep; break;
	}
	Invalidate();

	CView::OnKeyDown(nChar, nRepCnt, nFlags);
}
