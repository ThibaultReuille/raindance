#pragma once

#include <raindance/Core/Headers.hh>
#include <raindance/Core/Transformation.hh>
#include <raindance/Core/Controller.hh>
#include <raindance/Core/Interface/Box.hh>
#include <raindance/Core/GUI/Viewport.hh> // TODO: Move Viewport.hh inside Interface/

#include <raindance/Core/Primitives/Rectangle.hh>
#include <raindance/Core/FS.hh>
#include <raindance/Core/Variables.hh>

namespace Document
{

struct Length
{
public:
	enum Mode
	{
		PIXELS,
		PERCENTS
	};

	Length(Mode mode, float value)
	: m_Mode(mode), m_Value(value)
	{
	}

	inline Mode mode() { return m_Mode; }
	inline float value() { return m_Value; }

private:
	Mode m_Mode;
	float m_Value;
};

struct Style
{
	enum PositionType
	{
		ABSOLUTE,
		RELATIVE
	};

	enum AlignType
	{
		LEFT,
		RIGHT,
		CENTER
	};

	Style()
	: 	Position(RELATIVE),
		Align(LEFT),
		Left(Length::PIXELS, 0.0),
		Top(Length::PIXELS, 0.0),
		Near(Length::PIXELS, 0.0),
		Width(Length::PERCENTS, 1.0),
		Height(Length::PERCENTS, 1.0),
		BackgroundColor(glm::vec4(1.0, 1.0, 1.0, 1.0)),
		Pickable(true)
	{
	}

	virtual ~Style()
	{
	}

	PositionType Position;
	AlignType Align;

	Length Left;
	Length Top;
	Length Near;

	Length Width;
	Length Height;

	glm::vec4 BackgroundColor;

	bool Pickable;
};

class Background
{
public:
	Background()
	{
		m_Color = glm::vec4(1.0, 1.0, 1.0, 1.0);
		m_Texture = NULL;

		m_Camera.setOrthographicProjection(0, 1, 0, 1, 0, 1);
		m_Camera.lookAt(glm::vec3(0, 0, 1), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));
	}

	virtual ~Background()
	{
		if (m_Texture != NULL)
			ResourceManager::getInstance().unload(m_Texture);
	}

	void loadImage(const char* name, const FS::BinaryFile& image)
	{
		m_Texture = ResourceManager::getInstance().loadTexture(name, (unsigned char*)image.content().data(), image.content().size());
		m_Rectangle.setTexture(m_Texture);
	}

	void draw(Context* context)
	{
		glDisable(GL_DEPTH_TEST);
		glEnable(GL_BLEND);
		glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
		
		Transformation transformation;

		m_Rectangle.draw(context, m_Camera, transformation);
	}

	inline void setColor(const glm::vec4& color)
	{
		m_Color = color;
		m_Rectangle.setColor(m_Color);
	}

private:
	Camera m_Camera;
	Rectangle m_Rectangle;
	glm::vec4 m_Color;
	Texture* m_Texture;
};

