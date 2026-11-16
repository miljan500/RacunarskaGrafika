
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
	ON_WM_KEYDOWN()
END_MESSAGE_MAP()

// CIND19745View construction/destruction

CIND19745View::CIND19745View() noexcept
{
	MOVING_PIECE = 5;
	moving_dx = 500;
	moving_dy = 400;
	moving_step = 2;

	images = new DImage[9];
	for (int i = 0; i < 9; i++)
	{
		CString path;
		path.Format(_T("res/part%d.bmp"), i );

		images[i].Load(path);
	}

	gridStep = images[0].Width()/8;
	gridEnd = 16 * gridStep;
}

CIND19745View::~CIND19745View()
{
	delete[] images;
	images = NULL;
}


void CIND19745View::PreparePiece(CDC* pDC, int index)
{
	if (prepared[index])
		return;

	prepared[index] = true;

	DImage& img = images[index];

	int w = img.Width();
	int h = img.Height();
	int bpp = img.BPP();

	unsigned char* pixels = img.GetDIBBits();

	int keyB = pixels[0];
	int keyG = pixels[1];
	int keyR = pixels[2];

	masks[index].CreateBitmap(w, h, 1, 1, nullptr);

	CDC maskDC;
	maskDC.CreateCompatibleDC(pDC);

	CBitmap* oldMask =
		maskDC.SelectObject(&masks[index]);

	// Bela maska je providna oblast.
	// Crna maska je vidljivi deo slagalice.

	for (int y = 0; y < h; y++)
	{
		for (int x = 0; x < w; x++)
		{
			int i = (y * w + x) * bpp;

			bool transparent = false;

			int db = abs((int)pixels[i] - keyB);
			int dg = abs((int)pixels[i + 1] - keyG);
			int dr = abs((int)pixels[i + 2] - keyR);

			transparent = db < 25 && dg < 25 && dr < 25;

			maskDC.SetPixel( x, y, transparent ? RGB(255, 255, 255) : RGB(0, 0, 0));
		}
	}

	// Filtriranje nakon kreiranja maske

	if (index == MOVING_PIECE)
		MakeBlue(img);
	else
		MakeGray(img);

	colors[index].CreateCompatibleBitmap(pDC, w, h);

	CDC colorDC;
	colorDC.CreateCompatibleDC(pDC);

	CBitmap* oldColor = colorDC.SelectObject(&colors[index]);

	img.Draw(&colorDC,CRect(0, 0, w, h), CRect(0, 0, w, h));

	COLORREF oldText = colorDC.SetTextColor(RGB(255, 255, 255));

	COLORREF oldBk = colorDC.SetBkColor(RGB(0, 0, 0));

	colorDC.BitBlt(0, 0, w, h, &maskDC, 0, 0, SRCAND);

	colorDC.SetTextColor(oldText);
	colorDC.SetBkColor(oldBk);

	colorDC.SelectObject(oldColor);
	maskDC.SelectObject(oldMask);
}

void CIND19745View::MakeGray(DImage& img)
{
	unsigned char* pixels = img.GetDIBBits();

	if (!pixels || img.BPP() < 3)
		return;

	int w = img.Width();
	int h = img.Height();
	int bpp = img.BPP();

	for (int y = 0; y < h; y++)
	{
		for (int x = 0; x < w; x++)
		{
			int i = (y * w + x) * bpp;

			int b = pixels[i];
			int g = pixels[i + 1];
			int r = pixels[i + 2];

			int gr = 64 + (r + g + b) / 3;

			if (gr > 255)
				gr = 255;

			pixels[i] = (unsigned char)gr;
			pixels[i + 1] = (unsigned char)gr;
			pixels[i + 2] = (unsigned char)gr;
		}
	}

	img.Update();
}

// Plavi filter za pokretni deo

void CIND19745View::MakeBlue(DImage& img)
{
	unsigned char* pixels = img.GetDIBBits();

	if (!pixels || img.BPP() < 3)
		return;

	int w = img.Width();
	int h = img.Height();
	int bpp = img.BPP();

	for (int y = 0; y < h; y++)
	{
		for (int x = 0; x < w; x++)
		{
			int i = (y * w + x) * bpp;

			int b = pixels[i];
			int g = pixels[i + 1];
			int r = pixels[i + 2];

			int value = 64 + (r + g + b) / 3;

			if (value > 255)
				value = 255;

			pixels[i] = (unsigned char)value;
			pixels[i + 1] = 0;
			pixels[i + 2] = 0;
		}
	}

	img.Update();
}

