#include <SFML/Graphics.hpp>
#include "MineSweeper.h"
#include <iostream>
#include "FieldState.h"

// �������� ���� � ���������
void drawAll(MineSweeper& ms, sf::RenderWindow& window, sf::Sprite& sprite, FieldState& fs)
{
	// ������ �������� � ��������� ����
	bool unopenedCell = false;
	int openedCells = 0; // ���������� �������� ������
	int flagsPlaced = 0;
	for (int w = 0; w < ms.getCells(); w++)
		for (int h = 0; h < ms.getCells(); h++)
		{
			if (fs.bombSpotted && ms.getLogic()[w][h] == MineSweeper::BOMB && ms[w][h] != MineSweeper::EXPLODED)
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
	if (fs.bombSpotted)
		std::cout << "LOSE!\n";
	else if (flagsPlaced == ms.getMines() && openedCells + flagsPlaced == ms.getCells() * ms.getCells() && fs.left_prev_pressed == false)
		std::cout << "WIN!\n";

	window.display(); // ���������� ����
}

void play()
{
	MineSweeper ms; // ����� ������ ���������� ��� � ������
	sf::RenderWindow window(sf::VideoMode(ms.getCells() * ms.getWidth(), ms.getCells() * ms.getWidth()), "MineSweeper", sf::Style::Close);
	sf::Texture texture;
	texture.loadFromFile("MineSweeper.jpg");
	sf::Sprite sprite(texture);

	FieldState fs; // ������ � ������� � ���������� �������, ��������� ����, ������� ������� � �������� �����

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
				if (fs.firstClick)
				{
					ms.placeBombs(mX, mY);
					ms.putNumbers();
					fs.firstClick = false;
				}
				ms[mX][mY] = ms.getLogic()[mX][mY]; // ����������� ����������� ���������� ��������
				if (ms[mX][mY] == MineSweeper::BOMB) // ���� �� ������� ���� ���� ������� ����
				{
					ms[mX][mY] = MineSweeper::EXPLODED;
					fs.bombSpotted = true;
				}
				else if (ms[mX][mY] == MineSweeper::EMPTY) // ���� ������� ������ ������
					ms.openUp(mX, mY); // ������������� ��������� ������ ������

				fs.left_prev_pressed = false; // �������� ��� ��� ��������
			}

			if (fs.doublePress == true && fs.bombSpotted == false) // ������ ������ ����� ���������
			{
				if (fs.sc.m_flagCounter == ms[mX][mY]) // ���� ���������� ������� ����� ����� ����� �� ������� ��������� ����
				{
					for (int i = 0; i < fs.sc.m_coords.size(); ++i) // ��������� ���������� ������
					{
						int x = fs.sc.m_coords[i].first;
						int y = fs.sc.m_coords[i].second;
						ms[x][y] = ms.getLogic()[x][y];
						if (ms[x][y] == MineSweeper::BOMB) // ������� ����?
						{
							ms[x][y] = MineSweeper::EXPLODED;
							fs.bombSpotted = true;
						}
						if (ms[x][y] == MineSweeper::EMPTY) // ���� ������� ������ ������
							ms.openUp(x, y); // ��������� ����� ������ ������ ������
					}
					fs.sc.m_resSur = false; // �� ������� ������ - �� ����� �� ���������������
				}
				fs.sc.restoreSur(ms); // �������������� ���������� ������ � ������� �� ������
				fs.doublePress = false; // �������� ��� ��������� ���
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
			if (fs.pc.�oords.first != mX || fs.pc.�oords.second != mY)
			{
				// ���� ������� ������� - ��������������� ���������� ������
				if (sf::Mouse::isButtonPressed(sf::Mouse::Right))
					fs.sc.restoreSur(ms);

				// ��������������� ������� ������
				if (fs.left_prev_pressed == true) // � ������� ��� � ������� ������
				{
					if (fs.pc.value == MineSweeper::UNOPENED || fs.pc.value == MineSweeper::FLAG) // � � ����� ������������
						ms[fs.pc.�oords.first][fs.pc.�oords.second] = fs.pc.value;
				}
				else // ����� �������� ��� � ���� ������ �� � ������
					fs.left_prev_pressed = true;

				// ���������� ����� (�������) ������
				fs.pc.value = ms[mX][mY]; // ��� � ��� �����
				fs.pc.�oords = { mX, mY }; // � � ����������
			}

			// ��������� ������ �� ������� ���
			if (ms[mX][mY] == MineSweeper::UNOPENED || ms[mX][mY] == MineSweeper::FLAG)
				ms[mX][mY] = MineSweeper::EMPTY;

			// ��������� ���������� ������ ���� ��� ������ ���� ������ �� �����
			if (sf::Mouse::isButtonPressed(sf::Mouse::Right)
				&& ms[mX][mY] >= MineSweeper::ONE && ms[mX][mY] <= MineSweeper::EIGHT
				&& !fs.sc.m_sameCell) // �� ������������ ��� � �� ����������� ���� ������ ����� ������
			{
				fs.sc.m_sameCell = true;
				fs.doublePress = true; // �������� ��� ������ ��� ������
				for (int x = mX - 1; x <= mX + 1; ++x)	// �� 8 ������ ������ (+1 ����)
					for (int y = mY - 1; y <= mY + 1; ++y)
						if (x >= 0 && x < ms.getCells() && y >= 0 && y < ms.getCells())
						{
							if (ms[x][y] == MineSweeper::UNOPENED)
							{
								ms[x][y] = MineSweeper::EMPTY; // ������������ ������������ ��� ��������
								fs.sc.m_coords.push_back({ x, y }); // ���������� ���������� ���� ������
							}
							else if (ms[x][y] == MineSweeper::FLAG) // � ���������� ������ ������
								++fs.sc.m_flagCounter;
						}
			}
		}
		else if (event.type == sf::Event::Closed)
			window.close();

		// �������� ���� � ���������
		drawAll(ms, window, sprite, fs);
	}
}

int main() // TO DO: ��������� ���������� ����, restart, timer, ��������� ������ ��������, ������ ���������, ������ ���� �������� �� ������
{
	play();

	return 0;
}