// This may look like C code, but it's really -*- C++ -*-
/*
 * Copyright (C) 2013 Emweb bvba, Belgium.
 *
 * See the LICENSE file for terms of use.
 */


#ifndef ENUM_H
#define ENUM_H

#include <Python.h>
#include "structmember.h"
#include <string>
#include <map>

static const char* hexvals[] = {
	"0x01",
	"0x02",
	"0x04",
	"0x08",
	"0x10",
	"0x20",
	"0x40",
	"0x80",
	"0x0100",
	"0x0200",
	"0x0400",
	"0x0800",
	"0x1000",
	"0x2000",
	"0x4000",
	"0x8000",
	"0x010000",
	"0x020000",
	"0x040000",
	"0x080000",
	"0x100000",
	"0x200000",
	"0x400000",
	"0x800000",
	"0x01000000",
	"0x02000000",
	"0x04000000",
	"0x08000000",
	"0x10000000",
	"0x20000000",
	"0x40000000",
	"0x80000000"
};

/*************** Object representing a python Enum of type T ***********/
class Enum: public PyObject {
	public:
		const char* valuestr;
		unsigned int value;
};


/*************** Conversion to int and logical operators ***********/
inline unsigned int Enum_GetIntValue(PyObject* obj) {
	Enum* en = (Enum*)obj;
	return en->value;
}


static PyObject* Enum_Or(PyObject* o1, PyObject* o2);
static PyObject* Enum_And(PyObject* o1, PyObject* o2);
static PyObject* Enum_Not(PyObject* o1);
static PyObject* Enum_ToInt(PyObject* o1);
static PyObject* Enum_Compare(PyObject *a, PyObject *b, int op);


/*************** Python base Type for all Enums ***********/
// No Objects of this type will exist. It is only use for
// type checking, and as an initialization struct for the real
// Enum<T> types

extern PyTypeObject EnumTypeBase;
extern PyTypeObject EnumInit;
extern PyNumberMethods Enum_AsNumber;


/*************** Python type for a C enum T ***********/
// For each value of the enumerated type, a single instance
// is created. These instances are stored in the values map
// conversions from C to python are done by returning the
// corresponding python object from the map
template <class T>
class EnumType: public PyTypeObject {
	protected:
		std::map<T, Enum*> values; 

		static void dealloc(Enum* self) {
			self->ob_type->tp_free((PyObject*)self);
		}

		static PyObject* repr(PyObject* self) {
			Enum* me = (Enum*)self;
			if (me->valuestr) {
				return PyString_FromString(me->valuestr);
			}
			else if (me->value == 0) {
				return PyString_FromString("Zero Flags");
			}
			else {
				std::string s = "(";
				bool first = true;
				for (int i = 0; i<32; ++i) {
					unsigned int mask = 1 << i;
					if ((me->value & mask) == mask) {
						if (!first) {
							s += " | ";
						}
						typename std::map<T, Enum*>::const_iterator it = Type->values.find((T)mask);
						if (it == Type->values.end()) {
							s += hexvals[i];
						}
						else {
							s += it->second->valuestr;
						}
						first = false;
					}
				}
				s += ")";
				return PyString_FromString(s.c_str());
			}
		}

	public:
		static EnumType<T>* Type;

		// Constructor
		EnumType(const char* name, const char* doc = 0) {
			*((PyTypeObject*)this) = EnumInit;
			tp_basicsize = sizeof(Enum);
			tp_dealloc = (destructor)dealloc;
			tp_repr = repr;
			tp_name = name;
			tp_doc = doc;
			tp_as_number = &Enum_AsNumber;
			tp_flags = Py_TPFLAGS_DEFAULT;
			//assert(PyType_Ready(&EnumTypeBase) >= 0);
			//tp_base = &EnumTypeBase;
			PyType_Ready((PyTypeObject*)this);
			Type = this;
		}

		// Create (and add) a new value to this type
		static PyObject* create(const char* valuestr, const T& value, const char* doc = 0) {
			Enum* self = (Enum*)Type->tp_alloc(Type, 0);
			self->value = value;
			self->valuestr = valuestr;
			Type->values[value] = self;
			return (PyObject*) self;
		}

		// Create a new value of this type
		static PyObject* create2(const T& value) {
			Enum* self = (Enum*)Type->tp_alloc(Type, 0);
			self->value = value;
			self->valuestr = 0;
			return (PyObject*) self;
		}

		// Find the python instance representing value
		static PyObject* find(T value) {
			typename std::map<T, Enum*>::const_iterator it = Type->values.find(value);
			if (it == Type->values.end()) {
				PyErr_SetString(PyExc_ValueError, "Unknown Enum Value");
				return 0;
			}
			return it->second;
		}


};

template <class T>
EnumType<T>* EnumType<T>::Type = 0;


/*************** Check that a python object is an enum ***********/

bool Enum_Check(PyObject* o);

#endif
