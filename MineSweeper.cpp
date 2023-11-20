#include "MineSweeper.h"

MineSweeper::MineSweeper(int mines, int cells) : m_minesCount(mines), Field(cells) // �����������
{
	setField();
};

void MineSweeper::setField() // ��������� ������������ ���� ������������� ��������, � ���������� - �������
{
	m_gridLogic.resize(m_cells);
	m_gridView.resize(m_cells);
	for (int w = 0; w < m_cells; w++)
	{
		m_gridLogic[w].resize(m_cells);
		m_gridView[w].resize(m_cells);
		for (int h = 0; h < m_cells; h++)
		{
			m_gridView[w][h] = UNOPENED;
			m_gridLogic[w][h] = EMPTY;
		}
	}
}

void MineSweeper::placeBombs(const int mouseX, const int mouseY) // ����������� ���� ����� ������� �������� ������
{
	int getRandomNumber(int min, int max); // getRandom.cpp
	int minesLeft = m_minesCount;
	while (minesLeft > 0)
	{
		int randX = getRandomNumber(0, m_cells - 1); // �������� ������, � ������� ����� ����������� ����
		int randY = getRandomNumber(0, m_cells - 1);
		// � ���� ��� �� �� ������, ������� �� �������. � � ��� ��� ��� ����
		if ((randX != mouseX || randY != mouseY) && m_gridLogic[randX][randY] != BOMB)
		{
			m_gridLogic[randX][randY] = BOMB; // �� ������ ���� ����
			--minesLeft;
		}
	}
}

void MineSweeper::putNumbers() // ����������� �������� ���������� ��� ���������� ����� ������� �������� ������
{
	for (int w = 0; w < m_cells; w++)
		for (int h = 0; h < m_cells; h++) // ��������� ������ ������
		{
			if (m_gridLogic[w][h] == BOMB)
				continue;						 // � ���� � ������ ��� �������� ����
			if ((w + 1) < m_cells && m_gridLogic[w + 1][h] == BOMB) ++m_gridLogic[w][h]; // �� ��������� ������ ������ ������
			if ((h + 1) < m_cells && m_gridLogic[w][h + 1] == BOMB) ++m_gridLogic[w][h]; // ���� � ����� ����� ���� - ����������� �������� ���� (1-8 - ���������� ���)
			if ((w - 1) >= 0 && m_gridLogic[w - 1][h] == BOMB) ++m_gridLogic[w][h];
			if ((h - 1) >= 0 && m_gridLogic[w][h - 1] == BOMB) ++m_gridLogic[w][h];
			if ((w - 1) >= 0 && (h + 1) < m_cells && m_gridLogic[w - 1][h + 1] == BOMB) ++m_gridLogic[w][h];
			if ((w + 1) < m_cells && (h - 1) >= 0 && m_gridLogic[w + 1][h - 1] == BOMB) ++m_gridLogic[w][h];
			if ((w + 1) < m_cells && (h + 1) < m_cells && m_gridLogic[w + 1][h + 1] == BOMB) ++m_gridLogic[w][h];
			if ((w - 1) >= 0 && (h - 1) >= 0 && m_gridLogic[w - 1][h - 1] == BOMB) ++m_gridLogic[w][h];
		}
}

void MineSweeper::openUp(const int mouseX, const int mouseY) // ����������� �� �������� ������ ������. ��������� ������ ������ ������
{
	openXp(mouseX, mouseY);
	openXm(mouseX, mouseY);
	openYp(mouseX, mouseY);
	openYm(mouseX, mouseY);
}

void MineSweeper::openXp(const int mouseX, const int mouseY) // �������� ������ ������ �� ������������
{
	for (int x = mouseX; x + 1 < m_cells; ++x) // ���� �� ������� � ����
	{
		m_gridView[x + 1][mouseY] = m_gridLogic[x + 1][mouseY]; // ��������� ��������� ������ � �����������
		if (m_gridLogic[x + 1][mouseY] != EMPTY) // ���� ��� ����������� ����������� - �������� � ��������:
		{
			if ((mouseY + 1) < m_cells)
			{
				m_gridView[x + 1][mouseY + 1] = m_gridLogic[x + 1][mouseY + 1]; // ��������� ������
				if (m_gridLogic[x + 1][mouseY + 1] == EMPTY) // � ���� ������ - ������ ������
				{
					openYp(x + 1, mouseY + 1); // ��������� ������ ����� �����������
					openXp(x + 1, mouseY + 1);
				}
			}
			if ((mouseY - 1) >= 0)
			{
				m_gridView[x + 1][mouseY - 1] = m_gridLogic[x + 1][mouseY - 1];
				if (m_gridLogic[x + 1][mouseY - 1] == 0)
				{
					openYm(x + 1, mouseY - 1);
					openXp(x + 1, mouseY - 1);
				}
			}
			break; // � ���������� ���� �� �������� �����������
		}
		else // ���� ��� ��������� ������ �� ����������� ���� ������
		{
			if ((mouseY + 1) < m_cells && (m_gridView[x + 1][mouseY + 1] == UNOPENED || m_gridView[x + 1][mouseY + 1] == FLAG))
				openYp(x + 1, mouseY);		// ��������� �� �� ��������� ������� �����������
			if ((mouseY - 1) >= 0 && (m_gridView[x + 1][mouseY - 1] == UNOPENED || m_gridView[x + 1][mouseY - 1] == FLAG))
				openYm(x + 1, mouseY);
		}
	} // � ��������� � ��������� ������ � ������� �����������
}

