#pragma once


enum class FX_TYPE
{
	FADE_IN,
	FADE_OUT,
	NONE,
};

struct tFXEvent
{
	FX_TYPE eType;
	float	fTime;
	float	fCurTime;
};

class CCamera
{
	SINGLE(CCamera);
private:
	Vec2	m_vLookAt;
	Vec2	m_vDiff;
	float	m_fCamSpeed;

	list<tFXEvent> m_listFX;

	HBITMAP m_hVeil;
	HDC		m_memDC;
	float	m_fRatio;


public:
	void init();
	void update();
	void render(HDC _dc);

public:
	void FadeIn(float _fTime)
	{
		tFXEvent tFX = {};
		tFX.eType = FX_TYPE::FADE_IN;	
		tFX.fTime = _fTime;
		m_listFX.push_back(tFX);		
	}

	void FadeOut(float _fTime)
	{
		tFXEvent tFX = {};
		tFX.eType = FX_TYPE::FADE_OUT;
		tFX.fTime = _fTime;
		m_listFX.push_back(tFX);
	}

public:
	Vec2 GetLookAt() { return m_vLookAt; }

	Vec2 GetRenderPos(Vec2 _vOriginPos)
	{
		return _vOriginPos - m_vDiff;
	}

	Vec2 GetRealPos(Vec2 _vRenderPos)
	{
		return _vRenderPos + m_vDiff;
	}

private:
	void ExcuteFX();
};

