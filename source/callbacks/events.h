#ifndef EVENTS_H
#define EVENTS_H

#include <assert.h>

#include <iostream>

#include "connection_handle.h"

template < typename ReturnType >
class Callback0;

template < typename ReturnType, typename Argument1Type >
class Callback1;

#include "../list.h"
#include "events.inc"

#endif
