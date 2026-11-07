
// IND_19745.h : main header file for the IND_19745 application
//
#pragma once

#ifndef __AFXWIN_H__
	#error "include 'pch.h' before including this file for PCH"
#endif

#include "resource.h"       // main symbols


// CIND19745App:
// See IND_19745.cpp for the implementation of this class
//

class CIND19745App : public CWinApp
{
public:
	CIND19745App() noexcept;


// Overrides
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

// Implementation
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
};

extern CIND19745App theApp;
