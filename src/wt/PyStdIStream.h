// This may look like C code, but it's really -*- C++ -*-
/*
 * Copyright (C) 2013 Emweb bvba, Belgium.
 *
 * See the LICENSE file for terms of use.
 */

#ifndef PYSTDISTREAM
#define PYSTDISTREAM


#include <istream>
#include "PyCPPClassType.h"
#include "PyInstanceFactory.h"


class PyStdIStream: public PyCPPClassType< std::istream > {
	public:
		NonPoly_PyInstanceFactory< std::istream > theFactory;
		PyStdIStream();
};








#endif


