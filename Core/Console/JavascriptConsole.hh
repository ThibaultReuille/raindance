#pragma once

#if EMSCRIPTEN

#include "Core/Console/Console.hh"

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

	bool execute(IScript* script)
	{
		emscripten_run_script(script->source().c_str());
		return true;
	}
};

#endif
