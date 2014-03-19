#pragma once

#include <list>

#include "Core/Headers.hh"
#include "Core/Log.hh"

class IMessage
{
public:
	typedef unsigned int Type;

	enum
	{
		CORE = 0,
		SCRIPT = 1,
		WIDGET = 2,

		CUSTOM = 100
	};

	virtual Type type() = 0;

	virtual ~IMessage() {};
};


struct ScriptMessage : public IMessage
{
	ScriptMessage(const char* name) : Name(name) {}
	unsigned int type() { return IMessage::SCRIPT; }
	std::string Name;
};

struct WidgetMessage : public IMessage
{
	WidgetMessage(const char* message) : Name("unknown"), Message(message) {}
	WidgetMessage(const char* name, const char* message) : Name(name), Message(message) {}
	unsigned int type() { return IMessage::WIDGET; }
	std::string Name;
	std::string Message;
};

class IListener
{
public:
	virtual void notify(IMessage* message) = 0;
};

class MessageQueue
{
public:
	MessageQueue()
	{
	}

	~MessageQueue()
	{
	}

	// TODO : Make it thread safe
	void push(IMessage* message)
	{
		m_Messages.push_back(message);
	}

	inline unsigned long count()
	{
		return m_Messages.size();
	}

	void addListener(IListener* listener)
	{
		m_Listeners.push_back(listener);
	}

	void process()
	{
		while(m_Messages.size() > 0)
		{
			LOG("[SCHEDULER] %zu\n", m_Messages.size());

			IMessage* message = m_Messages.front();

			std::list<IListener*>::iterator it;
			for (it = m_Listeners.begin(); it != m_Listeners.end(); ++it)
			{
				(*it)->notify(message);
			}

			m_Messages.pop_front();
			delete message;
		}
	}

private:
	std::list<IMessage*> m_Messages;
	std::list<IListener*> m_Listeners;
};

