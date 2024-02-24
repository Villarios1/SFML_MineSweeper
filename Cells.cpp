#include "Cells.h"

void SurroundCells::restoreSur(MineSweeper& ms)
{
	if (m_resSur) // ���� ������� ������ - �� �� ����� ���������������
		for (int i = 0; i < m_coords.size(); ++i)
			ms[m_coords[i].first][m_coords[i].second] = MineSweeper::UNOPENED; // ��������������� ������������ ������

	if (!m_coords.empty()) // ������ ����������� ������ ��� ������� ������� �� �����
		m_coords.clear(); // ������� ���������� ������������ ������
	m_resSur = true;
	m_flagCounter = 0;
	m_sameCell = false;
}