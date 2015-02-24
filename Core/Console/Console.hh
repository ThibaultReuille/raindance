#pragma once

#include <raindance/Core/Headers.hh>
#include <raindance/Core/Clock.hh>

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

class Job
{
public:
	typedef unsigned long ID;

	Job(ID id, IScript* script, Timecode period)
	{
		m_ID = id;
		m_Script = script;
		m_Period = period;

		m_isInitialized = false;
		m_LastRun = 0;
	}

	void initialize(float seconds)
	{
		m_isInitialized = true;
		m_LastRun = seconds;
	}

	inline ID id() { return m_ID; }

	inline void setScript(IScript* script) { m_Script = script; }
	inline IScript* getScript() { return m_Script; }
	
	inline void setPeriod(float period) { m_Period = period; }
	inline float getPeriod() { return m_Period; }

	inline void setLastRun(float seconds) { m_LastRun = seconds; }
	inline float getLastRun() { return m_LastRun; }

	inline bool isInitialized() { return m_isInitialized; }

private:
	bool m_isInitialized;
	ID m_ID;
	IScript* m_Script;
	float m_Period;
	float m_LastRun;
};

class Console
{
public:
	Console() : m_FirstIdle(true) {}

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

	Job::ID addJob(IScript* script, float period)
	{
		LOG("Job %p %f\n", script, period);
		auto id = m_Jobs.size();
		auto job = new Job(id, script, period);
		m_Jobs.push_back(job);

		return id;
	}

	void removeJob(Job::ID id)
	{
		SAFE_DELETE(m_Jobs[id]);
	}

	void idle(Clock& clock)
	{
		if (m_FirstIdle)
		{
			m_Seconds = clock.seconds();
			m_FirstIdle = false;
			return;
		}

		float seconds = clock.seconds() - m_Seconds;

		for (auto job : m_Jobs)
		{
			if (job == NULL)
				continue;

			if (!job->isInitialized())
				job->initialize(seconds);

			else if (seconds - job->getLastRun() > job->getPeriod())
			{
				auto script = job->getScript();
				if (script != NULL)
					execute(script->source());
				job->setLastRun(seconds);
			}
		} 
	}

	virtual void initialize() = 0;

	virtual void begin() = 0;
	virtual void end() = 0;
	
	virtual bool execute(const std::string& command) = 0;

	inline const std::vector<IScript*>::iterator scripts_begin() { return m_Scripts.begin(); }
	inline const std::vector<IScript*>::iterator scripts_end() { return m_Scripts.end(); }

private:
	std::vector<IScript*> m_Scripts;
	float m_Seconds;
	bool m_FirstIdle;
	std::vector<Job*> m_Jobs;
};

