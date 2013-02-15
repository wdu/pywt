// This may look like C code, but it's really -*- C++ -*-
/*
 * Copyright (C) 2013 Emweb bvba, Belgium.
 *
 * See the LICENSE file for terms of use.
 */


#ifndef WT_PROXY_H
#define WT_PROXY_H


#include "Proxy.h"
#include "gwt_ProxyTypes.h"

#include <Wt/WSignal>
template <class A1, class A2, class A3, class A4, class A5, class A6>
class Proxy<Wt::JSignal<A1, A2, A3, A4, A5, A6> >: public ProxyBase<Wt::JSignal<A1, A2, A3, A4, A5, A6> > {};

template <class A1, class A2, class A3, class A4, class A5, class A6>
class Proxy<Wt::Signal<A1, A2, A3, A4, A5, A6> >: public ProxyBase<Wt::Signal<A1, A2, A3, A4, A5, A6> > {};

template <class A1>
class Proxy<Wt::EventSignal<A1> >: public ProxyBase<Wt::EventSignal<A1> > {};




/*
#include <istream>
#include <ostream>
template <>
class Proxy<std::ostream>: public ProxyBase<std::ostream> {};

template <>
class Proxy<std::istream>: public ProxyBase<std::istream> {};
*/


#endif
