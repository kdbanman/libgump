#include <cstdlib>
#include <iostream>

#include "Environment.h"

int main()
{
	Environment environment = Environment(100);

	int seedStart = 36;
	int seedEnd = 60;

	for (int x = seedStart; x <= seedEnd; x++) {
		for (int y = seedStart; y <= seedEnd; y++) {
			for (int z = seedStart; z <= seedEnd; z++) {
				if (rand() % 2 == 0 && environment.isValidCoordinate(x, y, z)) {
					environment.turnOn(x, y, z);
				}
			}
		}
	}

	for (int generation = 0; generation < 100; generation++) {
		std::cout << environment.getLiveCellCount();
		std::cout << "\n";

		environment.iterate();
	}
}