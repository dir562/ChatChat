#pragma once

class CObj;

class CCore
{
	SINGLE(CCore);

private:
	HWND	m_hWnd;
	POINT	m_ptResolution;
	HDC		m_dc;

	// 더블 버퍼링 용 백버퍼
	HBITMAP	m_backbuffer;
	HDC		m_memDC;

	HPEN	m_arrPen[(UINT)PEN_TYPE::END];
	

public:
	HWND GetHwnd() { return m_hWnd; }
	HDC GetMainDC() { return m_dc; }
	HDC GetMemDC() { return m_memDC; }
	POINT GetResolution() { return m_ptResolution; }
	HPEN GetPen(PEN_TYPE _eType) { return m_arrPen[(UINT)_eType]; }
	void ChangeWindowSize(POINT _ptLT, POINT _ptResolution, bool _bMenu);


public:
	void init(HWND _hWnd, POINT _ptResolution);
	void progress();
		
private:
	void update();
	void render();
	void CreateGDIObject();


};

