#ifndef EVENTS_H
#define EVENTS_H

#include <assert.h>

#include <iostream>

#include "connection_handle.h"

template < typename ReturnType >
class Callback0;

template < typename ReturnType, typename Argument1Type >
class Callback1;

template < typename ReturnType, typename Argument1Type, typename Argument2Type >
class Callback2;

template < typename ReturnType, typename Argument1Type, typename Argument2Type, typename Argument3Type, typename Argument4Type >
class Callback4;

#include "../list.h"
#include "events.inc"

#endif
