#include <SFML/Graphics.hpp>
#include "MineSweeper.h"
#include <iostream>

// Проверка поля и отрисовка
void drawAll(MineSweeper& ms, sf::RenderWindow& window, sf::Texture& texture, sf::Sprite& sprite, const bool bombSpotted, const bool left_prev_pressed)
{
	// Полная проверка и отрисовка поля
	bool unopenedCell = false;
	int openedCells = 0; // количество открытых клеток
	int flagsPlaced = 0;
	for (int w = 0; w < ms.getCells(); w++)
		for (int h = 0; h < ms.getCells(); h++)
		{
			if (bombSpotted && ms.getLogic()[w][h] == MineSweeper::BOMB && ms[w][h] != MineSweeper::EXPLODED)
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
	if (bombSpotted)
		std::cout << "LOSE!\n";
	else if (flagsPlaced == ms.getMines() && openedCells + flagsPlaced == ms.getCells() * ms.getCells() && left_prev_pressed == false)
		std::cout << "WIN!\n";

	window.display(); // обновление окна
}

struct SurroundCells // Данные об окружающих клетках // Это класс уже
{
	std::vector<std::pair<int, int>> coords; // координаты неопознанных окружающих клеток
	SurroundCells() { coords.reserve(9); } // задаем потенциальный размер
	bool resSur = true; // восстнавить окружающие клетки после переезда мыши на другую клетку, или просто очистить записи о них?

	int flagCounter = 0; // количество флагов в окружающих клетках

	bool sameCell = false; // Если мы находимся на той же клетке - не нужно обновлять записи от перемещения по ней
};

struct PreviousCell	// Данные о прошлой клетке
{
	std::pair<int, int> сoords{ -1, -1 }; // координаты прошлой клетки - для её восстановления при перемещении мыши с зажатой для ЛКМ
	int value; // и её значение - для ЛКМ
};

// Восстанавливает окружающие клетки и очищает записи о них чтобы при сдвиге с клетки не открывались клетки, подсвеченные в прошлом
void restoreSur(MineSweeper& ms, SurroundCells& sc)
{
	if (sc.resSur) // Если открыли клетки - их не нужно восстанавливать
		for (int i = 0; i < sc.coords.size(); ++i)
			ms[sc.coords[i].first][sc.coords[i].second] = MineSweeper::UNOPENED; // восстанавливаем подсвеченные клетки

	if (!sc.coords.empty()) // массив заполняется только при двойном зажатии на цифре
		sc.coords.clear(); // очищаем координаты подсвеченных клеток
	sc.resSur = true;
	sc.flagCounter = 0;
	sc.sameCell = false;
}

void play()
{
	MineSweeper ms; // можно задать количество мин и клеток
	sf::RenderWindow window(sf::VideoMode(ms.getCells() * ms.getWidth(), ms.getCells() * ms.getWidth()), "MineSweeper", sf::Style::Close);
	sf::Texture texture;
	texture.loadFromFile("MineSweeper.jpg");
	sf::Sprite sprite(texture);

	// Двойное нажатие:
	SurroundCells sc; // Данные об окружающих клетках
	bool doublePress = false; // для двойного отжатия

	// Перемещение зажатой мыши по полю:
	PreviousCell pc; // Данные о прошлой клетке
	bool left_prev_pressed = false; // зажата ли ЛКМ еще с прошлой клетки? - чтобы не/восстанавливалась прошлая клетка

	bool firstClick = true; // это первое открытие клетки - для расстановки бомб. Первое открытие всегда безопасно
	bool bombSpotted = false; // бомба еще не открыта

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
				if (firstClick)
				{
					ms.placeBombs(mX, mY);
					ms.putNumbers();
					firstClick = false;
				}
				ms[mX][mY] = ms.getLogic()[mX][mY]; // присваиваем отображению внутреннее значение
				if (ms[mX][mY] == MineSweeper::BOMB) // если от нажатия мыши была открыта мина
				{
					ms[mX][mY] = MineSweeper::EXPLODED;
					bombSpotted = true;
				}
				else if (ms[mX][mY] == MineSweeper::EMPTY) // если нажатая клетка пустая
					ms.openUp(mX, mY); // автоматически открываем пустые клетки

				left_prev_pressed = false; // помечаем что ЛКМ отпущена
			}

			if (doublePress == true && bombSpotted == false) // Отпуск кнопки после даблклика
			{
				if (sc.flagCounter == ms[mX][mY]) // Если количество флажков рядом равно цифре на которой находится мышь
				{
					for (int i = 0; i < sc.coords.size(); ++i) // открываем окружающие клетки
					{
						ms[sc.coords[i].first][sc.coords[i].second] = ms.getLogic()[sc.coords[i].first][sc.coords[i].second];
						if (ms[sc.coords[i].first][sc.coords[i].second] == MineSweeper::BOMB) // открыли мину?
						{
							ms[sc.coords[i].first][sc.coords[i].second] = MineSweeper::EXPLODED;
							bombSpotted = true;
						}
						if (ms[sc.coords[i].first][sc.coords[i].second] == MineSweeper::EMPTY) // если открыли пустую клетку
							ms.openUp(sc.coords[i].first, sc.coords[i].second); // запускаем поиск других пустых клеток
					}
					sc.resSur = false; // Мы открыли клетки - не нужно их восстанавливать
				}
				restoreSur(ms, sc); // восстановление окружающих клеток и очистка их данных
				doublePress = false; // помечаем что даблклика нет
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
			if (pc.сoords.first != mX || pc.сoords.second != mY)
			{
				// Если двойное зажатие - восстанавливаем окружающие клетки
				if (sf::Mouse::isButtonPressed(sf::Mouse::Right))
					restoreSur(ms, sc);

				// Восстанавливаем прошлую клетку
				if (left_prev_pressed == true) // с зажатой ЛКМ в прошлой клетке
				{
					if (pc.value == MineSweeper::UNOPENED || pc.value == MineSweeper::FLAG) // и её нужно восстановить
						ms[pc.сoords.first][pc.сoords.second] = pc.value;
				}
				else // иначе помечаем что в этой клетке мы её зажали
					left_prev_pressed = true;

				// Запоминаем новую (текущую) клетку
				pc.value = ms[mX][mY]; // что в ней лежит
				pc.сoords = { mX, mY }; // и её координаты
			}

			// Подсветка клетки от зажатия ЛКМ
			if (ms[mX][mY] == MineSweeper::UNOPENED || ms[mX][mY] == MineSweeper::FLAG)
				ms[mX][mY] = MineSweeper::EMPTY;

			// Подсветка окружающих клеток если обе кнопки мыши зажаты на цифре
			if (sf::Mouse::isButtonPressed(sf::Mouse::Right)
				&& ms[mX][mY] >= MineSweeper::ONE && ms[mX][mY] <= MineSweeper::EIGHT
				&& !sc.sameCell) // не перевызывать эту ф от перемещения мыши внутри одной клетки
			{
				sc.sameCell = true;
				doublePress = true; // помечаем что нажаты обе кнопки
				for (int x = mX - 1; x <= mX + 1; ++x)	// из 8 клеток вокруг (+1 своя)
					for (int y = mY - 1; y <= mY + 1; ++y)
						if (x >= 0 && x < ms.getCells() && y >= 0 && y < ms.getCells())
						{
							if (ms[x][y] == MineSweeper::UNOPENED)
							{
								ms[x][y] = MineSweeper::EMPTY; // подсвечиваем неопознанные как открытые
								sc.coords.push_back({ x, y }); // записываем координаты этих клеток
							}
							else if (ms[x][y] == MineSweeper::FLAG) // и количество флагов вокруг
								++sc.flagCounter;
						}
			}
		}
		else if (event.type == sf::Event::Closed)
			window.close();

		// Проверка поля и отрисовка
		drawAll(ms, window, texture, sprite, bombSpotted, left_prev_pressed);
	}
}

int main() // TO DO: restart, timer, локальная запись рекордов, уровни сложности, ширина поля отдельно от высоты
{
	play();

	return 0;
}