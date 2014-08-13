#pragma once

#include <raindance/Core/Headers.hh>

class IResource
{
public:
	virtual const std::string& name() const = 0;

	virtual unsigned long memory() const = 0;

	virtual ~IResource() = 0;
};

IResource::~IResource() {}

