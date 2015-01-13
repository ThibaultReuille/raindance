#pragma once

#include <raindance/Core/Headers.hh>
#include <raindance/Core/Primitives/Quad.hh>
#include <raindance/Core/Font.hh>

class IWidget
{
public:
	IWidget(const char* name, IWidget* parent, glm::vec3 position, glm::vec2 dimension)
	{
		m_Name = std::string(name);
		m_Parent = parent;
		m_Position = position;
		m_Dimension = dimension;
		m_Visible = true;

		m_Shader = ResourceManager::getInstance().loadShader("shaders/widget",
		        Assets_Shaders_Widgets_widget_vert, sizeof(Assets_Shaders_Widgets_widget_vert),
		        Assets_Shaders_Widgets_widget_frag, sizeof(Assets_Shaders_Widgets_widget_frag));
		// m_Shader->dump();

		m_Quad.getVertexBuffer().mute("a_Normal", true);
		m_Quad.getVertexBuffer().mute("a_Texcoord", true);
	}

	virtual ~IWidget()
	{
	    ResourceManager::getInstance().unload(m_Shader);
	}

	virtual void onMouseClick(MessageQueue& messages, const glm::vec2& pos)
	{
		(void) messages;
		(void) pos;
	}

	virtual void draw(Context* context, glm::mat4 model, glm::mat4 view, glm::mat4 projection) = 0;

	void drawQuad(Context* context, const glm::mat4& model, const glm::mat4& view, const glm::mat4& projection)
    {
        Transformation transformation;

        transformation.set(model);
        transformation.scale(glm::vec3(m_Dimension.x, -m_Dimension.y, 1.0));

        m_Shader->use();
        m_Shader->uniform("u_ModelViewProjection").set(projection * view * transformation.state());
        m_Shader->uniform("u_Mode").set(0);

        context->geometry().bind(m_Quad.getVertexBuffer(), *m_Shader);
        context->geometry().drawElements(GL_TRIANGLES, m_Quad.getTriangleBuffer().size(), GL_UNSIGNED_BYTE, m_Quad.getTriangleBuffer().ptr());
        context->geometry().unbind(m_Quad.getVertexBuffer());

        m_Shader->uniform("u_Mode").set(1);

        context->geometry().bind(m_Quad.getVertexBuffer(), *m_Shader);
        glDrawElements(GL_LINES, m_Quad.getLineBuffer().size(), GL_UNSIGNED_BYTE, m_Quad.getLineBuffer().ptr());
        context->geometry().unbind(m_Quad.getVertexBuffer());
    }

	inline const std::string& name() { return m_Name; }
	inline const glm::vec2 dimension() { return m_Dimension; }
	inline const glm::vec3 position() { return m_Position; }

    inline void setPosition(const glm::vec3& position) { m_Position = position; }
    inline void setDimension(const glm::vec2& dimension) { m_Dimension = dimension; }

	inline bool visible() { return m_Visible; }
	inline void visible(const bool v) { m_Visible = v; }

protected:
	std::string m_Name;
	IWidget* m_Parent;
	Icon* m_Icon;
	glm::vec3 m_Position;
	glm::vec2 m_Dimension;
	bool m_Visible;

private:
    Quad m_Quad;
    Shader::Program* m_Shader;
};

class WidgetGroup : public IWidget
{
	struct WidgetItem
	{
		IWidget* Widget;
	};

public:
	enum OriginType
	{
		TOP_LEFT,
		TOP_RIGHT,
		BOTTOM_LEFT,
		BOTTOM_RIGHT,
		CENTER,
		TOP_CENTER,
		BOTTOM_CENTER
	};

	WidgetGroup(const char* name)
	: IWidget(name, NULL, glm::vec3(0, 0, 0), glm::vec2(0, 0))
	{
		m_Origin = TOP_LEFT;
		m_WindowWidth = 0;
		m_WindowHeight = 0;
	}

	WidgetGroup(const char* name, OriginType type)
	: IWidget(name, NULL, glm::vec3(0, 0, 0), glm::vec2(0, 0))
	{
		m_Origin = type;
	}

	virtual ~WidgetGroup()
	{
		std::vector<WidgetItem>::iterator it;
		for (it = m_Widgets.begin(); it != m_Widgets.end(); ++it)
		{
			delete it->Widget;
		}
	}

	inline void add(IWidget* widget)
	{
		WidgetItem item;
		item.Widget = widget;
		m_Widgets.push_back(item);
	}

	virtual void draw(Context* context, glm::mat4 model, glm::mat4 view, glm::mat4 projection)
	{
	    (void) context;
		(void) model;

		glDisable(GL_DEPTH_TEST);
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_DST_ALPHA);

		Transformation transformation;

		std::vector<WidgetItem>::iterator it;
		for (it = m_Widgets.begin(); it != m_Widgets.end(); ++it)
		{
			if (it->Widget->visible())
			{
				transformation.push();
				{
					transformation.translate(this->position() + it->Widget->position());
					it->Widget->draw(context, transformation.state(), view, projection);
				    // it->Widget->drawQuad(context, transformation.state(), view, projection);
				}
				transformation.pop();
			}
		}

		glEnable(GL_DEPTH_TEST);
	}

	virtual void reshape(int width, int height)
	{
		m_WindowWidth = width;
		m_WindowHeight = height;

		switch(m_Origin)
		{
		case TOP_LEFT:
			m_Position = glm::vec3(0, m_WindowHeight, 0);
			break;
		case TOP_RIGHT:
			m_Position = glm::vec3(m_WindowWidth, m_WindowHeight, 0);
			break;
		case BOTTOM_LEFT:
			m_Position = glm::vec3(0, 0, 0);
			break;
		case BOTTOM_RIGHT:
			m_Position = glm::vec3(m_WindowWidth, 0, 0);
			break;
		case CENTER:
			m_Position = glm::vec3(m_WindowWidth / 2, m_WindowHeight / 2, 0);
			break;
		case TOP_CENTER:
		    m_Position = glm::vec3(m_WindowWidth / 2, m_WindowHeight, 0);
		    break;
		case BOTTOM_CENTER:
		    m_Position = glm::vec3(m_WindowWidth / 2, 0, 0);
		    break;
		default:
			break;
		}
	}

	virtual IWidget* pickWidget(const glm::vec2& pos)
	{
		std::vector<WidgetItem>::iterator it;
		for (it = m_Widgets.begin(); it != m_Widgets.end(); ++it)
		{
			glm::vec2 topleft = glm::vec2(this->position() + it->Widget->position());
			glm::vec2 dimension = it->Widget->dimension();

			if (pos.x > topleft.x && pos.x < topleft.x + dimension.x &&
				m_WindowHeight - pos.y + dimension.y > topleft.y && m_WindowHeight - pos.y < topleft.y)
				return it->Widget;
		}

		return NULL;
	}

	void onMouseClick(MessageQueue& messages, const glm::vec2& pos) override
	{
		auto pick = pickWidget(pos);
		if (pick)
			pick->onMouseClick(messages, pos);
	}

private:
	OriginType m_Origin;
	int m_WindowWidth;
	int m_WindowHeight;
	std::vector<WidgetItem> m_Widgets;
};

