#include <SFML/Graphics.hpp>
#include "MineSweeper.h"
#include <iostream>

// �������� ���� � ���������
void drawAll(MineSweeper& ms, sf::RenderWindow& window, sf::Texture& texture, sf::Sprite& sprite, const bool bombSpotted, const bool left_prev_pressed)
{
	// ������ �������� � ��������� ����
	bool unopenedCell = false;
	int openedCells = 0; // ���������� �������� ������
	int flagsPlaced = 0;
	for (int w = 0; w < ms.getCells(); w++)
		for (int h = 0; h < ms.getCells(); h++)
		{
			if (bombSpotted && ms.getLogic()[w][h] == MineSweeper::BOMB && ms[w][h] != MineSweeper::EXPLODED)
				ms[w][h] = ms.getLogic()[w][h]; // ���� ���� ������� ���� - ��������� ��� ����
			else if (!unopenedCell) // ����� ���� �� ������ ���������� ������ ��� �� ������� ���� �� �����
			{
				if (ms[w][h] == MineSweeper::UNOPENED)
					unopenedCell = true;
				else
				{
					if (ms[w][h] == MineSweeper::FLAG)
						++flagsPlaced;		// ������������ ������������� �����
					else
						++openedCells;		// � �������� ������
				}
			}
			// ��������� ������ ������:
			sprite.setTextureRect(sf::IntRect(ms[w][h] * ms.getWidth(), 0, ms.getWidth(), ms.getWidth()));	// �������� ������� ��������, ��������������� �������� ����
			sprite.setPosition(static_cast<float>(w * ms.getWidth()), static_cast<float>(h * ms.getWidth())); // ������������� ������ �� ��������������� ������� ����
			window.draw(sprite);	// � ������������
		}

	// ���� ����
	if (bombSpotted)
		std::cout << "LOSE!\n";
	else if (flagsPlaced == ms.getMines() && openedCells + flagsPlaced == ms.getCells() * ms.getCells() && left_prev_pressed == false)
		std::cout << "WIN!\n";

	window.display(); // ���������� ����
}

struct SurroundCells // ������ �� ���������� ������� // ��� ����� ���
{
	std::vector<std::pair<int, int>> coords; // ���������� ������������ ���������� ������
	SurroundCells() { coords.reserve(9); } // ������ ������������� ������
	bool resSur = true; // ����������� ���������� ������ ����� �������� ���� �� ������ ������, ��� ������ �������� ������ � ���?

	int flagCounter = 0; // ���������� ������ � ���������� �������

	bool sameCell = false; // ���� �� ��������� �� ��� �� ������ - �� ����� ��������� ������ �� ����������� �� ���
};

struct PreviousCell	// ������ � ������� ������
{
	std::pair<int, int> �oords{ -1, -1 }; // ���������� ������� ������ - ��� � �������������� ��� ����������� ���� � ������� ��� ���
	int value; // � � �������� - ��� ���
};

// ��������������� ���������� ������ � ������� ������ � ��� ����� ��� ������ � ������ �� ����������� ������, ������������ � �������
void restoreSur(MineSweeper& ms, SurroundCells& sc)
{
	if (sc.resSur) // ���� ������� ������ - �� �� ����� ���������������
		for (int i = 0; i < sc.coords.size(); ++i)
			ms[sc.coords[i].first][sc.coords[i].second] = MineSweeper::UNOPENED; // ��������������� ������������ ������

	if (!sc.coords.empty()) // ������ ����������� ������ ��� ������� ������� �� �����
		sc.coords.clear(); // ������� ���������� ������������ ������
	sc.resSur = true;
	sc.flagCounter = 0;
	sc.sameCell = false;
}

