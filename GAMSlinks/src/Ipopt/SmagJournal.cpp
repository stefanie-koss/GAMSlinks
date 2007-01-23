// Copyright (C) GAMS Development 2007
// All Rights Reserved.
// This code is published under the Common Public License.
//
// $Id$
//
// Author: Stefan Vigerske

#include "SmagJournal.hpp"

#if defined(_WIN32)
# define vsnprintf _vsnprintf
# define  snprintf  _snprintf
#endif

#if defined(NOVSNPRINTF)
# define VSNPRINTF fakevsnprintf
static char        hugeBufVSN[4096];
static int
fakevsnprintf(char *str, size_t size, const char *format, va_list ap) {
  /* this better not overflow! */
	int rc = vsprintf (hugeBufVSN, format, ap);
	int n = rc;
	if (n<size-1) n=size-1;
  if (n >= 0) {
    strncpy (str, hugeBufVSN, n);
    str[n] = '\0';
  }
  return rc;
} /* fakevsnprintf */
#else
# define VSNPRINTF vsnprintf
#endif


void SmagJournal::PrintfImpl(const char* pformat, va_list ap) {
  char outBuf[1024];

#ifdef HAVE_VA_COPY
	va_list apcopy;
	va_copy(apcopy, ap);
  VSNPRINTF(outBuf, sizeof(outBuf), pformat, apcopy);
	va_end(apcopy);
#else
  VSNPRINTF(outBuf, sizeof(outBuf), pformat, ap);
#endif

	PrintImpl(outBuf);
}

void SmagJournal::PrintImpl(const char* str) {
	//TODO: this is horrible  
  char* lastchar = const_cast<char*>(str) + strlen(str); --lastchar;
  if (*lastchar == '\n') {
    *lastchar = 0; lastchar--;
  }
	if (*str) smagStdOutputPrint(smag, smag_mask, str);
}
