#pragma once
class Coordinate
{
public:
	Coordinate(int x, int y, int z) : x(x), y(y), z(z) { };
	~Coordinate();
	const int x, y, z;
};