void play()
{
	MineSweeper ms; // ����� ������ ���������� ��� � ������
	sf::RenderWindow window(sf::VideoMode(ms.getCells() * ms.getWidth(), ms.getCells() * ms.getWidth()), "MineSweeper", sf::Style::Close);
	sf::Texture texture;
	texture.loadFromFile("MineSweeper.jpg");
	sf::Sprite sprite(texture);

	// ������� �������:
	SurroundCells sc; // ������ �� ���������� �������
	bool doublePress = false; // ��� �������� �������

	// ����������� ������� ���� �� ����:
	PreviousCell pc; // ������ � ������� ������
	bool left_prev_pressed = false; // ������ �� ��� ��� � ������� ������? - ����� ��/����������������� ������� ������

	bool firstClick = true; // ��� ������ �������� ������ - ��� ����������� ����. ������ �������� ������ ���������
	bool bombSpotted = false; // ����� ��� �� �������

	sf::Event event;
	while (window.waitEvent(event)) // ����� - ������� ������� ��� �������� ����
	{
		int mX = sf::Mouse::getPosition(window).x / ms.getWidth(); // ������� ���������� ������, � ������� ��������� ����
		int mY = sf::Mouse::getPosition(window).y / ms.getWidth();

		if (!(mX >= 0 && mX < ms.getCells() && mY >= 0 && mY < ms.getCells()))
			continue; // �� ������ � ������� �� ��������� �������� ����

		// ��������� ������� � ����������� ����:
		if (event.type == sf::Event::MouseButtonReleased) // ������ ������
		{
			if (event.mouseButton.button == sf::Mouse::Left) // ������ ���
			{
				if (firstClick)
				{
					ms.placeBombs(mX, mY);
					ms.putNumbers();
					firstClick = false;
				}
				ms[mX][mY] = ms.getLogic()[mX][mY]; // ����������� ����������� ���������� ��������
				if (ms[mX][mY] == MineSweeper::BOMB) // ���� �� ������� ���� ���� ������� ����
				{
					ms[mX][mY] = MineSweeper::EXPLODED;
					bombSpotted = true;
				}
				else if (ms[mX][mY] == MineSweeper::EMPTY) // ���� ������� ������ ������
					ms.openUp(mX, mY); // ������������� ��������� ������ ������

				left_prev_pressed = false; // �������� ��� ��� ��������
			}

			if (doublePress == true && bombSpotted == false) // ������ ������ ����� ���������
			{
				if (sc.flagCounter == ms[mX][mY]) // ���� ���������� ������� ����� ����� ����� �� ������� ��������� ����
				{
					for (int i = 0; i < sc.coords.size(); ++i) // ��������� ���������� ������
					{
						ms[sc.coords[i].first][sc.coords[i].second] = ms.getLogic()[sc.coords[i].first][sc.coords[i].second];
						if (ms[sc.coords[i].first][sc.coords[i].second] == MineSweeper::BOMB) // ������� ����?
						{
							ms[sc.coords[i].first][sc.coords[i].second] = MineSweeper::EXPLODED;
							bombSpotted = true;
						}
						if (ms[sc.coords[i].first][sc.coords[i].second] == MineSweeper::EMPTY) // ���� ������� ������ ������
							ms.openUp(sc.coords[i].first, sc.coords[i].second); // ��������� ����� ������ ������ ������
					}
					sc.resSur = false; // �� ������� ������ - �� ����� �� ���������������
				}
				restoreSur(ms, sc); // �������������� ���������� ������ � ������� �� ������
				doublePress = false; // �������� ��� ��������� ���
			}
			else if (event.mouseButton.button == sf::Mouse::Right) // ������ ��� ��� ���������
			{
				switch (ms[mX][mY]) // ���/���� ������
				{
				case MineSweeper::UNOPENED: ms[mX][mY] = MineSweeper::FLAG; break;
				case MineSweeper::FLAG: ms[mX][mY] = MineSweeper::UNOPENED; break;
				}
			}
		}
		else if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) // ������� ������/-�� ����
		{
			// ���� ���� ���������� � ������:
			if (pc.�oords.first != mX || pc.�oords.second != mY)
			{
				// ���� ������� ������� - ��������������� ���������� ������
				if (sf::Mouse::isButtonPressed(sf::Mouse::Right))
					restoreSur(ms, sc);

				// ��������������� ������� ������
				if (left_prev_pressed == true) // � ������� ��� � ������� ������
				{
					if (pc.value == MineSweeper::UNOPENED || pc.value == MineSweeper::FLAG) // � � ����� ������������
						ms[pc.�oords.first][pc.�oords.second] = pc.value;
				}
				else // ����� �������� ��� � ���� ������ �� � ������
					left_prev_pressed = true;

				// ���������� ����� (�������) ������
				pc.value = ms[mX][mY]; // ��� � ��� �����
				pc.�oords = { mX, mY }; // � � ����������
			}

			// ��������� ������ �� ������� ���
			if (ms[mX][mY] == MineSweeper::UNOPENED || ms[mX][mY] == MineSweeper::FLAG)
				ms[mX][mY] = MineSweeper::EMPTY;

			// ��������� ���������� ������ ���� ��� ������ ���� ������ �� �����
			if (sf::Mouse::isButtonPressed(sf::Mouse::Right)
				&& ms[mX][mY] >= MineSweeper::ONE && ms[mX][mY] <= MineSweeper::EIGHT
				&& !sc.sameCell) // �� ������������ ��� � �� ����������� ���� ������ ����� ������
			{
				sc.sameCell = true;
				doublePress = true; // �������� ��� ������ ��� ������
				for (int x = mX - 1; x <= mX + 1; ++x)	// �� 8 ������ ������ (+1 ����)
					for (int y = mY - 1; y <= mY + 1; ++y)
						if (x >= 0 && x < ms.getCells() && y >= 0 && y < ms.getCells())
						{
							if (ms[x][y] == MineSweeper::UNOPENED)
							{
								ms[x][y] = MineSweeper::EMPTY; // ������������ ������������ ��� ��������
								sc.coords.push_back({ x, y }); // ���������� ���������� ���� ������
							}
							else if (ms[x][y] == MineSweeper::FLAG) // � ���������� ������ ������
								++sc.flagCounter;
						}
			}
		}
		else if (event.type == sf::Event::Closed)
			window.close();

		// �������� ���� � ���������
		drawAll(ms, window, texture, sprite, bombSpotted, left_prev_pressed);
	}
}

int main() // TO DO: restart, timer, ��������� ������ ��������, ������ ���������, ������ ���� �������� �� ������
{
	play();

	return 0;
}