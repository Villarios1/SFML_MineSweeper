#include "Field.h"

Field::Field(int cells, int width) : m_cells(cells), m_width(width) // конструктор
{

}

std::vector<int>& Field::operator[](int index) // доступ к клеткам отображаемого поля
{
    return m_gridView[index];
};