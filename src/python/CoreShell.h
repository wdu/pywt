// This may look like C code, but it's really -*- C++ -*-
/*
 * Copyright (C) 2013 Emweb bvba, Belgium.
 *
 * See the LICENSE file for terms of use.
 */


#ifndef CORESHELL
#define CORESHELL

class PyCPPClassInstance;

class CoreShell {
	public:
		PyCPPClassInstance* pyobj;

		CoreShell(): pyobj(0) {}

		void setPyObject(PyCPPClassInstance* obj) {
			pyobj = obj;
		}
};

#endif
