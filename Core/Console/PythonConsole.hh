#pragma once

#ifdef Py_PYTHON_H

#include <Python.h>
#include "Core/Console/Console.hh"

class PythonConsole : public Console
{
public:
	PythonConsole(int argc, char** argv)
	{
		Py_Initialize();
		PySys_SetArgv(argc, argv);
	}

	virtual ~PythonConsole()
	{
		Py_Finalize();
	}

	void initialize()
	{
		FILE* fp = fopen("Scripts/start.py", "r");
		if (fp != NULL)
			PyRun_SimpleFile(fp, "__main__");
		fclose(fp);
	}

	bool execute(IScript* script)
	{
		PyRun_SimpleString(script->source().c_str());
		return true;
	}
};

#endif

