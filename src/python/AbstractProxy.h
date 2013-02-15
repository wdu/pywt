// This may look like C code, but it's really -*- C++ -*-
/*
 * Copyright (C) 2013 Emweb bvba, Belgium.
 *
 * See the LICENSE file for terms of use.
 */


#ifndef ABSTRACTPROXY_H
#define ABSTRACTPROXY_H


class AbstractProxy {
	public:
		static const int OWNED_BY_C = 0;
		static const int OWNED_BY_PYTHON = 1;

		static const int SUBCLASS_INSTANCE = 2;
		static const int DEAD = 4;

	public:
		virtual ~AbstractProxy() {}
		virtual void deleteCore() = 0;
		virtual bool isDead() const = 0;
		virtual bool isOwnedByPython() const = 0;
		virtual bool isSubClassInstance() const = 0;
		virtual void setPythonOwnership(bool v) = 0;
		virtual void deletedByC() = 0;
		virtual void* getCoreAsVoid() const = 0;


};

#endif
