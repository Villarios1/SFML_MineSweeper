#include "Field.h"

Field::Field(int cells, int width) : m_cells(cells), m_width(width) // �����������
{

}

std::vector<int>& Field::operator[](int index) // ������ � ������� ������������� ����
{
    return m_gridView[index];
};