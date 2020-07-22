#ifndef AABB_H
#define AABB_H

#include "Vector.h"



class AABB
{

public:
	void SetPosition(int x, int y);
	void SetDimension(int width, int height);

public:

	bool IsColliding(const AABB& secondBox) const;
private:

	void CreateBox();

private:

	Vector<int> m_min;
	Vector<int> m_max;
	Vector<int> m_position;
	Vector<int> m_dimension;
};

#endif