/*        Ієрархія рухомих            TVisibleObject
               об'єктів            TGun         TMovedObject
                                            TBullet        TAircraft

Абстрактний базовий клас TVisibleObject 
- задає структуру для зберігання даних об'єкта:
 координати, розміри, швидкість переміщення, бітмапку зовнішнього вигляду,
 а також, маску і посилання на контекст пристрою, потрібні для виведення графіки
- вказує, що всі об'єкти вміють себе зображати.
- статичні елементи класу потрібні для швидкого створення екземплярів

Клас TGun:TVisibleObject зображає гармату, моделює дискретні переміщення
 ліворуч-праворуч та можливість пострілу - породження кулі

Проміжний абстрактний клас TMovedObject:TVisibleObject описує
- вміння об'єктів рухатись самостійно (міняти координати)
- можливість вильоту за межі вікна

Клас TBullet:TMovedObject зображає кулю і
- конкретизує спосіб переміщення: вертикально
- конкретизує вихід за межі вікна: за верхню межу
- розширяє можливості базового класу: повідомляє свої координати

Клас TAircraft:TMovedObject зображає літак і
- конкретизує спосіб переміщення: горизонтально
- конкретизує вихід за межі вікна: за праву межу
- розширяє можливості базового класу: перевірка влучання кулі
       та перехід до підбитого стану
*/

#pragma once

class TVisibleObject
{
	// "секретна" константа для виведення зображення з прозорими цятками
	enum {ROP_DstCopy = 0x00AA0029};

protected:
	int m_x, m_y;			// координати лівого верхнього кута зображення
	int m_width, m_height;	// розміри зображення
	int m_step;			// швидкість переміщення (крок за одиницю часу)
	HBITMAP m_view, m_mask;	// власне зображення (вигляд і монохромна маска)
	HDC m_hMemoryDC;		// контекст для зображення

	static int ScreenWidth;  // розміри графічного екрана -
	static int ScreenHeight; // вікна застосунку

	// графічні зображення об'єктів потрібно завантажити з ресурсів аплікації
	// і використовувати для створення екземплярів
	static HBITMAP FastView;
	static HBITMAP FastMask;
	static HBITMAP MidView;
	static HBITMAP MidMask;
	static HBITMAP SlowView;
	static HBITMAP SlowMask;
	static HBITMAP CrashView;
	static HBITMAP CrashMask;
	static HBITMAP BulletView;
	static HBITMAP BulletMask;
	static HBITMAP GunView;
	static HBITMAP GunMask;
public:
	// завантажує зображення
	static void InitBitmaps(HINSTANCE);
	// запам'ятовує розміри екрана
	static void InitScreenSize(int width, int height);

	TVisibleObject(): m_x(0),m_y(0), m_width(0),m_height(0), m_step(0), m_view(0), m_mask(0), m_hMemoryDC(0) {}
	TVisibleObject(int x, int y, int w, int h, int s, HBITMAP b, HBITMAP m, HDC dc)
		: m_x(x), m_y(y), m_width(w), m_height(h), m_step(s), m_view(b), m_mask(m), m_hMemoryDC(dc) {}
	virtual ~TVisibleObject() {}

	// усі рухомі об'єкти вміють себе зображати на контексті пристрою
	void Show(CDC&) const;
};

//----------------------

class TMovedObject :public TVisibleObject
{
public:
	TMovedObject() :TVisibleObject() {}
	TMovedObject(int x, int y, int w, int h, int s, HBITMAP b, HBITMAP m, HDC dc)
		: TVisibleObject(x, y, w, h, s, b, m, dc) {}
	virtual void Move() = 0;
	virtual bool IsOut() const = 0;
};

class TBullet :public TMovedObject
{
public:
	TBullet() :TMovedObject() {}
// частину параметрів конструктора можна одразу конкретизувати:
// відомі розміри та бітмапки зображення/маски
	TBullet(int x, int y, HDC dc)	
		:TMovedObject(x, y, 2, 4, 6, BulletView, BulletMask, dc) {}
	virtual void Move() { m_y -= m_step; }
	virtual bool IsOut() const { return m_y < 5; }
	int GetX() const { return m_x; }
	int GetY() const { return m_y; }
};

