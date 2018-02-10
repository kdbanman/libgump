#pragma once

/*
Contains on and off cells.

Exposes state setters because coordinates are sparse(ish).

Knows how to evolve/iterate its cells.
- Warns and grows to accomodate expansion to boundary



Runtime pluggable processing pipeline.
EX:
I have three jobs.  A renderer job, which might block for a long time while a
user inspects the rendering from all sorts of angles.  A graph writer job that
writes a serialized identifier for the current environment state to a graph db,
linking the current environment to the previous environment in a parent-child
type relationship.  A logging job that just dumps basic stats, warnings, etc to
a file.

All three of those jobs might want to use some combination of environment
analysis stuff, like live cell count or convex hull computation.

The graph writer and possibly the renderer are stateful across iterations.
*/

#include <vector>

#include "Coordinate.h"

class Environment
{
public:
	Environment(int size);
	~Environment();

	void turnOn(int x, int y, int z);
	void turnOff(int x, int y, int z);

	void iterate();

	bool isValidCoordinate(int x, int y, int z);

	int getLiveCellCount();

	char getCellState(int x, int y, int z);

private:
	int wrapCoordinateValue(int u);

	void throwIfInvalid(int x, int y, int z);

	void throwIfInvalidCellIsLive();

	int size, liveCellCount;

	char*** cells;
	char*** nextCells;

	std::vector<Coordinate> validCoordinates;
};

