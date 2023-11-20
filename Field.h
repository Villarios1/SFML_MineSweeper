#pragma once
#include <vector>

class Field
{
protected:
	const int m_cells; // ���������� ������
	const int m_width; // ������ ������ � ��������

	std::vector<std::vector<int>> m_gridLogic; // ���������� ����
	std::vector<std::vector<int>> m_gridView; // ������������ ����

public:
	Field(int cells = 9, int width = 32); // �����������

	const int getCells() const { return m_cells; }
	const int getWidth() const { return m_width; }

	std::vector<int>& operator[](int index); // ������ � ������� ������������� ����
	const std::vector<std::vector<int>>& getLogic() const { return m_gridLogic; }
};