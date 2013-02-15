// This may look like C code, but it's really -*- C++ -*-
/*
 * Copyright (C) 2013 Emweb bvba, Belgium.
 *
 * See the LICENSE file for terms of use.
 */

#ifndef PYCLASSTYPEBASE
#define PYCLASSTYPEBASE


#include <Python.h>
//class PyCPPMethodIDBase;
class PyCPPConstructorWrapper;
class PyInstanceFactory;
class AbstractProxy;
class PyCPPClassInstance;


typedef void(*ConstructorMethod)(PyCPPClassInstance *, PyObject *,
		                         PyObject *, int);
typedef PyObject *(*WrapperMethod)(PyCPPClassInstance *, PyObject *,
		                         PyObject *);
typedef PyObject *(*WrapperGetter)(PyCPPClassInstance *, void *);
typedef int (*WrapperSetter)(PyCPPClassInstance *, PyObject*, void *);

typedef void(*CoreDestructor)(PyCPPClassInstance *);


class MethodDef;
class MemberDef;



class PyCPPClassTypeBase: public PyTypeObject {
	protected:
		PyCPPClassTypeBase(const char *name, const PyTypeObject** base);
		static PyObject *s_new(PyTypeObject *type, PyObject *args, PyObject *kwds);
		static long s_hash(PyObject *object);

		static int s_init(PyCPPClassInstance *self, PyObject* args, PyObject* kwds, PyCPPClassTypeBase *mytype);
		static void s_dealloc (PyCPPClassInstance * self);
			 

		void addStaticMember(const char* name, PyObject* value) {
			PyDict_SetItemString(tp_dict, name, value);
		}
		void addInnerClass(const char* name, PyTypeObject* value) {
			PyType_Ready(value);
			PyDict_SetItemString(tp_dict, name, (PyObject*)value);
		}
	public:
		ConstructorMethod baseconstructor;
		CoreDestructor deleteCore;
		PyInstanceFactory* Factory;
		bool instanciatable;

		inline void setBaseConstructorPtr(ConstructorMethod bc) {
			setConstructorPtr(bc);
			makeSubclassable();
		}
		
		inline void setConstructorPtr(ConstructorMethod bc) {
			baseconstructor = bc;
			tp_new = &PyCPPClassTypeBase::s_new;
		}

		

		inline void makeInstanciatable() {
			instanciatable = true;
		}

		inline void makeSubclassable() {
			tp_flags |= Py_TPFLAGS_BASETYPE;
		}


		void setMethods(MethodDef* m) {
			tp_methods = (PyMethodDef*)m;
		}
		void setMembers(MemberDef* m) {
			tp_getset = (PyGetSetDef*)m;
		}

		static const void* checkCore(PyCPPClassInstance* obj);

		static void checkCoreValid(PyCPPClassInstance* obj);



};

#endif
