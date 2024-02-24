#include <SFML/Graphics.hpp>
#include "MineSweeper.h"
#include <iostream>
#include "FieldState.h"

// Проверка поля и отрисовка
void drawAll(MineSweeper& ms, sf::RenderWindow& window, sf::Sprite& sprite, FieldState& fs)
{
	// Полная проверка и отрисовка поля
	bool unopenedCell = false;
	int openedCells = 0; // количество открытых клеток
	int flagsPlaced = 0;
	for (int w = 0; w < ms.getCells(); w++)
		for (int h = 0; h < ms.getCells(); h++)
		{
			if (fs.bombSpotted && ms.getLogic()[w][h] == MineSweeper::BOMB && ms[w][h] != MineSweeper::EXPLODED)
				ms[w][h] = ms.getLogic()[w][h]; // если была открыта мина - открываем все мины
			else if (!unopenedCell) // иначе пока не найдем неоткрытую клетку или не пройдем поле до конца
			{
				if (ms[w][h] == MineSweeper::UNOPENED)
					unopenedCell = true;
				else
				{
					if (ms[w][h] == MineSweeper::FLAG)
						++flagsPlaced;		// подсчитываем проставленные флаги
					else
						++openedCells;		// и открытые клетки
				}
			}
			// Отрисовка каждой клетки:
			sprite.setTextureRect(sf::IntRect(ms[w][h] * ms.getWidth(), 0, ms.getWidth(), ms.getWidth()));	// выбираем участок текстуры, соответствующий значению поля
			sprite.setPosition(static_cast<float>(w * ms.getWidth()), static_cast<float>(h * ms.getWidth())); // устанавливаем спрайт на соответствующую позицию поля
			window.draw(sprite);	// и отрисовываем
		}

	// Итог игры
	if (fs.bombSpotted)
		std::cout << "LOSE!\n";
	else if (flagsPlaced == ms.getMines() && openedCells + flagsPlaced == ms.getCells() * ms.getCells() && fs.left_prev_pressed == false)
		std::cout << "WIN!\n";

	window.display(); // обновление окна
}

