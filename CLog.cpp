/*
 * Implementation of the CLog (Conditional Log) class. A quick and dirty class used to conditionally log.
 * This is based heavily off of a solution found online.
 */
 
#include "CLog.h"

bool CLog::isInitialized;
int  CLog::theLogLevel;

// Conditional logger. Replaces printf or std::cout. 
void CLog::write(int logLevel, const char *szFormat, ...) {
   checkInit(logLevel);
   if (logLevel >= theLogLevel) {
      va_list args;
      va_start(args, szFormat);
      vprintf(szFormat, args);
      va_end(args);
   }
}

// Sets the log level.
bool CLog::setLevel(int logLevel) {
   if (logLevel != CLog::ALL && logLevel != CLog::METRICS && logLevel != CLog::VERBOSE) {
      return false;
   }
   
   theLogLevel = logLevel;
   isInitialized = true;
   return true;
}

// Ensures the logging level is initialized before the first call.
void CLog::checkInit(int logLevel) {
   if (!isInitialized) {
      init(logLevel);
   }
}

// Initializes the log level.
void CLog::init(int logLevel) {
   if (!setLevel(logLevel)) {
      std::cout << "ERROR - failed to init conditional logger." << std::endl;
   }
}
