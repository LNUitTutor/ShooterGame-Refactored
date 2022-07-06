/*    
			   */

#include <list>
#include <algorithm>
#include <functional>
#include "MovedObjects.h"

#pragma once

using std::list;

// зображення графічних об'єктів
class ShowObj
{
	CDC& m_dc;
public:
	ShowObj(CDC& dc) :m_dc(dc) {}
	void operator()(TMovedObject* a) { a->Show(m_dc); }
};
template<class T>
void ShowAll(T& L, CDC& dc)
{
	std::for_each(L.begin(), L.end(), ShowObj(dc));
	//for (T::iterator it = L.begin(); it != L.end(); ++it) (*it)->Show(dc);
}

// переміщення і вилучення об'єктів
class EraseObj
{
public:
	bool operator()(TMovedObject* a)
	{
		bool res = a->IsOut();
		if (res) delete a;
		return res;
	}
};
template<class T>
void MoveAndGarbage(T& L)
{
	std::for_each(L.begin(), L.end(), [](typename T::value_type x){ x->Move(); });

	/*typename T::iterator it = std::remove_if(L.begin(), L.end(), EraseObj());
	L.erase(it, L.end());*/

	L.remove_if(EraseObj());
}

// об'єкт-функція для перевірки влучання
class Hiter
{
	list<TAircraft*>& list_of_aircraft;
public:
	Hiter(list<TAircraft*>& a) :list_of_aircraft(a) {}
	bool operator()(TBullet* b)
	{
		/*list<TAircraft*>::iterator it = std::find_if(list_of_aircraft.begin(), list_of_aircraft.end(),
			[b](TAircraft* a) { return a->hitBy(*b); });
		if (it != list_of_aircraft.end())
		{
			(*it)->Crash();
			delete b;
			return true;
		}*/
		list<TAircraft*>::iterator it = list_of_aircraft.begin();
		while (it != list_of_aircraft.end())
		{
			if ((*it)->HitBy(*b))
			{
				(*it)->Crash();
				delete b;
				return true;
			}
			++it;
		}
		return false;
	}
};