#pragma once

#include <raindance/Core/Headers.hh>

class View
{
public:
	virtual ~View() {};

	virtual const char* name() const = 0;

	virtual void initialize() = 0;

	virtual void draw() = 0;

	virtual void idle() = 0;
};

