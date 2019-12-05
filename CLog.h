/*
 * Declaration of the CLog (Conditional Log) class. A quick and dirty class used to conditionally log.
 * This is based heavily off of a solution found online.
 */

#ifndef __CLOG__
#define __CLOG__

#include <cstdio>
#include <cstdarg>

#include "helpers.h"

class CLog {
   public:
      enum { ALL = 0, VERBOSE, METRICS };
      static void write(int logLevel, const char *szFormat, ...);
      static bool setLevel(int logLevel);

   protected:
      static void checkInit(int logLevel);
      static void init(int logLevel);

   private:
      CLog();
      static bool isInitialized;
      static int  theLogLevel;
};

#endif   // __CLOG__
