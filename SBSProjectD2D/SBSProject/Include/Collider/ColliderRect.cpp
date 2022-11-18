#include "ColliderRect.h"
#include "ColliderPixel.h"
#include "../Object/Obj.h"
#include "../Core/Camera.h"

CColliderRect::CColliderRect()
{
	m_eCollType = CT_RECT;
}

CColliderRect::CColliderRect(const CColliderRect & coll) :
	CCollider(coll)
{
	m_tInfo = coll.m_tInfo;
}

CColliderRect::~CColliderRect()
{
}

void CColliderRect::SetRect(float l, float t, float r, float b)
{
	m_tInfo.l = l;
	m_tInfo.t = t;
	m_tInfo.r = r;
	m_tInfo.b = b;
}

void CColliderRect::SetWorldInfo(float l, float t, float r, float b)
{
	m_tWorldInfo.l = l;
	m_tWorldInfo.t = t;
	m_tWorldInfo.r = r;
	m_tWorldInfo.b = b;
}

bool CColliderRect::Init()
{
	return true;
}

void CColliderRect::Input(float fDeltaTime)
{
	CCollider::Input(fDeltaTime);
}

int CColliderRect::Update(float fDeltaTime)
{
	CCollider::Update(fDeltaTime);
	return 0;
}

int CColliderRect::LateUpdate(float fDeltaTime)
{
	CCollider::LateUpdate(fDeltaTime);

	POSITION tPos = m_pObj->GetPos();
	m_tWorldInfo.l = tPos.x + m_tInfo.l;
	m_tWorldInfo.t = tPos.y + m_tInfo.t;
	m_tWorldInfo.r = tPos.x + m_tInfo.r;
	m_tWorldInfo.b = tPos.y + m_tInfo.b;

	return 0;
}

bool CColliderRect::Collision(CCollider * pDest)
{
	switch (pDest->GetColliderType())
	{
	case CT_RECT:
		return CollisionRectToRect(m_tWorldInfo, ((CColliderRect*)pDest)->GetWorldInfo());
	case CT_PIXEL:
		CColliderPixel* pPixColl = (CColliderPixel*)pDest;
		return CollisionRectToPixel(m_tWorldInfo, pPixColl->GetPos(), pPixColl->GetPixel(), pPixColl->GetWidth(), pPixColl->GetHeight(), pPixColl->GetHitColor());
	}
	return false;
}

void CColliderRect::Render(ID2D1GdiInteropRenderTarget* pGDIRenderTarget, ID2D1HwndRenderTarget* pRenderTarget, float fDeltaTime)
{
	CCollider::Render(pGDIRenderTarget, pRenderTarget, fDeltaTime);

	ID2D1SolidColorBrush* pBrush;
	pRenderTarget->CreateSolidColorBrush(ColorF(ColorF::Black), &pBrush);

	// 카메라 위치를 고려하여 위치 조정
	POSITION tPos = GET_SINGLE(CCamera)->GetPos();

	// tPos 기준 texture 사각형 범위
	D2D1_RECT_F tRect;
	tRect.left = m_tWorldInfo.l - tPos.x;
	tRect.top = m_tWorldInfo.t - tPos.y;
	tRect.right = m_tWorldInfo.r - tPos.x;
	tRect.bottom = m_tWorldInfo.b - tPos.y;

	pRenderTarget->DrawRectangle(&tRect, pBrush);

	pBrush->Release();
}

CColliderRect * CColliderRect::Clone()
{
	return new CColliderRect(*this);
}
