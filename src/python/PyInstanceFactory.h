// This may look like C code, but it's really -*- C++ -*-
/*
 * Copyright (C) 2013 Emweb bvba, Belgium.
 *
 * See the LICENSE file for terms of use.
 */


#ifndef PYINSTANCEFACTORY_H
#define PYINSTANCEFACTORY_H

#include <vector>
class PyCPPClassInstance;

class PyInstanceFactory {
	public:
		virtual bool check(const void* core) = 0;
		virtual PyCPPClassInstance* create(const void* core) = 0;
		virtual void addSubClass(PyInstanceFactory* subclass) {}

};

template <class T>
class NonPoly_PyInstanceFactory: public PyInstanceFactory {
	public:
		virtual bool check(const void* core) {return true;}
		virtual PyCPPClassInstance* create(const void* core);
};


template <class T, class BASE>
class Poly_PyInstanceFactory: public PyInstanceFactory {
	protected:
		std::vector<PyInstanceFactory*> subclasses;
	public:
		// given that core is of base class BASE, check if it is
		// also fof subclass T
		virtual bool check(const void* core);
		virtual PyCPPClassInstance* create(const void* core);
		virtual void addSubClass(PyInstanceFactory* subclass) {
			subclasses.push_back(subclass);
		}
};


#include "PyCPPClassType.h"
#include "CoreShell.h"


template <class T>
PyCPPClassInstance* NonPoly_PyInstanceFactory<T>::create(const void* core) {
	return PyCPPClassType<T>::create((T*)core);
}

template <class T, class BASE>
bool Poly_PyInstanceFactory<T, BASE>::check(const void* core) {
	return dynamic_cast<const T*>((const BASE*)core) != 0;
}

template <class T, class BASE>
PyCPPClassInstance*  Poly_PyInstanceFactory<T, BASE>::create(const void* core) {
	const CoreShell* pysubclass = dynamic_cast<const CoreShell*>((const T*)core);
	if (pysubclass) {
		Py_INCREF(pysubclass->pyobj);
		return pysubclass->pyobj;
	}

	std::vector<PyInstanceFactory*>::const_iterator it;
	for (it = subclasses.begin(); it != subclasses.end(); ++it) {
		if ((*it)->check(core)) {
			return (*it)->create(core);
		}
	}

	return PyCPPClassType<T>::create((T*)core);
}



#endif
