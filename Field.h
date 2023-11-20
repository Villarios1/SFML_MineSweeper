#pragma once
#include <vector>

class Field
{
protected:
	const int m_cells; // количество клеток
	const int m_width; // ширина клетки в пикселях

	std::vector<std::vector<int>> m_gridLogic; // внутреннее поле
	std::vector<std::vector<int>> m_gridView; // отображаемое поле

public:
	Field(int cells = 9, int width = 32); // конструктор

	const int getCells() const { return m_cells; }
	const int getWidth() const { return m_width; }

	std::vector<int>& operator[](int index); // доступ к клеткам отображаемого поля
	const std::vector<std::vector<int>>& getLogic() const { return m_gridLogic; }
};