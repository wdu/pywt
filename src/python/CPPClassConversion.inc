// This may look like C code, but it's really -*- C++ -*-
/*
 * Copyright (C) 2013 Emweb bvba, Belgium.
 *
 * See the LICENSE file for terms of use.
 */



#include "CPPClassConversion.h"
#include "PyCPPClassInstance.h"
#include "PyCPPClassType.h"
#include "ImplicitConversion.h"
#include "PyInstanceFactory.h"

/***************** Pointer to CPPClass ***************/

template<class T>
T* Python2C_ptr<T>::arg(PyObject* e) { 
	if (e == Py_None) return 0; 
	if (!PyObject_IsInstance(e, (PyObject*)(PyCPPClassType<T>::Type))) {\
		PyErr_Format(PyExc_TypeError, 
				"Wrong argument type: got %s while expecting %s",  
				e->ob_type->tp_name,  PyCPPClassType<T>::Type->tp_name);
			return 0;
	} 
	PyCPPClassInstance* n = (PyCPPClassInstance*)e; 
	return (PyCPPClassType<T>::getCore(n)); 
} 

template<class T>
T* Python2C_ptr<T>::ret(PyObject* e) { 
	return arg(e); 
} 

template<class T>
bool Python2C_ptr<T>::acceptarg(PyObject* e) {
	return (e == Py_None || 
		PyObject_IsInstance(e, (PyObject*)(PyCPPClassType<T>::Type))); 
}


/***************** const Pointer to CPPClass ***************/


template<class T>
const T* Python2C_cptr<T>::arg(PyObject* e) { 
	return Python2C_ptr<T>::arg(e);
} 

template<class T>
const T* Python2C_cptr<T>::ret(PyObject* e) { 
	return arg(e); 
} 

template<class T>
bool Python2C_cptr<T>::acceptarg(PyObject* e) {
	return (e == Py_None || 
		PyObject_IsInstance(e, (PyObject*)(PyCPPClassType<T>::Type))); 
}

/***************** const ref to CPPClass ***************/
template<class T>
const T& Python2C_cref<T>::arg(PyObject* e, T** ptr) { 
	if (e == Py_None) { 
		PyErr_SetString(PyExc_ValueError, "Argument is 'None'"); 
		throw PyException(); 
	} 
	try { 
		T* r = Python2C_ptr<T>::arg(e); 
		if (r) return *r; 
	} 
	catch (PyException&) {} 
	const T& r = ImplicitConvert<T>::convert(e, ptr); 
	PyErr_Clear(); 
	return r; 
} 

template<class T>
bool Python2C_cref<T>::acceptarg(PyObject* e) {
	return (PyObject_IsInstance(e, (PyObject*)(PyCPPClassType<T>::Type))
			|| ImplicitConvert<T>::accepts(e)); 
}



/***************** ref to CPPClass ***************/
template<class T>
T& Python2C_ref<T>::arg(PyObject* e) { 
	if (e == Py_None) { 
		PyErr_SetString(PyExc_ValueError, "Argument is 'None'"); 
		throw PyException(); 
	} 
	T* r = Python2C_ptr<T>::arg(e); 
	return *r; 
} 

template<class T>
T& Python2C_ref<T>::ret(PyObject* e) { 
	return arg(e);
}


template<class T>
bool Python2C_ref<T>::acceptarg(PyObject* e) {
	return (PyObject_IsInstance(e, (PyObject*)(PyCPPClassType<T>::Type))
			|| ImplicitConvert<T>::accepts(e)); 
}


/***************** value of CPPClass ***************/
template<class T>
T Python2C_val<T>::arg(PyObject* e) { 
	T* r = Python2C_ptr<T>::arg(e); 
	if (!r && !PyErr_Occurred()) { 
		PyErr_SetString(PyExc_ValueError, "Argument is 'None'"); 
	} 
	if (PyErr_Occurred()) { 
		throw PyException(); 
	} 
	return *r; 
} 

template<class T>
T Python2C_val<T>::ret(PyObject* e) { 
	return arg(e);  
} 

template<class T>
bool Python2C_val<T>::acceptarg(PyObject* e) {
	return 0 != (PyObject_IsInstance(e, (PyObject*)(PyCPPClassType<T>::Type))); 
}



/***************** const value of CPPClass ***************/

template<class T>
const T Python2C_cval<T>::arg(PyObject* e) { 
	T* r = Python2C_ptr<T>::arg(e); 
	if (!r && !PyErr_Occurred()) { 
		PyErr_SetString(PyExc_ValueError, "Argument is 'None'"); 
	} 
	if (PyErr_Occurred()) { 
		throw PyException(); 
	} 
	return *r; 
} 

template<class T>
bool Python2C_cval<T>::acceptarg(PyObject* e) {
	return (PyObject_IsInstance(e, (PyObject*)(PyCPPClassType<T>::Type))); 
}





/*****************************************************/
/*****************************************************/
/*****************************************************/
/*****************************************************/




/***************** Pointer to CPPClass ***************/



template<class T>
PyObject* C2Python_ptr<T>::ret(const T* e) { 
	if (!e) Py_RETURN_NONE; 
	return PyCPPClassType<T>::Type->Factory->create(e); 
} 

template<class T>
PyObject* C2Python_ptr<T>::arg(const T* e) { 
	if (!e) Py_RETURN_NONE; 
	return PyCPPClassType<T>::Type->Factory->create(e); 
} 


