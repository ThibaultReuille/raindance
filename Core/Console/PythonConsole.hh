#pragma once

#ifdef Py_PYTHON_H

#include <raindance/Core/Headers.hh>

#include <Python.h>
#include <raindance/Core/Console/Console.hh>

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
 		// TODO : make it generic
		#if defined(_MSC_VER)
			char start[] = "E:/VS2013/Release/Scripts/start.py";
		#else
			char start[] = "./Scripts/start.py";
		#endif
		char mode[] = "r";

		PyRun_SimpleFile(PyFile_AsFile(PyFile_FromString(start, mode)), start);
	}

	bool execute(IScript* script)
	{
		PyRun_SimpleString(script->source().c_str());
		return true;
	}
};

#endif

