#pragma once

#include <raindance/Core/Headers.hh>
#include <raindance/Core/Transformation.hh>

namespace rd
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

class Document
{
public:

	enum PickRegion
	{
		OUTSIDE = -1,
		MARGIN = 0,
		BORDER = 1,
		PADDING = 2,
		CONTENT = 3
	};

	Document(const glm::vec3& dimension)
	{
		content().X = glm::vec2(0, dimension.x);
		content().Y = glm::vec2(0, dimension.y);
		content().Z = glm::vec2(0, dimension.z);
		content().Color = glm::vec4(WHITE, 1.0);

		padding().X = glm::vec2(0, 0); 
		padding().Y = glm::vec2(0, 0); 
		padding().Z = glm::vec2(0, 0); 
		padding().Color = glm::vec4(WHITE, 0.0);

		border().X = glm::vec2(0, 0); 
		border().Y = glm::vec2(0, 0); 
		border().Z = glm::vec2(0, 0); 
		border().Color = glm::vec4(BLACK, 1.0);

		margin().X = glm::vec2(0, 0); 
		margin().Y = glm::vec2(0, 0); 
		margin().Z = glm::vec2(0, 0); 
		margin().Color = glm::vec4(WHITE, 0.0);
	}

	virtual ~Document()
	{
	}

	virtual glm::vec3 getDimension()
	{
		return glm::vec3(
			content().getWidth() + padding().getWidth() + border().getWidth() + margin().getWidth(),
			content().getHeight() + padding().getHeight() + border().getHeight() + margin().getHeight(),
			content().getDepth() + padding().getDepth() + border().getDepth() + margin().getDepth()
		);
	}

	PickRegion pick(const glm::vec2& position)
	{
		auto box_position = glm::vec2(0, 0);
		auto box_dimension = getDimension().xy();

		for (int i = 0; i < 4; i++)
		{
			if (position.x < box_position.x || position.x >= box_dimension.x ||
				position.y < box_position.y || position.y >= box_dimension.y)
				return static_cast<PickRegion>(i - 1);

			box_position += glm::vec2(m_Boxes[i].X[0], m_Boxes[i].Y[0]);
			box_dimension -= glm::vec2(m_Boxes[i].getWidth(), m_Boxes[i].getHeight());
		}

		return CONTENT;
	}

	inline Box& box(int i) { return m_Boxes[i]; }

	inline Box& margin() { return m_Boxes[0]; }
	inline Box& border() { return m_Boxes[1]; }
	inline Box& padding() { return m_Boxes[2]; }
	inline Box& content() { return m_Boxes[3]; }

	inline const glm::vec3& position() { return m_Position; }
	inline void position(const glm::vec3& position) { m_Position = position; }

protected:
	glm::vec3 m_Position;
	Box m_Boxes[4]; // Margin, Border, Padding, Content 
};

}