BOOL CIND19745View::PreCreateWindow(CREATESTRUCT& cs)
{
	return CView::PreCreateWindow(cs);
}

void CIND19745View::Translate(CDC* pdc, float dX, float dY, bool rightMultiply) {
	XFORM t = {
		1, 0,
		0, 1,
		dX, dY
	};
	pdc->ModifyWorldTransform(&t, rightMultiply ? MWT_RIGHTMULTIPLY : MWT_LEFTMULTIPLY);
}

void CIND19745View::Scale(CDC* pdc, float sX, float sY, bool rightMultiply) {
	XFORM t = {
		sX, 0,
		0, sY,
		0, 0
	};
	pdc->ModifyWorldTransform(&t, rightMultiply ? MWT_RIGHTMULTIPLY : MWT_LEFTMULTIPLY);
}

#define ToRad(x) x*3.14/180

void CIND19745View::Rotate(CDC* pdc, float angle, bool rightMultiply) {
	angle = ToRad(angle);
	XFORM t = {
		cos(angle), sin(angle),
		-sin(angle), cos(angle),
		0, 0
	};
	pdc->ModifyWorldTransform(&t, rightMultiply ? MWT_RIGHTMULTIPLY : MWT_LEFTMULTIPLY);
}

void CIND19745View::Mirror(CDC* pdc, bool mx, bool my, bool rightMultiply) {
	int sX = mx ? -1 : 1;
	int sY = my ? -1 : 1;
	XFORM t = {
		sX, 0,
		0, sY,
		0, 0
	};
	pdc->ModifyWorldTransform(&t, rightMultiply ? MWT_RIGHTMULTIPLY : MWT_LEFTMULTIPLY);
}

void CIND19745View::TransparentPuzzlePiece(CDC* pdc, DImage& img)
{
	int index = -1;

	for (int i = 0; i < 9; i++)
	{
		if (&images[i] == &img)
		{
			index = i;
			break;
		}
	}

	if (index == -1)
		return;

	int w = img.Width();
	int h = img.Height();

	CDC maskDC, colorDC;

	maskDC.CreateCompatibleDC(pdc);
	colorDC.CreateCompatibleDC(pdc);

	CBitmap* oldMask = maskDC.SelectObject(&masks[index]);
	CBitmap* oldColor = colorDC.SelectObject(&colors[index]);

	COLORREF oldText = pdc->SetTextColor(RGB(255, 255, 255));
	COLORREF oldBk = pdc->SetBkColor(RGB(0, 0, 0));

	pdc->BitBlt(0, 0, w, h, &maskDC, 0, 0, SRCAND);

	pdc->BitBlt(0, 0, w, h, &colorDC, 0, 0, SRCPAINT);

	pdc->SetTextColor(oldText);
	pdc->SetBkColor(oldBk);

	colorDC.SelectObject(oldColor);
	maskDC.SelectObject(oldMask);
}

void CIND19745View::FilterColor(CBitmap* bitMap, int w, int h)
{
	BITMAP bm;
	bitMap->GetBitmap(&bm);

	int stride = bm.bmWidthBytes;
	int bytesPerPixel = bm.bmBitsPixel / 8;

	if (bytesPerPixel < 3)
		return;

	int size = stride * h;

	BYTE* pixels = new BYTE[size];

	bitMap->GetBitmapBits(size, pixels);

	for (int y = 0; y < h; y++)
	{
		BYTE* row = pixels + y * stride;

		for (int x = 0; x < w; x++)
		{
			int i = x * bytesPerPixel;

			BYTE B = row[i];
			BYTE G = row[i + 1];
			BYTE R = row[i + 2];

			int gray = 64 + (R + G + B) / 3;

			if (gray > 255)
				gray = 255;

			row[i] = (BYTE)gray;
			row[i + 1] = (BYTE)gray;
			row[i + 2] = (BYTE)gray;
		}
	}

	bitMap->SetBitmapBits(size, pixels);

	delete[] pixels;
}


