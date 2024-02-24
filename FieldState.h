#pragma once
#include "Cells.h"

struct FieldState
{
	// Двойное нажатие:
	SurroundCells sc; // Данные об окружающих клетках
	bool doublePress = false; // для двойного отжатия

	// Перемещение зажатой мыши по полю:
	PreviousCell pc; // Данные о прошлой клетке
	bool left_prev_pressed = false; // зажата ли ЛКМ еще с прошлой клетки? - чтобы не/восстанавливалась прошлая клетка

	bool firstClick = true; // это первое открытие клетки - для расстановки бомб. Первое открытие всегда безопасно
	bool bombSpotted = false; // бомба еще не открыта
};