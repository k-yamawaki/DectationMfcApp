
// DectationMfcApp.h : PROJECT_NAME アプリケーションのメイン ヘッダー ファイルです。
//

#pragma once

#ifndef __AFXWIN_H__
	#error "PCH に対してこのファイルをインクルードする前に 'stdafx.h' をインクルードしてください"
#endif

#include "resource.h"		// メイン シンボル


// CDectationMfcAppApp:
// このクラスの実装については、DectationMfcApp.cpp を参照してください。
//

class CDectationMfcAppApp : public CWinApp
{
public:
	CDectationMfcAppApp();

// オーバーライド
public:
	virtual BOOL InitInstance();

// 実装

	DECLARE_MESSAGE_MAP()
};

extern CDectationMfcAppApp theApp;