class Node : public Controller
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

	Node(Node* parent = NULL)
	{
		m_Parent = parent;
		m_Clicks = 0;
		m_Refresh = true;
	}

	virtual ~Node()
	{
	}

	virtual void draw(Context* context)
	{
		(void) context;
	}

	virtual void idle(Context* context)
	{
		(void) context;
	}

	virtual void request(const Variables& input, Variables& output)
	{
		(void) input;
		(void) output;	
	}

	virtual void update()
	{
		// Update Dimensions
		{
			if (style().Width.mode() == Length::PIXELS)
				content().X = glm::vec2(0.0, style().Width.value());
			else if (style().Width.mode() == Length::PERCENTS && parent() != NULL)
				content().X = glm::vec2(0.0, style().Width.value() * parent()->content().getWidth());
			else
			{
				LOG("Error: Document has relative width but no parent!\n");
			}

			if (style().Height.mode() == Length::PIXELS)
				content().Y = glm::vec2(0.0, style().Height.value());
			else if (style().Height.mode() == Length::PERCENTS && parent() != NULL)
				content().Y = glm::vec2(0.0, style().Height.value() * parent()->content().getHeight());
			else
			{
				LOG("Error: Document has relative height but no parent!\n");
			}

			content().Z = glm::vec2(0.0, 0.0);

			padding().X = glm::vec2(0, 0); 
			padding().Y = glm::vec2(0, 0); 
			padding().Z = glm::vec2(0, 0); 
			
			border().X = glm::vec2(0, 0); 
			border().Y = glm::vec2(0, 0); 
			border().Z = glm::vec2(0, 0); 
			
			margin().X = glm::vec2(0, 0); 
			margin().Y = glm::vec2(0, 0); 
			margin().Z = glm::vec2(0, 0); 
		}

		// NOTE: Update Position
		{
			glm::vec3 pos(0.0, 0.0, 0.0);
			if (style().Position == Style::RELATIVE)
			{
       			pos = parent()->position() +
       				glm::vec3
       				(
		            	parent()->margin().left() +   parent()->border().left() +   parent()->padding().left(),
		            	parent()->margin().bottom() + parent()->border().bottom() + parent()->padding().bottom(),
		            	0
		            );
       		}

       		if (style().Align == Style::LEFT)
       		{
       		}
			else if (style().Align == Style::RIGHT && parent() != NULL)
				pos.x += parent()->content().getWidth() - getDimension().x;
			else if (style().Align == Style::CENTER && parent() != NULL)
				pos.x += parent()->content().getWidth() / 2 - getDimension().x / 2;
			else
			{
				LOG("Error: Can't compute alignment, document has no parent!\n");
			}

			if (style().Left.mode() == Length::PIXELS)
				pos.x += style().Left.value();
			else if (style().Left.mode() == Length::PERCENTS && parent() != NULL)
				pos.x += style().Left.value() * parent()->content().getWidth();
			else
			{
				LOG("Error: Document has relative 'left' property but no parent!\n");
			}

			if (style().Top.mode() == Length::PIXELS)
				pos.y -= style().Top.value();
			else if (style().Top.mode() == Length::PERCENTS && parent() != NULL)
				pos.y -= style().Top.value() * parent()->content().getHeight();
			else
			{
				LOG("Error: Document has relative 'top' property but no parent!\n");
			}

			if (style().Near.mode() == Length::PIXELS)
				pos.z += style().Near.value();
			else if (style().Top.mode() == Length::PERCENTS && parent() != NULL)
				pos.z += style().Near.value() * parent()->content().getDepth();
			else
			{
				LOG("Error: Document has relative 'near' property but no parent!\n");
			}

			position(pos);
		}

		// Update Colors
		{
			content().Color = glm::vec4(WHITE, 1.0);
			padding().Color = glm::vec4(WHITE, 0.0);
			border().Color = glm::vec4(BLACK, 1.0);
			margin().Color = glm::vec4(WHITE, 0.0);

			background().setColor(style().BackgroundColor);
		}
	}


	virtual Node* pick(const glm::vec2& position, PickRegion* region)
	{
		if (!style().Pickable)
			return NULL;

		auto box_position = glm::vec2(0, 0);
		auto box_dimension = getDimension().xy();

		for (int i = 0; i < 4; i++)
		{
			if (position.x < box_position.x || position.x >= box_dimension.x ||
				position.y < box_position.y || position.y >= box_dimension.y)
			{
				auto r = static_cast<PickRegion>(i - 1);
				if (region != NULL)
					*region = r;
				return r == OUTSIDE ? NULL : this;
			}

			box_position += glm::vec2(m_Boxes[i].X[0], m_Boxes[i].Y[0]);
			box_dimension -= glm::vec2(m_Boxes[i].getWidth(), m_Boxes[i].getHeight());
		}

		if (region != NULL)
			*region = CONTENT;
		
		return this;
	}

	void onMouseClick(const glm::vec2& pos) override
	{
		PickRegion region;

		pick(pos,&region);

		if (region == OUTSIDE || region == PADDING)
			return;

		m_Clicks++;

		if (m_Clicks % 2 == 1)
			border().Color = glm::vec4(ORANGE, 1.0);
		else
			border().Color = content().Color;
	}

    virtual void onKey(int key, int scancode, int action, int mods)
    {     
    	(void) key;
    	(void) scancode;
    	(void) action;
    	(void) mods;
    }

    virtual void onChar(unsigned codepoint)
    {
    	(void) codepoint;
    }
	
	virtual glm::vec3 getDimension()
	{
		return glm::vec3(
			content().getWidth() + padding().getWidth() + border().getWidth() + margin().getWidth(),
			content().getHeight() + padding().getHeight() + border().getHeight() + margin().getHeight(),
			content().getDepth() + padding().getDepth() + border().getDepth() + margin().getDepth()
		);
	}

	inline Document::Box& box(int i) { return m_Boxes[i]; }

	inline Document::Box& margin() { return m_Boxes[0]; }
	inline Document::Box& border() { return m_Boxes[1]; }
	inline Document::Box& padding() { return m_Boxes[2]; }
	inline Document::Box& content() { return m_Boxes[3]; }

	inline const glm::vec3& position() { return m_Position; }
	inline void position(const glm::vec3& position) { m_Position = position; }

	inline bool refresh() { return m_Refresh; }
	inline void refresh(bool r) { m_Refresh = r; }

	inline void parent(Node* parent) { m_Parent = parent; }
	inline Node* parent() { return m_Parent; }

	inline void style(const Style& style) { m_Style = style; }
	inline Style& style() { return m_Style; }

	inline Background& background() { return m_Background; }

protected:
	Node* m_Parent;
	Style m_Style;
	Background m_Background;

	int m_Clicks;
	bool m_Refresh;
	glm::vec3 m_Position;
	Document::Box m_Boxes[4]; // NOTE: Margin, Border, Padding, Content
};

}

