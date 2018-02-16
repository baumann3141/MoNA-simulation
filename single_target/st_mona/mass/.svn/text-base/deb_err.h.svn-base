#ifndef ERR_H
#define ERR_H

/** \file
    Some macros that are useful for debuging are defined here.  They all
    print the file name, line number and the function name, where they were called.
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#ifdef __cplusplus
#include <iostream>
#endif

/** This macro prints a system error message (including the errno number, error string,
    file name, line number, and function name).
    The arguments format and args are to be used as in printf.
    The macro calls exit(-1).
    @see ERR, WAR, DEB
    @param format format like in printf
    @param args argument list according to format (link in printf)
    @return There is no return value.  This is only a macro.
 */
#define SERR(format, args...)  do { \
  fprintf (stderr, "SE %s:%-4d (%s) :: " \
           format "\n_ The error is (%d): %s\n_ exiting (-1)\n", \
           __FILE__, __LINE__ , __func__ , ## args, \
           errno, strerror(errno)); \
           abort(); \
                                      } while (0)
                                      

/** This macro prints an error message including file name, line number, and function name.
    The arguments format and args are to be used as in printf.
    The macro calls exit(-1). */
#define ERR(format, args...)  do { \
  fprintf (stderr, "E  %s:%-4d (%s) :: " \
           format "\nExiting (-1)\n", \
           __FILE__, __LINE__ , __func__ , ## args); \
           abort(); } while (0)

/** This macro prints a warning including file name, line number, and function name.
    The arguments format and args are to be used as in printf. */
#define WAR(format, args...)  do { \
   fprintf (stderr, "W  %s:%-4d (%s) :: " \
            format "\n", __FILE__, __LINE__ , __func__ , ## args); fflush(0); } while (0)

/** This macro prints a system error message (including the errno number, error string,
    file name, line number, and function name).
    The arguments format and args are to be used as in printf.
    @see ERR, WAR, DEB
    @param format format like in printf
    @param args argument list according to format (link in printf)
    @return There is no return value.  This is only a macro.
 */
#define SWAR(format, args...)  do { \
   fprintf (stderr, "SW %s:%-4d (%s) :: " \
            format "\n_ The error is (%d): %s\n", \
            __FILE__, __LINE__ , __func__ , ## args, \
            errno, strerror(errno)); fflush(0); } while (0)

/** This macro prints an info message including file name, line number, and function name.
    The arguments format and args are to be used as in printf. 
  */
#define INFO(format, args...)  do { \
  fprintf (stderr, "I  %s:%-4d (%s) :: " \
           format "\n", __FILE__, __LINE__ , __func__ , ## args); fflush(0); } while (0)


#ifdef DEBUG
/** This macro prints a debug message including file name, line number, and function name.
    The arguments format and args are to be used as in printf. 
    The DEBUG macro must be defined during compilation
  */
#define DEB(format, args...)  do { \
  fprintf (stderr, "D  %s:%-4d (%s) :: " \
           format "\n", __FILE__, __LINE__ , __func__ , ## args); fflush(0); } while (0)


#ifdef __cplusplus
/** This macro is a quick way to print the value of a variable.
    DSV = DEBUG Show Variable  (works only for C++).
    The DEBUG macro must be defined during compilation
 */
#define DSV(var)  do { \
    fprintf (stderr, "DV %s:%-4d (%s) :: ", __FILE__, __LINE__ , __func__ ); \
             std::cerr << #var " = " << var << std::endl; fflush(0); } while (0)
#else
#define DSV(var) 
#endif

#else

//@{
/// DEBug message and Debug Show Var are undefinded unless DEBUG is defined
#define DEB(args...)
#define DSV(var) 
//@}

#endif

#endif
