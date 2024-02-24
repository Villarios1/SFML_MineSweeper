#pragma once
#include "Field.h"

class MineSweeper : public Field
{
	const int m_minesCount; // количество мин

	void setField(); // Заполняем отображаемое поле неопознанными клетками, а внутреннее - пустыми

	void openXp(const int mouseX, const int mouseY); // открытие пустых клеток по направлениям
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

	MineSweeper(int mines = 10, int cells = 9); // конструктор
	void placeBombs(const int mouseX, const int mouseY); // Расставляем мины после первого открытия клетки - чтобы первый клик не был по бомбе
	void putNumbers(); // Расставляем значения количества мин поблизости после первого открытия клетки
	void openUp(const int mouseX, const int mouseY); // запускается от открытия пустой клетки. Открывает другие пустые клетки

	const int getMines() const { return m_minesCount; }
};