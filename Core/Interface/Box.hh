#pragma once

#include <raindance/Core/Headers.hh>

namespace Document
{

/*

Box Model
		                                TOP
		+--------------------------------------------------------------------------+
		|                                                                          |
		|                               Margin                                     |
		|                                                                          |
		|        +----------------------Border----------------------------+        |
		|        |                                                        |        |
		|        |                      Padding                           |        |
		|        |                                                        |        |
		|        |        +--------- <- Width  -> ----------------+       |        |
		|        |        |                                       |       |        |
		|        |        ^                                       |       |        |
   LEFT |        |        |                                       |       |        | RIGHT
		|        |        Height                                  |       |        |
		|        |        |                                       |       |        |
		|        |        v                                       |       |        |
		|        |        |                                       |       |        |
		|        |        +---------------------------------------+       |        |
		|        |                                                        |        |
		|        |                                                        |        |
		|        |                                                        |        |
		|        +--------------------------------------------------------+        |
		|                                                                          |
		|                                                                          |
		|                                                                          |
		+--------------------------------------------------------------------------+
	                                	BOTTOM
*/

class Box
{
public:
	Box()
	{
		X = glm::vec2(0, 1);
		Y = glm::vec2(0, 1);
		Z = glm::vec2(0, 1);
		Color = glm::vec4(WHITE, 1.0);
	}

	virtual ~Box()
	{
	}

	inline float getWidth() { return X[0] + X[1]; }
	inline float getHeight() { return Y[0] + Y[1]; }
	inline float getDepth() { return Z[0] + Z[1]; }

	void left(float f) { X[0] = f; }
	float left() { return X[0]; }
	
	void right(float f) { X[1] = f; }
	float right() { return X[1]; }

	void top(float f) { Y[0] = f; }
	float top() { return Y[0]; }

	void bottom(float f) { Y[1] = f; }
	float bottom() { return Y[1]; }

	void near(float f) { Z[0] = f; }
	float near() { return Z[0]; }

	void far(float f) { Z[1] = f; }
	float far() { return Z[1]; }

	void color(const glm::vec4& color) { Color = color; }
	glm::vec4 color() { return Color; }

	glm::vec2 X; // Left, Right
	glm::vec2 Y; // Top, Bottom
	glm::vec2 Z; // Near, Far

	glm::vec4 Color;
};

}