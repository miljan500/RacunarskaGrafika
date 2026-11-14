
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
#include "DImage.h"

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
END_MESSAGE_MAP()

// CIND19745View construction/destruction

CIND19745View::CIND19745View() noexcept
{
	// TODO: add construction code here

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

void CIND19745View::Translate(CDC* pdc, float dX, float dY, bool rightMultiply) {
	XFORM t = {
		1, 0,
		0, 1,
		dX, dY
	};
	if (rightMultiply)
		pdc->ModifyWorldTransform(&t, MWT_RIGHTMULTIPLY);
	else
		pdc->ModifyWorldTransform(&t, MWT_LEFTMULTIPLY);
}

void CIND19745View::Scale(CDC* pdc, float sX, float sY, bool rightMultiply) {
	XFORM t = {
		sX, 0,
		0, sY,
		0, 0
	};
	if (rightMultiply)
		pdc->ModifyWorldTransform(&t, MWT_RIGHTMULTIPLY);
	else
		pdc->ModifyWorldTransform(&t, MWT_LEFTMULTIPLY);
}

#define ToRad(x) x*3.14/180

void CIND19745View::Rotate(CDC* pdc, float angle, bool rightMultiply) {
	angle = ToRad(angle);
	XFORM t = {
		cos(angle), sin(angle),
		-sin(angle), cos(angle),
		0, 0
	};
	if (rightMultiply)
		pdc->ModifyWorldTransform(&t, MWT_RIGHTMULTIPLY);
	else
		pdc->ModifyWorldTransform(&t, MWT_LEFTMULTIPLY);
}

void CIND19745View::Mirror(CDC* pdc, bool mx, bool my, bool rightMultiply) {
	int sX = mx ? -1 : 1;
	int sY = my ? -1 : 1;
	XFORM t = {
		sX, 0,
		0, sY,
		0, 0
	};
	if (rightMultiply)
		pdc->ModifyWorldTransform(&t, MWT_RIGHTMULTIPLY);
	else
		pdc->ModifyWorldTransform(&t, MWT_LEFTMULTIPLY);
}

void CIND19745View::DrawPuzzlePiece(CDC* pdc, DImage& img, float x, float y, float angle, float sx, float sy, bool mx, bool my) {
	int w = img.Width();
	int h = img.Height();

	CBitmap* image = img.GetBitmap();
	CBitmap mask1;
	mask1.CreateBitmap(w, h, 1, 1, NULL);

	CDC* srcdc = new CDC();
	CDC* dstdc = new CDC();
	srcdc->CreateCompatibleDC(pdc);
	dstdc->CreateCompatibleDC(pdc);

	CBitmap* old_src_bm = srcdc->SelectObject(image);
	CBitmap* old_dst_bm = dstdc->SelectObject(&mask1);

	COLORREF topcolor = srcdc->GetPixel(0, 0);
	COLORREF old_src_bk = srcdc->SetBkColor(topcolor);
	dstdc->BitBlt(0, 0, w, h, srcdc, 0, 0, SRCCOPY);

	COLORREF old_src_fg = srcdc->SetTextColor(RGB(255, 255, 255));
	srcdc->SetBkColor(RGB(0, 0, 0));
	srcdc->BitBlt(0, 0, w, h, dstdc, 0, 0, SRCAND);

	srcdc->SelectObject(old_src_bm);
	dstdc->SelectObject(old_dst_bm);
	srcdc->SetBkColor(old_src_bk);
	srcdc->SetTextColor(old_src_fg);
	delete srcdc;
	delete dstdc;

	CDC* memdc = new CDC();
	memdc->CreateCompatibleDC(pdc);

	CBitmap* old_mem_bm = memdc->SelectObject(&mask1);
	pdc->BitBlt(0, 0, w, h, memdc, 0, 0, SRCAND);
	memdc->SelectObject(image);
	pdc->BitBlt(0, 0, w, h, memdc, 0, 0, SRCPAINT);

	memdc->SelectObject(old_mem_bm);
	delete memdc;

}


void CIND19745View::OnDraw(CDC* pdc)
{
	CIND19745Doc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// PRIKAZ BITMAPE
	
	//CBitmap cb1;
	//cb1.LoadBitmapW(310);
	//CDC* mdc = new CDC();
	//mdc->CreateCompatibleDC(pdc);
	//mdc->SelectObject(cb1);

	//pdc->BitBlt(0, 0, 256, 256, mdc, 0, 0, SRCCOPY);

	//delete mdc;


	// TRANSPARENTNOST
	
	//CBitmap image;
	//image.LoadBitmapW(310);

	//BITMAP bit_mask;
	//image.GetBitmap(&bit_mask);
	//int sX = bit_mask.bmWidth;
	//int sY = bit_mask.bmHeight;

	//CBitmap cbit_mask;
	//cbit_mask.CreateBitmap(sX, sY, 1, 1, NULL);

	//CDC* srcdc = new CDC();
	//srcdc->CreateCompatibleDC(pdc);
	//CDC* dstdc = new CDC();
	//dstdc->CreateCompatibleDC(pdc);

	//CBitmap* old_src = srcdc->SelectObject(&image);
	//CBitmap* old_dst = dstdc->SelectObject(&cbit_mask);

	//COLORREF colorTopLeft = srcdc->GetPixel(0, 0);
	//COLORREF old_color_bk = srcdc->SetBkColor(colorTopLeft);

	//dstdc->BitBlt(0, 0, bit_mask.bmWidth, bit_mask.bmHeight, srcdc, 0, 0, SRCCOPY);

	//COLORREF old_color_fg = srcdc->SetTextColor(RGB(255,255,255));
	//srcdc->SetBkColor(RGB(0, 0, 0));
	//srcdc->BitBlt(0, 0, sX, sX, dstdc, 0, 0, SRCAND);
	////pdc->BitBlt(0, 0, sX, sY, srcdc, 0, 0, SRCCOPY);

	//srcdc->SelectObject(old_src);
	//dstdc->SelectObject(old_dst);
	//srcdc->SetBkColor(old_color_bk);
	//srcdc->SetTextColor(old_color_fg);
	//delete srcdc;
	//delete dstdc;

	//CDC* memdc = new CDC();
	//memdc->CreateCompatibleDC(pdc);

	//CBitmap *oldbm = memdc->SelectObject(&cbit_mask);
	//pdc->BitBlt(0, 0, sX, sY, memdc, 0, 0, SRCAND);
	//memdc->SelectObject(&image);
	//pdc->BitBlt(0, 0, sX, sY, memdc, 0, 0, SRCPAINT);

	//memdc->SelectObject(oldbm);
	//delete memdc;

	DImage img1;
	img1.Load(CString("res/part1.bmp"));
	DrawPuzzlePiece(pdc, img1, 0, 0, 0, 1, 1, false, false);
	
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
