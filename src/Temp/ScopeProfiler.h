#pragma once

#include "Profiler.h"

class ScopeProfiler {
public:
	ScopeProfiler(Profiler&, std::string);
	~ScopeProfiler();

private:
	Profiler& profiler;
	std::string nodeName;
};