#pragma once
#include "Cells.h"

struct FieldState
{
	// ������� �������:
	SurroundCells sc; // ������ �� ���������� �������
	bool doublePress = false; // ��� �������� �������

	// ����������� ������� ���� �� ����:
	PreviousCell pc; // ������ � ������� ������
	bool left_prev_pressed = false; // ������ �� ��� ��� � ������� ������? - ����� ��/����������������� ������� ������

	bool firstClick = true; // ��� ������ �������� ������ - ��� ����������� ����. ������ �������� ������ ���������
	bool bombSpotted = false; // ����� ��� �� �������
};