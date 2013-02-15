// This may look like C code, but it's really -*- C++ -*-
/*
 * Copyright (C) 2013 Emweb bvba, Belgium.
 *
 * See the LICENSE file for terms of use.
 */


#ifndef WTHOOK_H
#define WTHOOK_H

namespace Wt {
	class WApplication;
	class WEnvironment;
};



typedef struct {
	Wt::WApplication* (*createApplication)(const Wt::WEnvironment& env);
	void (*initThreading)();
	void (*finalizeThreading)();
	void (*setAppCreatorFunc)(PyObject*);
} WtHook;

#endif


