#pragma once

#include <raindance/Core/Headers.hh>
#include <raindance/Core/Geometry.hh>

class BezierCurve
{
public:
	struct ControlPoint
	{
		ControlPoint(const glm::vec3& pos, const glm::vec4& color) : Position(pos), Color(color) {}
		ControlPoint(const glm::vec4& pos) : Position(pos), Color(glm::vec4(1.0, 1.0, 1.0, 1.0)) {}
		glm::vec3 Position;
		glm::vec4 Color;
	};

	struct Vertex
	{
		Vertex(const glm::vec3& pos, const glm::vec4& color) : Position(pos), Color(color) {}
		Vertex(const glm::vec3& pos) : Position(pos), Color(glm::vec4(1.0, 1.0, 1.0, 1.0)) {}
		glm::vec3 Position;
		glm::vec4 Color;
	};

	BezierCurve()
	{
		m_FirstUpdate = true;
		m_Divisions = 5;
	}

	~BezierCurve()
	{
	}

	inline void clearControlPoints()
	{
		m_ControlPoints.clear();
	}

	inline void addControlPoint(const ControlPoint& point)
	{
		m_ControlPoints.push_back(point);
	}

    inline ControlPoint getControlPoint(unsigned long i) { return m_ControlPoints[i]; }

    inline void setControlPoint(unsigned long i, const ControlPoint& cp) { m_ControlPoints[i] = cp; }

    inline unsigned long size() { return m_ControlPoints.size(); }

	inline void setDivisions(unsigned int divisions) { m_Divisions = divisions; }

	void update()
	{
		m_VertexBuffer.clear();

		for (unsigned int i = 0; i <= m_Divisions; i++)
		{
			float t = (float) i / (float) m_Divisions;

			Vertex vertex = interpolate(m_ControlPoints, t);

			m_VertexBuffer.push(&vertex, sizeof(Vertex));
		}

		if (m_FirstUpdate)
		{
			m_VertexBuffer.describe("a_Position", 3, GL_FLOAT, sizeof(Vertex), 0);
			m_VertexBuffer.describe("a_Color",    4, GL_FLOAT, sizeof(Vertex), sizeof(glm::vec3));
			m_VertexBuffer.generate(Buffer::DYNAMIC);
		}
		else
		{
			m_VertexBuffer.update();
			m_VertexBuffer.describe("a_Position", 3, GL_FLOAT, sizeof(Vertex), 0);
			m_VertexBuffer.describe("a_Color", 4, GL_FLOAT, sizeof(Vertex), sizeof(glm::vec3));
		}
		m_FirstUpdate = false;
	}

	const Vertex interpolate(const std::vector<ControlPoint>& points, const float t)
	{
		switch(points.size())
		{
		case 0:
		case 1:
			LOG("[BEZIER] Not enough control points to make a curve !\n");
			return Vertex(glm::vec3(0, 0, 0), glm::vec4(1.0, 1.0, 1.0, 1.0));
		case 2:
			return Vertex(linear(points[0].Position, points[1].Position, t),
						  linear(points[0].Color,    points[1].Color,    t));
		case 3:
			return Vertex(quadratic(points[0].Position, points[1].Position, points[2].Position, t),
						  quadratic(points[0].Color,    points[1].Color,    points[2].Color,    t));
		case 4:
			return Vertex(cubic(points[0].Position, points[1].Position, points[2].Position, points[3].Position, t),
						  cubic(points[0].Color,    points[1].Color,    points[2].Color,    points[3].Color,    t));
		case 5:
			return Vertex(hypercubic(points[0].Position, points[1].Position, points[2].Position, points[3].Position, points[4].Position, t),
						  hypercubic(points[0].Color,    points[1].Color,    points[2].Color,    points[3].Color,    points[4].Color,    t));
		default:
			LOG("[BEZIER] High order curves are not supported yet !\n"); // TODO
			return Vertex(glm::vec3(0, 0, 0), glm::vec4(1.0, 1.0, 1.0, 1.0));
		}
	}

    static inline glm::vec3 linear(const glm::vec3& a, const glm::vec3& b, const float t)
    {
        return a  + (b - a) * t; // NOTE : Same as : (1 - t) * a + t * b
    }

    static inline glm::vec4 linear(const glm::vec4& a, const glm::vec4& b, const float t)
    {
        return a  + (b - a) * t; // NOTE : Same as : (1 - t) * a + t * b
    }

    static inline glm::vec3 quadratic(const glm::vec3& a, const glm::vec3& b, const glm::vec3& c, const float t)
    {
        return linear(linear(a, b, t), linear(b, c, t), t);
    }

    static inline glm::vec4 quadratic(const glm::vec4& a, const glm::vec4& b, const glm::vec4& c, const float t)
    {
        return linear(linear(a, b, t), linear(b, c, t), t);
    }

    static glm::vec3 cubic(const glm::vec3& a, const glm::vec3& b, const glm::vec3& c, const glm::vec3& d, const float t)
    {
        return linear(quadratic(a, b, c, t), quadratic(b, c, d, t), t);
    }

    static glm::vec4 cubic(const glm::vec4& a, const glm::vec4& b, const glm::vec4& c, const glm::vec4& d, const float t)
    {
        return linear(quadratic(a, b, c, t), quadratic(b, c, d, t), t);
    }

    static inline glm::vec3 hypercubic(const glm::vec3& a, const glm::vec3& b, const glm::vec3& c, const glm::vec3& d, const glm::vec3& e, const float t)
    {
        return linear(cubic(a, b, c, d, t), cubic(b, c, d, e, t), t);
    }

    static inline glm::vec4 hypercubic(const glm::vec4& a, const glm::vec4& b, const glm::vec4& c, const glm::vec4& d, const glm::vec4& e, const float t)
    {
        return linear(cubic(a, b, c, d, t), cubic(b, c, d, e, t), t);
    }

	static inline float bezier(const unsigned int order, const unsigned int nth, const float t)
	{
		return binomial(nth, order) * pow(1 - t, order - nth) * pow(t, nth);
	}

	static inline unsigned int binomial(unsigned int k, unsigned int n)
	{
		return factorial(n) / (factorial(n - k) * factorial(k));
	}

	static unsigned int factorial(unsigned int n)
	{
		unsigned int result = 1;
		for (unsigned int i = 2; i <= n; i++)
			result *= i;
		return result;
	}

	inline Buffer& getVertexBuffer() { return m_VertexBuffer; }

private:
	std::vector<ControlPoint> m_ControlPoints;
	unsigned int m_Divisions;
	Buffer m_VertexBuffer;
	bool m_FirstUpdate;
};

