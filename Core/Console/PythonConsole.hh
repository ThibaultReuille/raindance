#pragma once

#ifdef Py_PYTHON_H

#include <raindance/Core/Headers.hh>
#include <raindance/Core/Console/Console.hh>

#include <stdio.h>
#include <unistd.h>
#include <Python.h>

class PythonConsole : public Console
{
public:
	PythonConsole(int argc, char** argv)
	{
		PyEval_InitThreads();
		Py_Initialize();
		PySys_SetArgv(argc, argv);

		m_ThreadState = NULL;
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

	virtual ~PythonConsole()
	{
		Py_Finalize();
	}

	bool execute(IScript* script)
	{
		end();

		PyRun_SimpleString(script->source().c_str());

		begin();
		
		return true;
	}

	void begin()
	{
		m_ThreadState = PyEval_SaveThread();
	}

	void end()
	{
		PyEval_RestoreThread(m_ThreadState);
	}

private:
	PyThreadState* m_ThreadState;
};

#endif

