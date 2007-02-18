// Copyright (C) GAMS Development 2007
// All Rights Reserved.
// This code is published under the Common Public License.
//
// $Id$
//
// Authors: Stefan Vigerske

#include "SmagMessageHandler.hpp"

#ifdef HAVE_CSTDIO
#include <cstdio>
#else
#ifdef HAVE_STDIO_H
#include <stdio.h>
#else
#error "don't have header file for stdio"
#endif
#endif

SmagMessageHandler::SmagMessageHandler(smagHandle_t smag_)
: smag(smag_) 
{ }

// Print message, return 0 normally
int SmagMessageHandler::print() {
  if (0 == smag)
    printf("%s", messageBuffer());
  else {
  	int smag_mask = currentMessage().detail() < 2 ? SMAG_ALLMASK : SMAG_LOGMASK; 
  	smagStdOutputPrintX(smag, smag_mask, messageBuffer(), 0);
//  	smagStdOutputFlush(smag, smag_mask);
  }
  return 0;
}