void MineSweeper::openXm(const int mouseX, const int mouseY)
{
	for (int x = mouseX; x - 1 >= 0; --x)
	{
		m_gridView[x - 1][mouseY] = m_gridLogic[x - 1][mouseY];
		if (m_gridLogic[x - 1][mouseY] != EMPTY)
		{
			if ((mouseY + 1) < m_cells)
			{
				m_gridView[x - 1][mouseY + 1] = m_gridLogic[x - 1][mouseY + 1];
				if (m_gridLogic[x - 1][mouseY + 1] == EMPTY)
				{
					openYp(x - 1, mouseY + 1);
					openXm(x - 1, mouseY + 1);
				}
			}
			if ((mouseY - 1) >= 0)
			{
				m_gridView[x - 1][mouseY - 1] = m_gridLogic[x - 1][mouseY - 1];
				if (m_gridLogic[x - 1][mouseY - 1] == 0)
				{
					openYm(x - 1, mouseY - 1);
					openXm(x - 1, mouseY - 1);
				}
			}
			break;
		}
		else
		{
			if ((mouseY + 1) < m_cells && (m_gridView[x - 1][mouseY + 1] == UNOPENED || m_gridView[x - 1][mouseY + 1] == FLAG))
				openYp(x - 1, mouseY);
			if ((mouseY - 1) >= 0 && (m_gridView[x - 1][mouseY - 1] == UNOPENED || m_gridView[x - 1][mouseY - 1] == FLAG))
				openYm(x - 1, mouseY);
		}
	}
}

void MineSweeper::openYp(const int mouseX, const int mouseY)
{
	for (int y = mouseY; y + 1 < m_cells; ++y)
	{
		m_gridView[mouseX][y + 1] = m_gridLogic[mouseX][y + 1];
		if (m_gridLogic[mouseX][y + 1] != EMPTY)
		{
			if ((mouseX + 1) < m_cells)
			{
				m_gridView[mouseX + 1][y + 1] = m_gridLogic[mouseX + 1][y + 1];
				if (m_gridLogic[mouseX + 1][y + 1] == EMPTY)
				{
					openXp(mouseX + 1, y + 1);
					openYp(mouseX + 1, y + 1);
				}
			}
			if ((mouseX - 1) >= 0)
			{
				m_gridView[mouseX - 1][y + 1] = m_gridLogic[mouseX - 1][y + 1];
				if (m_gridLogic[mouseX - 1][y + 1] == 0)
				{
					openXm(mouseX - 1, y + 1);
					openYp(mouseX - 1, y + 1);
				}
			}
			break;
		}
		else
		{
			if ((mouseX + 1) < m_cells && (m_gridView[mouseX + 1][y + 1] == UNOPENED || m_gridView[mouseX + 1][y + 1] == FLAG))
				openXp(mouseX, y + 1);
			if ((mouseX - 1) >= 0 && (m_gridView[mouseX - 1][y + 1] == UNOPENED || m_gridView[mouseX - 1][y + 1] == FLAG))
				openXm(mouseX, y + 1);
		}
	}
}

void MineSweeper::openYm(const int mouseX, const int mouseY)
{
	for (int y = mouseY; y - 1 >= 0; --y)
	{
		m_gridView[mouseX][y - 1] = m_gridLogic[mouseX][y - 1];
		if (m_gridLogic[mouseX][y - 1] != EMPTY)
		{
			if ((mouseX + 1) < m_cells)
			{
				m_gridView[mouseX + 1][y - 1] = m_gridLogic[mouseX + 1][y - 1];
				if (m_gridLogic[mouseX + 1][y - 1] == EMPTY)
				{
					openXp(mouseX + 1, y - 1);
					openYm(mouseX + 1, y - 1);
				}
			}
			if ((mouseX - 1) >= 0)
			{
				m_gridView[mouseX - 1][y - 1] = m_gridLogic[mouseX - 1][y - 1];
				if (m_gridLogic[mouseX - 1][y - 1] == 0)
				{
					openXm(mouseX - 1, y - 1);
					openYm(mouseX - 1, y - 1);
				}
			}
			break;
		}
		else
		{
			if ((mouseX + 1) < m_cells && (m_gridView[mouseX + 1][y - 1] == UNOPENED || m_gridView[mouseX + 1][y - 1] == FLAG))
				openXp(mouseX, y - 1);
			if ((mouseX - 1) >= 0 && (m_gridView[mouseX - 1][y - 1] == UNOPENED || m_gridView[mouseX - 1][y - 1] == FLAG))
				openXm(mouseX, y - 1);
		}
	}
}