void play()
{
	MineSweeper ms; // можно задать количество мин и клеток
	sf::RenderWindow window(sf::VideoMode(ms.getCells() * ms.getWidth(), ms.getCells() * ms.getWidth()), "MineSweeper", sf::Style::Close);
	sf::Texture texture;
	texture.loadFromFile("MineSweeper.jpg");
	sf::Sprite sprite(texture);

	FieldState fs; // данные о прошлой и окружающих клетках, состоянии мыши, первого нажатия и открытой бомбы

	sf::Event event;
	while (window.waitEvent(event)) // ивент - нажатие клавиши или смещение мыши
	{
		int mX = sf::Mouse::getPosition(window).x / ms.getWidth(); // текущие координаты клетки, в которой находится мышь
		int mY = sf::Mouse::getPosition(window).y / ms.getWidth();

		if (!(mX >= 0 && mX < ms.getCells() && mY >= 0 && mY < ms.getCells()))
			continue; // от выхода и нажатий за пределами игрового окна

		// Обработка нажатий и перемещения мыши:
		if (event.type == sf::Event::MouseButtonReleased) // Отпуск кнопки
		{
			if (event.mouseButton.button == sf::Mouse::Left) // отпуск ЛКМ
			{
				if (fs.firstClick)
				{
					ms.placeBombs(mX, mY);
					ms.putNumbers();
					fs.firstClick = false;
				}
				ms[mX][mY] = ms.getLogic()[mX][mY]; // присваиваем отображению внутреннее значение
				if (ms[mX][mY] == MineSweeper::BOMB) // если от нажатия мыши была открыта мина
				{
					ms[mX][mY] = MineSweeper::EXPLODED;
					fs.bombSpotted = true;
				}
				else if (ms[mX][mY] == MineSweeper::EMPTY) // если нажатая клетка пустая
					ms.openUp(mX, mY); // автоматически открываем пустые клетки

				fs.left_prev_pressed = false; // помечаем что ЛКМ отпущена
			}

			if (fs.doublePress == true && fs.bombSpotted == false) // Отпуск кнопки после даблклика
			{
				if (fs.sc.m_flagCounter == ms[mX][mY]) // Если количество флажков рядом равно цифре на которой находится мышь
				{
					for (int i = 0; i < fs.sc.m_coords.size(); ++i) // открываем окружающие клетки
					{
						int x = fs.sc.m_coords[i].first;
						int y = fs.sc.m_coords[i].second;
						ms[x][y] = ms.getLogic()[x][y];
						if (ms[x][y] == MineSweeper::BOMB) // открыли мину?
						{
							ms[x][y] = MineSweeper::EXPLODED;
							fs.bombSpotted = true;
						}
						if (ms[x][y] == MineSweeper::EMPTY) // если открыли пустую клетку
							ms.openUp(x, y); // запускаем поиск других пустых клеток
					}
					fs.sc.m_resSur = false; // Мы открыли клетки - не нужно их восстанавливать
				}
				fs.sc.restoreSur(ms); // восстановление окружающих клеток и очистка их данных
				fs.doublePress = false; // помечаем что даблклика нет
			}
			else if (event.mouseButton.button == sf::Mouse::Right) // Отпуск ПКМ без даблклика
			{
				switch (ms[mX][mY]) // вкл/выкл флажок
				{
				case MineSweeper::UNOPENED: ms[mX][mY] = MineSweeper::FLAG; break;
				case MineSweeper::FLAG: ms[mX][mY] = MineSweeper::UNOPENED; break;
				}
			}
		}
		else if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) // Зажатие кнопки/-ок мыши
		{
			// Если мышь сместилась с клетки:
			if (fs.pc.сoords.first != mX || fs.pc.сoords.second != mY)
			{
				// Если двойное зажатие - восстанавливаем окружающие клетки
				if (sf::Mouse::isButtonPressed(sf::Mouse::Right))
					fs.sc.restoreSur(ms);

				// Восстанавливаем прошлую клетку
				if (fs.left_prev_pressed == true) // с зажатой ЛКМ в прошлой клетке
				{
					if (fs.pc.value == MineSweeper::UNOPENED || fs.pc.value == MineSweeper::FLAG) // и её нужно восстановить
						ms[fs.pc.сoords.first][fs.pc.сoords.second] = fs.pc.value;
				}
				else // иначе помечаем что в этой клетке мы её зажали
					fs.left_prev_pressed = true;

				// Запоминаем новую (текущую) клетку
				fs.pc.value = ms[mX][mY]; // что в ней лежит
				fs.pc.сoords = { mX, mY }; // и её координаты
			}

			// Подсветка клетки от зажатия ЛКМ
			if (ms[mX][mY] == MineSweeper::UNOPENED || ms[mX][mY] == MineSweeper::FLAG)
				ms[mX][mY] = MineSweeper::EMPTY;

			// Подсветка окружающих клеток если обе кнопки мыши зажаты на цифре
			if (sf::Mouse::isButtonPressed(sf::Mouse::Right)
				&& ms[mX][mY] >= MineSweeper::ONE && ms[mX][mY] <= MineSweeper::EIGHT
				&& !fs.sc.m_sameCell) // не перевызывать эту ф от перемещения мыши внутри одной клетки
			{
				fs.sc.m_sameCell = true;
				fs.doublePress = true; // помечаем что нажаты обе кнопки
				for (int x = mX - 1; x <= mX + 1; ++x)	// из 8 клеток вокруг (+1 своя)
					for (int y = mY - 1; y <= mY + 1; ++y)
						if (x >= 0 && x < ms.getCells() && y >= 0 && y < ms.getCells())
						{
							if (ms[x][y] == MineSweeper::UNOPENED)
							{
								ms[x][y] = MineSweeper::EMPTY; // подсвечиваем неопознанные как открытые
								fs.sc.m_coords.push_back({ x, y }); // записываем координаты этих клеток
							}
							else if (ms[x][y] == MineSweeper::FLAG) // и количество флагов вокруг
								++fs.sc.m_flagCounter;
						}
			}
		}
		else if (event.type == sf::Event::Closed)
			window.close();

		// Проверка поля и отрисовка
		drawAll(ms, window, sprite, fs);
	}
}

int main() // TO DO: отрисовка результата игры, restart, timer, локальная запись рекордов, уровни сложности, ширина поля отдельно от высоты
{
	play();

	return 0;
}