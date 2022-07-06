#include "stdafx.h"
#include "Airfield.h"
#include <ctime>

TAirfield::TAirfield(list<TAircraft*>& list, HDC dc) : m_list(list), m_dc(dc)
{
	srand(time(0));
	m_interval = 500 + rand() % 500; 
}

// ����� ���� �� ��������� ����� � ���������� ��������
TAircraft* TAirfield::SendAircraft()
{
	return new TAircraft(5 + rand() % 200, 1 + rand() % 8, m_dc);
}

// ���� ��� ���������� ����������, �� ������ ����� �������� ������������� ����
void TAirfield::Spent(int t)
{
	m_interval -= t;
	if (m_interval <= 0)
	{
		m_list.push_back(this->SendAircraft());
		m_interval = 100 + rand() % 1900;
	}
}