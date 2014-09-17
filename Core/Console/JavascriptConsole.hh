#pragma once

#if EMSCRIPTEN

#include <raindance/Core/Headers.hh>
#include <raindance/Core/Console/Console.hh>

class JavascriptConsole : public Console
{
public:
	JavascriptConsole(int argc, char** argv)
	{
		(void) argc;
		(void) argv;
	}

	~JavascriptConsole()
	{
	}

	void initialize()
	{
		// TODO : Run Javascript initialization script
	}

	virtual void begin() {}

	virtual void end() {}

	bool execute(IScript* script)
	{
		emscripten_run_script(script->source().c_str());
		return true;
	}
};

#endif
