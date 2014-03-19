#pragma once

#include "Core/Headers.hh"
#include "Core/Transformation.hh"
#include "Core/Icon.hh"

#include "Core/HUD/Widgets/Widget.hh"
#include "Core/HUD/Widgets/ScriptWidget.hh"

class HUD : public Controller
{
public:
	HUD()
	{
		m_WindowWidth = glutGet(GLUT_WINDOW_WIDTH);
		m_WindowHeight = glutGet(GLUT_WINDOW_HEIGHT);

		m_Camera.setOrthographicProjection(0.0f, (float)m_WindowWidth, 0.0f, (float)m_WindowHeight, 0.001f, 100.f);
		m_Camera.lookAt(glm::vec3(0, 0, 1), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));

		m_Logo = new Icon();
		m_Logo->load("logo", Resources_Textures_umbrella_logo_png, sizeof(Resources_Textures_umbrella_logo_png));

		m_WidgetFont = new Font();
		m_WidgetDimension = glm::vec2(16, 16);
		m_WidgetSpacing = 10;
		m_WidgetFontWidth = 9;

		m_ScriptWidgetGroup = NULL;

		m_ShowMenu = true;
	}

	~HUD()
	{
		delete m_Logo;
		delete m_ScriptWidgetGroup;
	}

	void bind(Context* context)
	{
		m_Context = context;
	}

	void buildScriptWidgets(Console* console)
	{
		float xmargin = -m_WidgetSpacing;
		float ymargin = -m_WidgetSpacing;
		float ypadding = -m_WidgetSpacing - m_WidgetDimension.y;

		std::string caption;
		glm::vec3 tl;

		if (m_ScriptWidgetGroup != NULL)
			delete m_ScriptWidgetGroup;

		m_ScriptWidgetGroup = new WidgetGroup("Scripts", WidgetGroup::TOP_RIGHT);

		std::vector<IScript*>::iterator its;
		unsigned int count = 0;
		for (its = console->scripts_begin(); its != console->scripts_end(); ++its)
		{
			caption = (*its)->name();

			 // NOTE : Script names starting with '#' are reserved for engine callbacks
			if (caption.size() > 0 && caption[0] == '#')
				continue;

			tl = glm::vec3(xmargin - m_WidgetDimension.x, ymargin + count * ypadding, 0);

			ScriptWidget* scriptWidget = new ScriptWidget(caption.c_str(), NULL, tl, m_WidgetDimension, caption);
			m_ScriptWidgetGroup->add(scriptWidget);
			tl.x -= m_WidgetSpacing + (caption.size() + 1) * m_WidgetFontWidth;

			TextWidget* textWidget = new TextWidget(caption.c_str(), NULL, tl, glm::vec2(1.0, 1.0));
			m_ScriptWidgetGroup->add(textWidget);
			textWidget->text().set(caption.c_str(), m_WidgetFont);

			count++;
		}
	}

	void draw(Context* context)
	{
		glDisable(GL_DEPTH_TEST);
		glDisable(GL_BLEND);

		Transformation transformation;

		if (m_ShowMenu)
			m_ScriptWidgetGroup->draw(m_Context, transformation.state(), m_Camera.getViewMatrix(), m_Camera.getProjectionMatrix());

		transformation.translate(glm::vec3(m_WindowWidth - 64 / 2 - 10, 10 + 64 / 2, 0));
		transformation.scale(glm::vec3(64, 64, 0));
		m_Logo->draw(context, m_Camera.getViewProjectionMatrix() * transformation.state(), glm::vec4(1.0, 1.0, 1.0, 1.0), 0);

		glEnable(GL_DEPTH_TEST);
	}

	void reshape(int width, int height)
	{
		m_WindowWidth = width;
		m_WindowHeight = height;

		m_Camera.reshape(width, height);
		m_Camera.setOrthographicProjection(0.0f, (float)width, 0.0f, (float)height, 0.001f, 100.f);
		m_Camera.lookAt(glm::vec3(0, 0, 1), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));

		m_ScriptWidgetGroup->reshape(width, height);
	}

	IWidget* pickWidget(int x, int y)
	{
		IWidget* pick = NULL;

		pick = m_ScriptWidgetGroup->pickWidget(x, y);
		if (pick != NULL)
			return pick;

		return NULL;
	}

	IWidget* getWidgetPick() { return m_WidgetPick; }

	virtual void idle()
	{
	}

	virtual void onKeyboard(unsigned char key, Controller::KeyEvent event)
	{
		if (event == Controller::KEY_DOWN && key == 9) // TAB
		{
			m_ShowMenu = !m_ShowMenu;
			return;
		}
	}

	virtual void onSpecial(int key, Controller::KeyEvent event)
	{
		(void) key;
		(void) event;
	}

	virtual void onMouseDown(int x, int y)
	{
		(void) x;
		(void) y;
		m_WidgetPick = NULL;
	}
	virtual void onMouseClick(int x, int y)
	{
		m_WidgetPick = m_ScriptWidgetGroup->pickWidget(x, y);
		if (m_WidgetPick != NULL)
			m_WidgetPick->onMouseClick(m_Context->messages(), x, y);
	}
	virtual void onMouseDoubleClick(int x, int y)
	{
		(void) x;
		(void) y;
		m_WidgetPick = NULL;
	}
	virtual void onMouseTripleClick(int x, int y)
	{
		(void) x;
		(void) y;
		m_WidgetPick = NULL;
	}
	virtual void onMouseMove(int x, int y, int dx, int dy)
	{
		(void) x;
		(void) y;
		(void) dx;
		(void) dy;
		m_WidgetPick = NULL;
	}

private:
	Context* m_Context;

	int m_WindowWidth;
	int m_WindowHeight;

	Camera m_Camera;
	Icon* m_Logo;

	WidgetGroup* m_ScriptWidgetGroup;
	Font* m_WidgetFont;
	float m_WidgetFontWidth;
	glm::vec2 m_WidgetDimension;
	float m_WidgetSpacing;

	IWidget* m_WidgetPick;

	bool m_ShowMenu;
};