/***************** const Pointer to CPPClass ***************/

template<class T>
PyObject* C2Python_cptr<T>::ret(const T* e) { 
	if (!e) Py_RETURN_NONE; 
	return PyCPPClassType<T>::Type->Factory->create(e); 
} 

template<class T>
PyObject* C2Python_cptr<T>::arg(const T* e) { 
	if (!e) Py_RETURN_NONE; 
	return PyCPPClassType<T>::Type->Factory->create(e); 
} 


/***************** value of CPPClass ***************/
// for arg, a reference can be used to the incoming variable,
// because we know that it will stay alive for the entire
// scope of the call to the python function
// for ret, however, we need to create a new value, which is
// handed over to the python


template<class T>
PyObject*  C2Python_val<T>::arg(const T& e) {
	PyCPPClassInstance* r = 
		static_cast<PyCPPClassInstance*>(C2Python_ptr<T>::ret(new T(e)));
	r->setPythonOwnership(); 
	return r;
} 

template<class T>
PyObject*  C2Python_val<T>::ret(const T& e) { 
	PyCPPClassInstance* r = 
		static_cast<PyCPPClassInstance*>(C2Python_ptr<T>::ret(new T(e)));
	r->setPythonOwnership(); 
	return r;
} 


/***************** const value of CPPClass ***************/
template<class T>
PyObject*  C2Python_cval<T>::arg(const T& e) {
	return C2Python_ptr<T>::arg(&e);
} 

template<class T>
PyObject*  C2Python_cval<T>::ret(const T& e) { 
	PyCPPClassInstance* r = 
		static_cast<PyCPPClassInstance*>(C2Python_ptr<T>::ret(new T(e)));
	r->setPythonOwnership(); 
	return r;
} 


/***************** const ref to CPPClass ***************/
template<class T>
PyObject*  C2Python_cref<T>::arg(const T& e) {
	return PyCPPClassType<T>::Type->Factory->create(&e); 
} 

template<class T>
PyObject*  C2Python_cref<T>::ret(const T& e) {
	return PyCPPClassType<T>::Type->Factory->create(&e); 
} 


/***************** ref to CPPClass ***************/
template<class T>
PyObject*  C2Python_ref<T>::arg(T& e) {
	return PyCPPClassType<T>::Type->Factory->create(&e); 
} 

template<class T>
PyObject*  C2Python_ref<T>::ret(T& e) {
	return PyCPPClassType<T>::Type->Factory->create(&e); 
} 




/*****************************************************/
/*****************************************************/
/*****************************************************/
/*****************************************************/

/***************** Enum ***************/
template<class T>
T Python2C_enum<T>::arg(PyObject* e) { 
	if (!PyObject_IsInstance(e, (PyObject*)(EnumType<T>::Type))) { 
		PyErr_Format(PyExc_TypeError, "Wrong argument type: got %s while expecting %s",  e->ob_type->tp_name,  EnumType<T>::Type->tp_name); 
		return (T)0;  
	} 
	Enum* me = (Enum*)e; 
	if (me->valuestr == 0) { 
		PyErr_Format(PyExc_TypeError, "Combination of %s flags is not allowed",  e->ob_type->tp_name); 
		return (T)0;  
	} 
	return (T)(me->value); 
} 

template<class T>
T Python2C_enum<T>::ret(PyObject* e) { 
	return arg(e); 
} 

// TODO: valuestr must be present: otherwise = flags
template<class T>
bool Python2C_enum<T>::acceptarg(PyObject* e) { 
	return 0 != PyObject_IsInstance(e, (PyObject*)(EnumType<T>::Type)); 
} 

/***************** Enum Flags***************/

template<class T>
Wt::WFlags<T>  Python2C_flags<T>::arg(PyObject* e) { 
	if (!PyObject_IsInstance(e, (PyObject*)(EnumType<T>::Type))) { 
		PyErr_Format(PyExc_TypeError, "Wrong argument type: got %s while expecting %s",  e->ob_type->tp_name,  EnumType<T>::Type->tp_name); 
		return (T)0;  
	} 
	Enum* me = (Enum*)e; 
	return  Wt::WFlags<T>((T)(me->value)); 
} 

template<class T>
Wt::WFlags<T>  Python2C_flags<T>::arg(PyObject* e, Wt::WFlags<T>** f) { 
	*f = new Wt::WFlags<T>(Python2C_flags<T>::arg(e));
	return **f;
}

template<class T>
Wt::WFlags<T> Python2C_flags<T>::ret(PyObject* e) { 
	return arg(e); 
} 

template<class T>
bool Python2C_flags<T>::acceptarg(PyObject* e) { 
	return 0 != PyObject_IsInstance(e, (PyObject*)(EnumType<T>::Type)); 
} 


template<class T>
PyObject* C2Python_enum<T>::ret(const T e) { 
	PyObject* r = EnumType<T>::find(e); 
	if (r) Py_INCREF(r); 
	return r; 
} 

template<class T>
PyObject*  C2Python_enum<T>::arg(const T e) { 
	return ret(e); 
}

template<class T>
PyObject* C2Python_flags<T>::ret(const Wt::WFlags<T> e) { 
	return EnumType<T>::create2((T)e); 
} 

template<class T>
PyObject* C2Python_flags<T>::arg(const Wt::WFlags<T> e) { 
	return ret(e);                      
}                                       






