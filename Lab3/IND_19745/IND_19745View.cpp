
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
	images = new DImage[9];

	for (int i = 0; i < 9; i++)
	{
		CString path;
		path.Format(_T("res/part%d.bmp"), i );

		images[i].Load(path);
		FilterColor(images[i].GetBitmap(), images[i].Width(), images[i].Height());
	}

	gridStep = images[0].Width()/8;
	gridEnd = 16 * gridStep;
}

CIND19745View::~CIND19745View()
{
	delete[] images;
	images = NULL;
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

void CIND19745View::TransparentPuzzlePiece(CDC* pdc, DImage& img) {
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
	
	//FilterColor(image, w, h);
	memdc->SelectObject(image);

	pdc->BitBlt(0, 0, w, h, memdc, 0, 0, SRCPAINT);

	memdc->SelectObject(old_mem_bm);
	delete memdc;
}

void CIND19745View::FilterColor(CBitmap* bitMap,int w,int h) {

	BITMAP bm;
	bitMap->GetBitmap(&bm);

	int stride = bm.bmWidthBytes; 
	int bytesPerPixel = bm.bmBitsPixel / 8; 

	if (bytesPerPixel < 3)
		return;

	int size = stride * h;
	BYTE* pixels = new BYTE[size];

	bitMap->GetBitmapBits(size, pixels);

	for (int y = 0; y < h; y++) {
		BYTE* row = pixels + y * stride;
		for (int x = 0; x < w; x++) {
			int i = x * bytesPerPixel;

			BYTE B = row[i];
			BYTE G = row[i + 1];
			BYTE R = row[i + 2];

			if (R == 0 && G == 255 && B == 0)
				continue;

			int gray = 64 +(R + G + B) / 3;
			if (gray > 255)
				gray = 255;

			row[i] = (BYTE) gray;
			row[i + 1] = (BYTE) gray; 
			row[i + 2] = (BYTE) gray; 
		}
	}

	bitMap->SetBitmapBits(size, pixels);
	delete[] pixels;
}


void CIND19745View::DrawPuzzlePiece(CDC* pdc, DImage& img, float x, float y, float angle, float sx, float sy, bool mx, bool my) {
	
	int old_gmode = pdc->SetGraphicsMode(GM_ADVANCED);
	XFORM temp;
	pdc->GetWorldTransform(&temp);

	//Translate(pdc, x, y, false);
	//Scale(pdc, sx, sy, false);
	//
	//Translate(pdc, img.Width() / 2, img.Height() / 2, false);
	//Mirror(pdc, mx, my, false);
	//Rotate(pdc, angle, false);
	//Translate(pdc, -img.Width() / 2, -img.Height() / 2, false);

	Translate(pdc, x, y, false);

	Scale(pdc, sx, sy, false);

	Mirror(pdc, mx, my, false);
	Rotate(pdc, angle, false);

	Translate(pdc, -img.Width() / 2.0f, -img.Height() / 2.0f, false);


	//FilterColor(img.GetBitmap(), img.Width(), img.Height());
	TransparentPuzzlePiece(pdc, img);


	pdc->SetWorldTransform(&temp);
	pdc->SetGraphicsMode(old_gmode);
}


void CIND19745View::OnDraw(CDC* pdc)
{
	CIND19745Doc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	DrawGrid(pdc);

	float xstep = gridStep * 0.8;
	float ystep = gridStep * 0.9;

	DrawPuzzlePiece(pdc, images[1], xstep*10-15, ystep*9, -32.5-90, 1, 1, false, true);
	DrawPuzzlePiece(pdc, images[2], xstep*15, ystep*9, -107, 1, 1, false, true);
	DrawPuzzlePiece(pdc, images[8], xstep*4-11, ystep*9+4, -74, 1, 1, false, true);

	DrawPuzzlePiece(pdc, images[0], xstep*4+5, ystep*4, 114-90, 1, 1, false, true);
	DrawPuzzlePiece(pdc, images[6], xstep*10, ystep*4-2, 55, 1, 1, false, true);
	DrawPuzzlePiece(pdc, images[4], xstep*15+12, ystep*4-10, 105, 1, 1, false, true);

	DrawPuzzlePiece(pdc, images[3], xstep*4-10, ystep*15-30, -161, 1, 1, false, true);
	DrawPuzzlePiece(pdc, images[5], xstep*10-15, ystep*15-15, -67.5, 1, 1, false, true);
	DrawPuzzlePiece(pdc, images[7], xstep*15+12, ystep*15-13, -18, 1, 1, false, true);
}

void CIND19745View::DrawGrid(CDC* pdc) {
	CPen grayPen(PS_SOLID, 2, RGB(230, 230, 230));
	CPen *old = pdc->SelectObject(&grayPen);

	for (int i = 1; i < gridEnd; i += gridStep) {
		pdc->MoveTo(i, 0);
		pdc->LineTo(i, gridEnd);
		pdc->MoveTo(0, i);
		pdc->LineTo(gridEnd, i);
	}

	pdc->SelectObject(old);
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
