/*
 * stdhdr.h
 * Joel Wolfrath 2014
 *
 * Standard GL header includes
 */

#ifndef STD_HEADER
#define STD_HEADER

#if defined(WIN32)
#include <windows.h>
#endif

#if defined(__APPLE__)
#include <OpenGl/gl.h>
#include <OpenGl/glu.h>
#include <GLUT/glut.h>
#else
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include <GL/glx.h>
#endif

#endif /* STD_HEADER */
