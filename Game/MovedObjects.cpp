#include "stdafx.h"
#include "Resource.h"
#include "MovedObjects.h"

void TVisibleObject::Show(CDC& dc) const
{
// для відображення графіки використано АРІ-функцію
	SelectObject(m_hMemoryDC, m_view);
	::MaskBlt(dc.m_hDC,m_x,m_y,m_width,m_height,m_hMemoryDC,0,0,m_mask,0,0,MAKEROP4(ROP_DstCopy,SRCCOPY));
}

//------------------------------------

TAircraft::TAircraft(int y, int s, HDC dC)
		: TMovedObject(1, y, 0, 0, s, 0, 0, dC)
{
	if (s < 3) m_craft = new SlowCraft(*this);
	else if (s < 6) m_craft = new UsualCraft(*this);
	else m_craft = new FastCraft(*this);
}

void TAircraft::Move()
{
	m_craft->Move();
}
void TAircraft::Crash()
{
	delete m_craft;
	m_craft = new KilledCraft(*this);
}
bool TAircraft::IsOut() const
{
	return m_craft->IsOut();
}
// перевірка влучання - порівняння координат об'єктів на співпадіння
bool TAircraft::HitBy(const TBullet& b) const
{
	return m_craft->HitBy(b);
}

//-------------------------------------

TBullet* TGun::Fire() const
{
	return new TBullet(m_x + 9, m_y - 4, m_hMemoryDC);
}

//-------------------------------------

HBITMAP TVisibleObject::FastView = 0;
HBITMAP TVisibleObject::FastMask = 0;
HBITMAP TVisibleObject::MidView = 0;
HBITMAP TVisibleObject::MidMask = 0;
HBITMAP TVisibleObject::SlowView = 0;
HBITMAP TVisibleObject::SlowMask = 0;
HBITMAP TVisibleObject::CrashView = 0;
HBITMAP TVisibleObject::CrashMask = 0;
HBITMAP TVisibleObject::BulletView = 0;
HBITMAP TVisibleObject::BulletMask = 0;
HBITMAP TVisibleObject::GunView = 0;
HBITMAP TVisibleObject::GunMask = 0;

int TVisibleObject::ScreenWidth = 100;
int TVisibleObject::ScreenHeight = 100;

// ініціалізація змінних класу - завантаження необхідних зображень з ресурсів аплікації
void TVisibleObject::InitBitmaps(HINSTANCE h)
{
	FastView = ::LoadBitmap(h,MAKEINTRESOURCE(IDB_FASTVIEW));
	FastMask = ::LoadBitmap(h,MAKEINTRESOURCE(IDB_FASTMASK));
	MidView = ::LoadBitmap(h,MAKEINTRESOURCE(IDB_MIDVIEW));
	MidMask = ::LoadBitmap(h,MAKEINTRESOURCE(IDB_MIDMASK));
	SlowView = ::LoadBitmap(h,MAKEINTRESOURCE(IDB_SLOWVIEW));
	SlowMask = ::LoadBitmap(h,MAKEINTRESOURCE(IDB_SLOWMASK));
	CrashView = ::LoadBitmap(h,MAKEINTRESOURCE(IDB_CRASHVIEW));
	CrashMask = ::LoadBitmap(h,MAKEINTRESOURCE(IDB_CRASHMASK));
	BulletView = ::LoadBitmap(h,MAKEINTRESOURCE(IDB_BULLETVIEW));
	BulletMask = ::LoadBitmap(h,MAKEINTRESOURCE(IDB_BULLETMASK));
	GunView = ::LoadBitmap(h,MAKEINTRESOURCE(IDB_GUNVIEW));
	GunMask = ::LoadBitmap(h,MAKEINTRESOURCE(IDB_GUNMASK));
}

void TVisibleObject::InitScreenSize(int m_width, int m_height)
{
	ScreenWidth = m_width;
	ScreenHeight = m_height;
}
