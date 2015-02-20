#pragma once

#include <raindance/Core/Headers.hh>
#include <raindance/Core/Transformation.hh>
#include <raindance/Core/Icon.hh>
#include <raindance/Core/Console/Shell.hh>

#include <raindance/Core/GUI/Widgets/Widget.hh>
#include <raindance/Core/GUI/Widgets/ScriptWidget.hh>
#include <raindance/Core/GUI/Widgets/TextWidget.hh>

class HUD : public Controller
{
public:
	HUD(const Viewport& viewport)
	{
		m_WindowWidth = (int) viewport.getDimension()[0];
		m_WindowHeight = (int) viewport.getDimension()[1];
		LOG("[HUD] Creating HUD with %ix%i viewport\n", m_WindowWidth, m_WindowHeight);

		m_Camera.setOrthographicProjection(0.0f, (float)m_WindowWidth, 0.0f, (float)m_WindowHeight, 0.001f, 100.f);
		m_Camera.lookAt(glm::vec3(0, 0, 1), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));

		m_Logo = new Icon();
		m_Logo->load("logo", Assets_Textures_umbrella_logo_png, sizeof(Assets_Textures_umbrella_logo_png));

		m_WidgetFont = new Font();
		m_WidgetDimension = glm::vec2(16, 16);
		m_WidgetSpacing = 10;

		m_Shell = new Shell();

		m_ScriptWidgetGroup = NULL;

		m_ShowShell = true;

		m_WidgetPick = NULL;
		m_Context = NULL;
	}

	virtual ~HUD()
	{
		SAFE_DELETE(m_Shell);
		SAFE_DELETE(m_Logo);
		SAFE_DELETE(m_ScriptWidgetGroup);
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

		SAFE_DELETE(m_ScriptWidgetGroup);
		m_ScriptWidgetGroup = new WidgetGroup("Scripts", WidgetGroup::TOP_RIGHT);

		float fontRatio = m_WidgetDimension.y / (m_WidgetFont->getSize() * m_WidgetFont->getAscender());

		unsigned int count = 0;
		std::vector<IScript*>::iterator its;
		for (its = console->scripts_begin(); its != console->scripts_end(); ++its)
		{
			caption = (*its)->name();

			// NOTE : Script names starting with '#' are reserved for engine callbacks
			if (caption.size() > 0 && caption[0] == '#')
				continue;

			tl = glm::vec3(xmargin - m_WidgetDimension.x, ymargin + count * ypadding, 0);

			ScriptWidget* scriptWidget = new ScriptWidget(caption.c_str(), NULL, tl, m_WidgetDimension, caption);
			m_ScriptWidgetGroup->add(scriptWidget);

			TextWidget* textWidget = new TextWidget(caption.c_str(), NULL, glm::vec3(0, 0, 0), m_WidgetDimension);
			textWidget->text().set(caption.c_str(), m_WidgetFont);
			tl.x -= m_WidgetSpacing + fontRatio * textWidget->text().getWidth();
			textWidget->setPosition(tl);
			m_ScriptWidgetGroup->add(textWidget);

			count++;
		}
	}

	void draw(Context* context)
	{
		Transformation transformation;

		glDisable(GL_DEPTH_TEST);
		glDisable(GL_BLEND);
		
		// Draw Logo
		transformation.push();
			transformation.translate(glm::vec3(m_WindowWidth - 64 / 2 - 10, 10 + 64 / 2, 0));
			transformation.scale(glm::vec3(64, 64, 0));
			m_Logo->draw(m_Context, m_Camera.getViewProjectionMatrix() * transformation.state(), glm::vec4(1.0, 1.0, 1.0, 1.0), 0);
		transformation.pop();

		// TODO : Deprecate Menu
		if (m_ScriptWidgetGroup && m_ShowShell)
			m_ScriptWidgetGroup->draw(m_Context, glm::mat4(), m_Camera.getViewMatrix(), m_Camera.getProjectionMatrix());

		// Draw Shell
		if (m_ShowShell)
		{
			transformation.push();
				transformation.translate(glm::vec3(10, 10, 0));
				m_Shell->draw(m_Context, m_Camera, transformation);
			transformation.pop();
		}

		glEnable(GL_DEPTH_TEST);
	}

	void reshape(const Viewport& viewport)
	{
		m_WindowWidth = viewport.getDimension()[0];
		m_WindowHeight = viewport.getDimension()[1];

		m_Camera.reshape(viewport);
		m_Camera.setOrthographicProjection(0.0f, viewport.getDimension()[0], 0.0f,  viewport.getDimension()[1], 0.001f, 100.f);
        m_Camera.lookAt(glm::vec3(0, 0, 1), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));

		if (m_ScriptWidgetGroup)
			m_ScriptWidgetGroup->reshape(m_WindowWidth, m_WindowHeight);
	}

	IWidget* pickWidget(const glm::vec2& pos)
	{
		IWidget* pick = NULL;

		pick = m_ScriptWidgetGroup->pickWidget(pos);
		if (pick != NULL)
			return pick;

		return NULL;
	}

	IWidget* getWidgetPick() { return m_WidgetPick; }

	void onKey(int key, int scancode, int action, int mods) override
	{
		(void) scancode;
		(void) mods;
		
		if (action == GLFW_PRESS && key == GLFW_KEY_TAB)
			m_ShowShell = !m_ShowShell;

		if (m_ShowShell)
			m_Shell->onKey(key, scancode, action, mods);
	}
    
    void onChar(unsigned codepoint) override
    {
        m_Shell->onChar(codepoint);
    }

	void onMouseDown(const glm::vec2& pos) override
	{
		m_WidgetPick = pickWidget(pos);
	}

	void onMouseClick(const glm::vec2& pos) override
	{
		m_WidgetPick = pickWidget(pos);
		if (m_WidgetPick != NULL)
			m_WidgetPick->onMouseClick(m_Context->messages(), pos);
	}

private:
	Context* m_Context;

	Shell* m_Shell;

	int m_WindowWidth;
	int m_WindowHeight;

	Camera m_Camera;
	Icon* m_Logo;

	WidgetGroup* m_ScriptWidgetGroup;
	Font* m_WidgetFont;
	glm::vec2 m_WidgetDimension;
	float m_WidgetSpacing;

	IWidget* m_WidgetPick;

	bool m_ShowShell;
};
