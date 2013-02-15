// This may look like C code, but it's really -*- C++ -*-
/*
 * Copyright (C) 2013 Emweb bvba, Belgium.
 *
 * See the LICENSE file for terms of use.
 */


#ifndef PYCPPCLASSINSTANCE_H
#define PYCPPCLASSINSTANCE_H

#include <Python.h>

class LibExtension;
class AbstractProxy;

class PyCPPClassInstance: public PyObject {
	public:
		PyObject* weakreflist;
		AbstractProxy* proxy;

		inline PyCPPClassInstance():weakreflist(0), proxy(0) {
		}

		inline void setPythonOwnership();
		inline void setCOwnership();

		inline bool isOwnedByPython() const; 
		inline bool isSubClassInstance() const;
		inline bool isDead() const;


};

#include "AbstractProxy.h"

// note: proxy should never be zero, except when an exception occurred in
// the constructor. In that case the ownership is irrelevant

void PyCPPClassInstance::setPythonOwnership() {
	if (!proxy) return;
	proxy->setPythonOwnership(true);
}
void PyCPPClassInstance::setCOwnership() {
	if (!proxy) return;
	proxy->setPythonOwnership(false);
}
bool PyCPPClassInstance::isOwnedByPython() const {
	if (!proxy) return false;
	return proxy->isOwnedByPython();
}

bool PyCPPClassInstance::isDead() const {
	if (!proxy) return true;
	return proxy->isDead();
}

bool PyCPPClassInstance::isSubClassInstance() const {
	if (!proxy) return false;
	return proxy->isSubClassInstance();
}

#endif


