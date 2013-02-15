// This may look like C code, but it's really -*- C++ -*-
/*
 * Copyright (C) 2013 Emweb bvba, Belgium.
 *
 * See the LICENSE file for terms of use.
 */

#ifndef PYCLASSTYPE
#define PYCLASSTYPE


#include <Python.h>
#include "PyCPPClassTypeBase.h"
#include "PyCPPClassInstance.h"
#include "Proxy.h"


template < class CORE > class PyCPPClassType:public PyCPPClassTypeBase {
	public:

	    static PyCPPClassType<CORE>* Type;

	public:
		PyCPPClassType (const char *name, const PyTypeObject** base = 0):
				PyCPPClassTypeBase(name, base) {
			tp_init = (initproc)s_init;
			Type = this;
		}

		inline PyCPPClassType<CORE>* getType() {return Type;}

		static CORE* getCore(PyCPPClassInstance* obj) {
			checkCoreValid(obj);
			return (CORE*)(obj->proxy->getCoreAsVoid());
		}


		static PyCPPClassInstance* create(const CORE* c) {
			PyCPPClassInstance* r = (PyCPPClassInstance*)s_new(PyCPPClassType<CORE>::Type, 0, 0);
			r->proxy = Proxy<CORE>::create(r, c, AbstractProxy::OWNED_BY_C);
			return r;
		}



	protected:



		static int s_init(PyCPPClassInstance *self, PyObject *args, PyObject *kwds) {
			return PyCPPClassTypeBase::s_init(self, args, kwds, PyCPPClassType<CORE>::Type);
		}


};

#define NONZERO(a) (a != 0)
#define ALWAYS(a) (true)

#include <methodobject.h>


class MethodDef: public PyMethodDef {
	public:
		MethodDef(const char* name, WrapperMethod f, const char* doc = 0) {
			ml_name = name;
			ml_meth = (PyCFunction)f; 
			ml_flags = METH_KEYWORDS; 
			ml_doc = doc;
		}
		MethodDef() {
			ml_name = 0;
			ml_meth = 0; 
			ml_flags = 0; 
			ml_doc = 0;
		}
};

class StaticMethodDef: public MethodDef {
	public:
		StaticMethodDef(const char* name, WrapperMethod f, const char* doc = 0) {
			ml_name = name;
			ml_meth = (PyCFunction)f; 
			ml_flags = METH_KEYWORDS | METH_STATIC; 
			ml_doc = doc;
		}
};

class MemberDef: public PyGetSetDef {
	public:
		MemberDef() {
			name = 0;
			get = 0;
			set = 0;
			doc = 0;
			closure = 0;
		}

		MemberDef(const char* aname, WrapperGetter aget, WrapperSetter aset, const char* adoc = 0) {
			name = (char*)aname;
			get = (getter)aget;
			set = (setter)aset;
			doc = (char*)adoc;
			closure = 0;
		}
};

/*
class ROMemberDef: public MemberDef {
	public:
		ROMemberDef(char* aname, WrapperGetter aget) {
			name = aname;
			get = (getter)aget;
			set = 0;
			doc = 0;
			closure = 0;
		}

};
*/


#endif
