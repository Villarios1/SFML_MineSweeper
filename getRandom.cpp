#include <random>

int getRandomNumber(int min, int max)
{
	std::random_device rd;
	std::mt19937_64 gen(rd());
	std::uniform_int_distribution distr(min, max);
	return distr(gen);
}