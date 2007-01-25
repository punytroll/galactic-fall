#ifndef BUFFER_READING_H
#define BUFFER_READING_H

#include <BufferReader.h>

#include <math3d/vector2f.h>

#include "color.h"

Arxx::BufferReader & operator>>(Arxx::BufferReader & BufferReader, Color & Color);
Arxx::BufferReader & operator>>(Arxx::BufferReader & BufferReader, math3d::vector2f & Vector);

#endif
