// This may look like C code, but it's really -*- C++ -*-
/*
 * Copyright (C) 2013 Emweb bvba, Belgium.
 *
 * See the LICENSE file for terms of use.
 */

#include <istream>
template <>
PyCPPClassType< std::istream >* PyCPPClassType< std::istream >::Type = 0;


PyStdIStream::PyStdIStream(): PyCPPClassType< std::istream >(MODPREFIX "StdIStream") {
			Type->Factory = &theFactory;
			// Inherited methods
};

