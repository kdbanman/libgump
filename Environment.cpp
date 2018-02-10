#include <assert.h>
#include <exception>
#include <sstream>

#include "Environment.h"



class InvalidCoordinateException : public std::exception
{
public:
	InvalidCoordinateException(int x, int y, int z) {
		InvalidCoordinateException::x = x;
		InvalidCoordinateException::y = y;
		InvalidCoordinateException::z = z;
	}

	const char * what() const throw ()
	{
		std::stringstream errorStream;
		errorStream << "Invalid coordinate encountered! (" << x << ", " << y << ", " << z << ")";

		return errorStream.str().c_str();
	}

private:
	int x;
	int y;
	int z;
};

class InvalidLiveCoordinateException : public std::exception
{
public:
	InvalidLiveCoordinateException(int x, int y, int z) {
		InvalidLiveCoordinateException::x = x;
		InvalidLiveCoordinateException::y = y;
		InvalidLiveCoordinateException::z = z;
	}

	const char * what() const throw ()
	{
		std::stringstream errorStream;
		errorStream << "Invalid live coordinate encountered! (" << x << ", " << y << ", " << z << ")";

		return errorStream.str().c_str();
	}

private:
	int x;
	int y;
	int z;
};



Environment::Environment(int size)
{
	Environment::size = size;
	liveCellCount = 0;

	validCoordinates = std::vector<Coordinate>();

	cells = new char**[size];
	nextCells = new char**[size];
	for (int x = 0; x < size; x++) {
		cells[x] = new char*[size];
		nextCells[x] = new char*[size];

		for (int y = 0; y < size; y++) {
			cells[x][y] = new char[size];
			nextCells[x][y] = new char[size];

			for (int z = 0; z < size; z++) {
				cells[x][y][z] = 0;
				nextCells[x][y][z] = 0;

				if (isValidCoordinate(x, y, z)) {
					validCoordinates.push_back(Coordinate(x, y, z));
				}
			}
		}
	}
}


Environment::~Environment()
{
}

void Environment::turnOn(int x, int y, int z)
{
	throwIfInvalid(x, y, z);

	if (cells[x][y][z] == 0) {
		liveCellCount++;
	}

	cells[x][y][z] = 1;
}

void Environment::turnOff(int x, int y, int z)
{
	throwIfInvalid(x, y, z);

	if (cells[x][y][z] == 1) {
		liveCellCount--;
	}

	cells[x][y][z] = 0;
}

void Environment::iterate()
{
	for (Coordinate const& coordinate : validCoordinates) {
		int x = coordinate.x;
		int y = coordinate.y;
		int z = coordinate.z;

		throwIfInvalid(x, y, z);

		int liveNeighbors = 0;
		if (x % 2 == 1) {
			liveNeighbors += getCellState(x, y + 2, z + 2);
			liveNeighbors += getCellState(x, y + 2, z - 2);
			liveNeighbors += getCellState(x, y - 2, z + 2);
			liveNeighbors += getCellState(x, y - 2, z - 2);

			liveNeighbors += getCellState(x + 1, y, z + 1);
			liveNeighbors += getCellState(x + 1, y, z - 1);
			liveNeighbors += getCellState(x + 1, y + 1, z);
			liveNeighbors += getCellState(x + 1, y - 1, z);

			liveNeighbors += getCellState(x - 1, y, z + 1);
			liveNeighbors += getCellState(x - 1, y, z - 1);
			liveNeighbors += getCellState(x - 1, y + 1, z);
			liveNeighbors += getCellState(x - 1, y - 1, z);
		}
		else if (y % 2 == 1) {
			liveNeighbors += getCellState(x + 2, y, z + 2);
			liveNeighbors += getCellState(x + 2, y, z - 2);
			liveNeighbors += getCellState(x - 2, y, z + 2);
			liveNeighbors += getCellState(x - 2, y, z - 2);

			liveNeighbors += getCellState(x, y + 1, z + 1);
			liveNeighbors += getCellState(x, y + 1, z - 1);
			liveNeighbors += getCellState(x + 1, y + 1, z);
			liveNeighbors += getCellState(x - 1, y + 1, z);

			liveNeighbors += getCellState(x, y - 1, z + 1);
			liveNeighbors += getCellState(x, y - 1, z - 1);
			liveNeighbors += getCellState(x + 1, y - 1, z);
			liveNeighbors += getCellState(x - 1, y - 1, z);
		}
		else if (z % 2 == 1) {
			liveNeighbors += getCellState(x + 2, y + 2, z);
			liveNeighbors += getCellState(x - 2, y + 2, z);
			liveNeighbors += getCellState(x + 2, y - 2, z);
			liveNeighbors += getCellState(x - 2, y - 2, z);

			liveNeighbors += getCellState(x + 1, y, z + 1);
			liveNeighbors += getCellState(x - 1, y, z + 1);
			liveNeighbors += getCellState(x, y + 1, z + 1);
			liveNeighbors += getCellState(x, y - 1, z + 1);

			liveNeighbors += getCellState(x + 1, y, z - 1);
			liveNeighbors += getCellState(x - 1, y, z - 1);
			liveNeighbors += getCellState(x, y + 1, z - 1);
			liveNeighbors += getCellState(x, y - 1, z - 1);
		}

		// Coming to life.
		if ((liveNeighbors == 4 || liveNeighbors == 5) && cells[x][y][z] == 0) {
			liveCellCount++;
			nextCells[x][y][z] = 1;
		}
		// Staying alive.
		else if ((liveNeighbors == 3 || liveNeighbors == 4 || liveNeighbors == 5) && cells[x][y][z] == 1) {
			nextCells[x][y][z] = 1;
		}
		else {
			// Becoming dead.
			if (cells[x][y][z] == 1) {
				liveCellCount--;
				nextCells[x][y][z] = 0;
			}
			// Staying dead.
			else {
				nextCells[x][y][z] = 0;
			}
		}
	}

	char*** tmp = cells;
	cells = nextCells;
	nextCells = tmp;

	throwIfInvalidCellIsLive();
}

bool Environment::isValidCoordinate(int x, int y, int z)
{
	return (x % 2 + y % 2 + z % 2 == 1) &&
		x >= 0 && x < size &&
		y >= 0 && y < size &&
		z >= 0 && z < size;
}

int Environment::getLiveCellCount()
{
	return liveCellCount;
}

char Environment::getCellState(int x, int y, int z)
{	
	int wrappedX = wrapCoordinateValue(x);
	int wrappedY = wrapCoordinateValue(y);
	int wrappedZ = wrapCoordinateValue(z);

	throwIfInvalid(wrappedX, wrappedY, wrappedZ);

	return cells[wrappedX][wrappedY][wrappedZ];
}

int Environment::wrapCoordinateValue(int u)
{
	if (u < 0) {
		u += size;
	}
	else if (u >= size) {
		u -= size;
	}
	return u;
}



void Environment::throwIfInvalid(int x, int y, int z)
{
#ifndef NDEBUG
    assert(false);
	if (!isValidCoordinate(x, y, z)) {
		throw InvalidCoordinateException(x, y, z);
	}
#endif // DEBUG
}

void Environment::throwIfInvalidCellIsLive()
{
#ifndef NDEBUG
	for (int x = 0; x < size; x++) {
		for (int y = 0; y < size; y++) {
			for (int z = 0; z < size; z++) {
				if (!isValidCoordinate(x, y, z) && cells[x][y][z] == 1) {
					throw InvalidLiveCoordinateException(x, y, z);
				}
			}
		}
	}
#endif // DEBUG
}