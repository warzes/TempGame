#include "stdafx.h"
#include "ScopeProfiler.h"

ScopeProfiler::ScopeProfiler(Profiler& profiler, std::string nodeName) :
	profiler(profiler),
	nodeName(std::move(nodeName)) {
	profiler.startNewProfile(this->nodeName);
}

ScopeProfiler::~ScopeProfiler() {
	profiler.stopProfile(nodeName);
}