void CIND19745View::DrawPuzzlePiece(CDC* pDC, DImage& img, float x, float y, float angle, float sx, float sy, bool mx, bool my)
{
	int index = -1;

	for (int i = 0; i < 9; i++)
	{
		if (&images[i] == &img)
		{
			index = i;
			break;
		}
	}

	int w = img.Width();
	int h = img.Height();

	int saved = pDC->SaveDC();

	pDC->SetGraphicsMode(GM_ADVANCED);

	Translate(pDC, x, y, false);
	Scale(pDC, sx, sy, false);
	Mirror(pDC, mx, my, false);
	Rotate(pDC, angle, false);
	Translate(pDC, -w / 2.0f, -h / 2.0f, false);

	CDC maskDC,colorDC;

	maskDC.CreateCompatibleDC(pDC);
	colorDC.CreateCompatibleDC(pDC);

	CBitmap* oldMask = maskDC.SelectObject(&masks[index]);

	CBitmap* oldColor = colorDC.SelectObject(&colors[index]);

	COLORREF oldText = pDC->SetTextColor(RGB(0, 0, 0));

	COLORREF oldBk = pDC->SetBkColor(RGB(255, 255, 255));

	pDC->SetStretchBltMode(COLORONCOLOR);

	// Transparentnost - maska
	pDC->StretchBlt(0, 0, w, h, &maskDC, 0, 0, w, h, SRCAND);

	// Transparentnost - slika
	pDC->StretchBlt(0, 0, w, h, &colorDC, 0, 0, w, h, SRCPAINT);

	pDC->SetTextColor(oldText);
	pDC->SetBkColor(oldBk);

	colorDC.SelectObject(oldColor);
	maskDC.SelectObject(oldMask);

	pDC->RestoreDC(saved);
}

void CIND19745View::OnDraw(CDC* pdc)
{
	CIND19745Doc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	CRect rect;
	GetClientRect(&rect);

	int width = rect.Width();
	int height = rect.Height();

	CDC memDC;
	memDC.CreateCompatibleDC(pdc);

	CBitmap backBuffer;
	backBuffer.CreateCompatibleBitmap(pdc, width, height);

	CBitmap* oldBuffer = memDC.SelectObject(&backBuffer);

	memDC.FillSolidRect(0, 0, width, height, RGB(255, 255, 255));

	for (int i = 0; i < 9; i++)
		PreparePiece(&memDC, i);

	DrawGrid(&memDC);

	float xstep = gridStep * 0.8;
	float ystep = gridStep * 0.9;

	DrawPuzzlePiece(&memDC, images[0], xstep * 4 + 5, ystep * 4, 114 - 90, 1, 1, false, true);
	DrawPuzzlePiece(&memDC, images[6], xstep*10, ystep*4-2, 55, 1, 1, false, true);
	DrawPuzzlePiece(&memDC, images[4], xstep*15+21, ystep*4-10, 105, 1, 1, false, true);

	DrawPuzzlePiece(&memDC, images[8], xstep * 4 - 11, ystep * 9 + 4, -74, 1, 1, false, true);
	DrawPuzzlePiece(&memDC, images[1], xstep * 10 - 15, ystep * 9, -32.5 - 90, 1, 1, false, true);
	DrawPuzzlePiece(&memDC, images[2], xstep * 15+6, ystep * 9, -107.7, 1, 1, false, true);

	DrawPuzzlePiece(&memDC, images[3], xstep*4-10, ystep*15-30, -161, 1, 1, false, true);
	//DrawPuzzlePiece(&memDC, images[5], xstep*10-15, ystep*15-17, -67.5, 1, 1, false, true);
	DrawPuzzlePiece(&memDC, images[7], xstep*15+15, ystep*15-14, -19, 1, 1, false, true);

	DrawPuzzlePiece(&memDC, images[5], moving_dx, moving_dy, -67.5, 1, 1, false, true);

	pdc->BitBlt(0, 0, width, height, &memDC, 0, 0, SRCCOPY);

	memDC.SelectObject(oldBuffer);
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

		case VK_LEFT:  moving_dx -= moving_step; break;
		case VK_RIGHT: moving_dx += moving_step; break;
		case VK_UP: moving_dy -= moving_step; break;
		case VK_DOWN: moving_dy += moving_step; break;
	}
	Invalidate(FALSE);

	CView::OnKeyDown(nChar, nRepCnt, nFlags);
}
