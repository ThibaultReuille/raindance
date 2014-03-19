#pragma once

#include "Core/Headers.hh"

class IScript
{
public:
	virtual const std::string& name() = 0;

	virtual const std::string& source() = 0;

	virtual ~IScript() = 0;
};

IScript::~IScript() {}

class StaticScript : public IScript
{
public:
	StaticScript(const std::string& name, const std::string& source)
	{
		m_Name = name;
		m_Source = source;
	}

	virtual ~StaticScript()
	{
	}

	virtual const std::string& name()
	{
		return m_Name;
	}

	virtual const std::string& source()
	{
		return m_Source;
	}

private:
	std::string m_Name;
	std::string m_Source;
};

class Console
{
public:
	virtual ~Console()
	{
		std::vector<IScript*>::iterator it;
		for (it = m_Scripts.begin(); it != m_Scripts.end(); ++it)
			delete *it;
	}

	inline void registerScript(IScript* script)
	{
		m_Scripts.push_back(script);
	}

	void unregisterScript(IScript* script)
	{
		std::vector<IScript*>::iterator it;

		it = std::find(m_Scripts.begin(), m_Scripts.end(), script);
		if (it != m_Scripts.end())
			m_Scripts.erase(it);
		else
			LOG("[CONSOLE] Script %p not found !\n", script);
	}

	IScript* getScript(const std::string& name)
	{
		std::vector<IScript*>::iterator its;
		for (its = m_Scripts.begin(); its != m_Scripts.end(); ++its)
		{
			if (name == (*its)->name())
				return *its;
		}
		return NULL;
	}

	virtual void initialize() = 0;

	virtual bool execute(IScript* script) = 0;

	inline const std::vector<IScript*>::iterator scripts_begin() { return m_Scripts.begin(); }

	inline const std::vector<IScript*>::iterator scripts_end() { return m_Scripts.end(); }

private:
	std::vector<IScript*> m_Scripts;
};