// використано паттерн проектування "Стан"
class TAircraft :public TMovedObject
{
private:
	// абстрактний клас - основа ієрархії станів
	class Warcraft
	{
	protected:
		TAircraft& m_plane;
	public:
		Warcraft(TAircraft& p) :m_plane(p) {}
		virtual void Move() abstract;
		virtual bool IsOut() const abstract;
		virtual bool HitBy(const TBullet& b) const abstract;
	};
	class AliveCraft :public Warcraft
	{
	public:
		AliveCraft(TAircraft& p) :Warcraft(p) {}
		virtual void Move()
		{
			m_plane.m_x += m_plane.m_step;
		}
		virtual bool IsOut() const
		{
			return m_plane.m_x > ScreenWidth - m_plane.m_width;
		}
		virtual bool HitBy(const TBullet& b) const
		{
			return b.GetX() > m_plane.m_x &&
				b.GetX() + 1 < m_plane.m_x + m_plane.m_width &&
				b.GetY() > m_plane.m_y &&
				b.GetY() < m_plane.m_y + m_plane.m_height;
		}
	};
	class SlowCraft :public AliveCraft
	{
	public:
		SlowCraft(TAircraft& p) :AliveCraft(p)
		{
			m_plane.m_width = 30; m_plane.m_height = 16;
			m_plane.m_view = SlowView; m_plane.m_mask = SlowMask;
		}
	};
	class UsualCraft : public AliveCraft
	{
	public:
		UsualCraft(TAircraft& p) :AliveCraft(p)
		{
			m_plane.m_width = 31; m_plane.m_height = 18;
			m_plane.m_view = MidView; m_plane.m_mask = MidMask;
		}
	};
	class FastCraft :public AliveCraft
	{
	public:
		FastCraft(TAircraft& p) : AliveCraft(p)
		{
			m_plane.m_width = 29; m_plane.m_height = 14;
			m_plane.m_view = FastView; m_plane.m_mask = FastMask;
		}
	};
	class KilledCraft :public Warcraft
	{
	private:
		int m_shift;
	public:
		KilledCraft(TAircraft& p) :Warcraft(p), m_shift(1)
		{
			m_plane.m_view = CrashView;
			m_plane.m_mask = CrashMask;
		}
		virtual void Move()
		{
			m_plane.m_x += m_plane.m_step;
			m_plane.m_y += m_shift;
			m_shift *= 2;
		}
		virtual bool IsOut() const
		{
			return m_plane.m_x > ScreenWidth - m_plane.m_width || m_shift > 64;
		}
		virtual bool HitBy(const TBullet& b) const
		{
			return false;
		}
	};
	Warcraft* m_craft;
public:
	friend class AliveCraft;
	friend class KilledCraft;
	TAircraft() : TMovedObject(), m_craft(nullptr) {}
	TAircraft(int y, int s, HDC dc);
	~TAircraft() { delete m_craft; }
	virtual void Move();
	virtual bool IsOut() const;
	// метод зміни стану літака
	void Crash();
	// перевірка на влучання кулі
	bool HitBy(const TBullet&) const;
};

//----------------------

class TGun :public TVisibleObject
{
public:
	TGun() :TVisibleObject() {}
	// майже всі параметри можна одразу конкретизувати
	TGun(HDC dc) :TVisibleObject(ScreenWidth / 2 - 10, ScreenHeight - 40, 20, 30, 5, GunView, GunMask, dc) {}
	// постріл створює новий об'єкт - кулю
	TBullet* Fire() const;
	// переміщення обмежені розмірами вікна
	void MoveLeft() { m_x = (m_x >= m_step + 5) ? m_x - m_step : 5; }
	void MoveRight()
	{
		int limit = ScreenWidth - m_width - 5;
		m_x = (m_x <= limit - m_step) ? m_x + m_step : limit;
	}
};