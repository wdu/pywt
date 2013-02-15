// This may look like C code, but it's really -*- C++ -*-
/*
 * Copyright (C) 2013 Emweb bvba, Belgium.
 *
 * See the LICENSE file for terms of use.
 */


#ifndef RES_H
#define RES_H

/*
This class enables to write the call to underlying c++ methods in a 
separate scope:
A typical construction is like:


 1 release python lock
 2 ResultType r;
 3 try {
 4 	r = somecall(args)
 5 }
 6 catch (anexception& e) {
 7 	acquire python lock
 8 	convert exception to python error
 9 }
10 acquire python lock
11 return convert_to_pythonobject(r);


This code does not compile if 
- ResultType is a reference (line 2, missing initialization),
- ResultType is const (line 4, assignment to a const)
- ResultType is a class with no default constructor (line 2, no initialization)
(Note that this code is automatically generated, the tool is unaware of types)

The solution is to have a template type which encapsulates r.
- for values a new object is allocated on the heap, initialized using the copy constructor
in order to avoid problems with missing default constructors
- for references, the address of the object is stored
- for pointersi, the pointer itself is stored.
- for consts, the const is casted away to allow the assignment


*/


template <class T>
class Res {
	protected:
		T* _v;
	public:
		Res(): _v(0) {}
		~Res() {delete _v;}

		// there is only one assignment, so there is no check for
		// a previous value to be deleted
		void operator=(const T& val) {_v = new T(val);}
		operator T() const {return *_v;}
};



template <class T>
class Res<T*> {
	protected:
	       	T* _v;
	public:
		Res(){}
		void operator=(T* val) {_v = val;}
		operator T*() const {return _v;}

};

template <class T>
class Res<const T> {
	protected:
	       	T _v;
	public:
		Res(){}
		void operator=(const T& val) {_v = val;}
		operator const T() const {return _v;}

};

template <class T>
class Res<T&> {
	protected:
	       	T* _v;
	public:
		Res(){}
		void operator=(T& val) {_v = &val;}
		operator T&() const {return *_v;}

};
#endif
