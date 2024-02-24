#pragma once
#include "Field.h"

class MineSweeper : public Field
{
	const int m_minesCount; // ���������� ���

	void setField(); // ��������� ������������ ���� ������������� ��������, � ���������� - �������

	void openXp(const int mouseX, const int mouseY); // �������� ������ ������ �� ������������
	void openXm(const int mouseX, const int mouseY);
	void openYp(const int mouseX, const int mouseY);
	void openYm(const int mouseX, const int mouseY);

public:
	enum ICONS
	{
		EMPTY,
		ONE, TWO, THREE, FOUR, FIVE, SIX, SEVEN, EIGHT,
		BOMB,
		UNOPENED,
		FLAG,
		EXPLODED
	};

	MineSweeper(int mines = 10, int cells = 9); // �����������
	void placeBombs(const int mouseX, const int mouseY); // ����������� ���� ����� ������� �������� ������ - ����� ������ ���� �� ��� �� �����
	void putNumbers(); // ����������� �������� ���������� ��� ���������� ����� ������� �������� ������
	void openUp(const int mouseX, const int mouseY); // ����������� �� �������� ������ ������. ��������� ������ ������ ������

	const int getMines() const { return m_minesCount; }
};