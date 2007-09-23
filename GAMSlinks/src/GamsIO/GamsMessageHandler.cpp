// Copyright (C) GAMS Development 2006
// All Rights Reserved.
// This code is published under the Common Public License.
//
// $Id$
//
// Authors: Michael Bussieck, Stefan Vigerske

#if defined(_MSC_VER)
// Turn off compiler warning about long names
#  pragma warning(disable:4786)
#endif

#include "GamsMessageHandler.hpp"

#ifdef HAVE_CSTDIO
#include <cstdio>
#else
#ifdef HAVE_STDIO_H
#include <stdio.h>
#else
#error "don't have header file for stdio"
#endif
#endif
#ifdef HAVE_CSTRING
#include <cstring>
#else
#ifdef HAVE_STRING_H
#include <string.h>
#else
#error "don't have header file for string"
#endif
#endif

GamsMessageHandler::GamsMessageHandler(GamsModel *GMptr)
: GMptr_(GMptr), rmlblanks_(1) 
{ }

void GamsMessageHandler::setCurrentDetail(int detail) {
	currentMessage_.setDetail(detail);
}

int GamsMessageHandler::getCurrentDetail() const {
	return currentMessage_.detail();
}

// Print message, return 0 normally
int GamsMessageHandler::print() {
  char *messageOut = const_cast<char*>(messageBuffer());
  char *lastchar;
  int i=rmlblanks_;

  // white space at the beginning
  while (i-- > 0 && *messageOut == ' ') 
      messageOut++;
	//TODO: maybe copy the string before altering it?  
  lastchar = messageOut + strlen(messageOut); lastchar--;
  while (*lastchar == '\n') {
    *lastchar = 0; lastchar--;
  }
  if (0 == GMptr_)
    printf("%s\n", messageOut);
  else {
    if (currentMessage_.detail() < 2)
      GMptr_->PrintOut(GamsModel::AllMask, messageOut);
    else
      GMptr_->PrintOut(GamsModel::LogMask, messageOut);
  }
  return 0;
}
