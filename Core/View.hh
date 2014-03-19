#pragma once

class View
{
public:
	virtual ~View() {};

	virtual void initialize() = 0;

	virtual void draw() = 0;

	virtual void idle() = 0;
};

