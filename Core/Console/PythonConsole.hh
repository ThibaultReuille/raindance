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

	virtual bool execute(const std::string& command)
	{
		end();

		auto callback = getScript(std::string("#console"));
		if (callback != NULL)
		{
			std::string eval = callback->source() + "(\"" + command + "\")"; 
			PyRun_SimpleString(eval.c_str());
		}
		else
			PyRun_SimpleString(command.c_str());

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

