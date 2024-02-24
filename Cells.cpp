#include "Cells.h"

void SurroundCells::restoreSur(MineSweeper& ms)
{
	if (m_resSur) // Если открыли клетки - их не нужно восстанавливать
		for (int i = 0; i < m_coords.size(); ++i)
			ms[m_coords[i].first][m_coords[i].second] = MineSweeper::UNOPENED; // восстанавливаем подсвеченные клетки

	if (!m_coords.empty()) // массив заполняется только при двойном зажатии на цифре
		m_coords.clear(); // очищаем координаты подсвеченных клеток
	m_resSur = true;
	m_flagCounter = 0;
	m_sameCell = false;
}