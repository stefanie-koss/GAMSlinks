// Copyright (C) GAMS Development 2008
// All Rights Reserved.
// This code is published under the Common Public License.
//
// $Id$
//
// Author: Stefan Vigerske

#include "GamsOptions.hpp"

#ifdef HAVE_CSTDLIB
#include <cstdlib>
#else
#ifdef HAVE_STDLIB_H
#include <stdlib.h>
#else
#error "don't have header file for stdlib"
#endif
#endif
#ifdef HAVE_CSTDIO
#include <cstdio>
#else
#ifdef HAVE_STDIO_H
#include <stdio.h>
#else
#error "don't have header file for stdio"
#endif
#endif
#ifdef HAVE_CASSERT
#include <cassert>
#else
#ifdef HAVE_ASSERT_H
#include <assert.h>
#else
#error "don't have header file for assert"
#endif
#endif

extern "C" {
#include "optcc.h"
}

#if defined(_WIN32)
# define snprintf _snprintf
#endif

//TODO: have to find a nicer way to get the output either through GamsModel or smag
void PrintOut(int, const char* msg) {
	fprintf(stderr, msg);
	fprintf(stderr, "\n");
}
#define AllMask 1

GamsOptions::GamsOptions(const char* systemdir, const char* solvername)
: optionshandle(NULL)
{
	/* Get the Option File Handling set up */
	char buffer[255];
	if (!optCreate(&optionshandle, buffer, 255)) {
		PrintOut(AllMask, "\n*** Could not create optionfile handle: "); 
		PrintOut(AllMask, buffer); 
		PrintOut(AllMask, "\n"); 
		exit(EXIT_FAILURE);
	}

	if (snprintf(buffer, 255, "%sopt%s.def", systemdir, solvername)>=255) {
		PrintOut(AllMask, "\n*** Path to GAMS system directory too long.\n");
		exit(EXIT_FAILURE);
	}
	
	if (optReadDefinition(optionshandle,buffer)) {
		int itype; 
		for (int i=1; i<=optMessageCount(optionshandle); ++i) {
			optGetMessage(optionshandle, i, buffer, &itype);
			PrintOut(AllMask, buffer);
		}
		optClearMessages(optionshandle);
		exit(EXIT_FAILURE);
	}
	optEOLOnlySet(optionshandle,1);
}

GamsOptions::~GamsOptions() {
	if (optionshandle) optFree(&optionshandle);
}

bool GamsOptions::readOptionsFile(const char* optfilename) {
	assert(optionshandle!=NULL);
	if (optfilename==NULL) return true;
	/* Read option file */
  optEchoSet(optionshandle, 1);
  optReadParameterFile(optionshandle, optfilename);
  if (optMessageCount(optionshandle)) {
	  char buffer[255];
	  int itype;
	  for (int i=1; i<=optMessageCount(optionshandle); ++i) {
  	  optGetMessage(optionshandle, i, buffer, &itype);
    	if (itype<=optMsgFileLeave || itype==optMsgUserError)
    		PrintOut(AllMask, buffer);
	  }
	  optClearMessages(optionshandle);
		optEchoSet(optionshandle, 0);
	  return false;
	} else {
		optEchoSet(optionshandle, 0);
		return true;
	}
}

bool GamsOptions::isDefined(const char *optname) {
	assert(optionshandle!=NULL);
  int i, refNum, isDefined, isDefinedRecent, dataType, optType, subType;

  if (optFindStr(optionshandle, optname, &i, &refNum)) {
    optGetInfoNr(optionshandle, i, &isDefined, &isDefinedRecent, &refNum, &dataType, &optType, &subType);
    return isDefined;
  } else {
  	char buffer[255];
  	snprintf(buffer, 255, "*** Internal Error. Unknown option %s", optname);
		PrintOut(AllMask, buffer);
    return false;
  }
}

//bool GamsModel::optDefinedRecent(const char *optname) {
//	if (NULL==optionshandle) {
//		PrintOut(AllMask, "GamsModel::optDefinedRecent: Optionfile handle not initialized.");
//		return false;
//	}
//  int i, refNum, isDefined, isDefinedRecent, dataType, optType, subType;
//
//  if (optFindStr (optionshandle, optname, &i, &refNum)) {
//    optGetInfoNr (optionshandle, i, &isDefined, &isDefinedRecent, &refNum, &dataType, &optType, &subType);
//    return isDefinedRecent;
//  } else {
//  	char buffer[255];
//  	snprintf(buffer, 255, "*** Internal Error. Unknown option %s", optname);
//		PrintOut(AllMask, buffer);
//    return false;
//  }
//}

int GamsOptions::getInteger(const char *optname) {
	assert(optionshandle!=NULL);
  int i, refNum, isDefined, isDefinedRecent, dataType, optType, subType, ival;
  double dval;
  char sval[255], oname[255];

  if (optFindStr(optionshandle, optname, &i, &refNum)) {
    optGetInfoNr(optionshandle, i, &isDefined, &isDefinedRecent, &refNum, &dataType, &optType, &subType);
    if (dataType != optDataInteger) {
    	snprintf(sval, 255, "*** Internal Error. Option %s is not an integer (it is %d)", optname, dataType);
    	PrintOut(AllMask, sval);
      return 0;
    }
    optGetValuesNr(optionshandle, i, oname, &ival, &dval, sval);
    return ival;
  } 

 	snprintf(sval, 255, "*** Internal Error. Unknown option %s", optname);
	PrintOut(AllMask, sval);
	return 0;
}

double GamsOptions::getDouble(const char *optname) {
	assert(optionshandle!=NULL);
  int i, refNum, isDefined, isDefinedRecent, dataType, optType, subType, ival;
  double dval;
  char sval[255], oname[255];

  if (optFindStr(optionshandle, optname, &i, &refNum)) {
    optGetInfoNr(optionshandle, i, &isDefined, &isDefinedRecent, &refNum, &dataType, &optType, &subType);
    if (dataType != optDataDouble) {
    	snprintf(sval, 255, "*** Internal Error. Option %s is not a double (it is %d)", optname, dataType);
    	PrintOut(AllMask, sval);
      return 0.;
    }
    optGetValuesNr(optionshandle, i, oname, &ival, &dval, sval);
    return dval;
  }

 	snprintf(sval, 255, "*** Internal Error. Unknown option %s", optname);
	PrintOut(AllMask, sval);
	return 0.;
}

char* GamsOptions::getString(const char *optname, char *sval) {
	assert(optionshandle!=NULL);
  int i, refNum, isDefined, isDefinedRecent, dataType, optType, subType, ival;
  double dval;
  char oname[255];

  if (optFindStr(optionshandle, optname, &i, &refNum)) {
    optGetInfoNr(optionshandle, i, &isDefined, &isDefinedRecent, &refNum, &dataType, &optType, &subType);
    if (dataType != optDataString) {
    	snprintf(oname, 255, "*** Internal Error. Option %s is not a string (it is %d)", optname, dataType);
 	  	PrintOut(AllMask, oname);
   	  return sval;
    }
    optGetValuesNr(optionshandle, i, oname, &ival, &dval, sval);
    return sval;
  }

 	snprintf(oname, 255, "*** Internal Error. Unknown option %s", optname);
	PrintOut(AllMask, oname);
	return sval;
}

void GamsOptions::setInteger(const char *optname, int ival) {
	assert(optionshandle!=NULL);
  int i, k, refNum, isDefined, isDefinedRecent, dataType, optType, subType;
  char stmp[255];

  if (optFindStr(optionshandle, optname, &i, &refNum)) {
    optGetInfoNr(optionshandle, i, &isDefined, &isDefinedRecent, &refNum, &dataType, &optType, &subType);
    if (dataType != optDataInteger) {
			snprintf(stmp, 255, "*** Internal Error. Option %s is not an integer (it is %d)\n", optname, dataType);
			PrintOut(AllMask, stmp);
      return;
    }
    optSetValuesNr(optionshandle, i, ival, 0.0, "");
    
    if (optMessageCount(optionshandle)) {
    	char buffer[255];
	    for (i=1; i<=optMessageCount(optionshandle); i++) {
	      optGetMessage(optionshandle, i, stmp, &k);
	      if (k==optMsgUserError || k<optMsgFileEnter) {
					snprintf(buffer, 255, "%d: %s\n", k, stmp);
					PrintOut(AllMask, buffer);
	      }
			}
	    optClearMessages(optionshandle);
		}
		
		return;
  }
  
 	snprintf(stmp, 255, "*** Internal Error. Unknown option %s", optname);
	PrintOut(AllMask, stmp);
}

void GamsOptions::setDouble(const char *optname, double dval) {
	assert(optionshandle!=NULL);
  int i, k, refNum, isDefined, isDefinedRecent, dataType, optType, subType;
  char stmp[255];

  if (optFindStr(optionshandle, optname, &i, &refNum)) {
    optGetInfoNr(optionshandle, i, &isDefined, &isDefinedRecent, &refNum, &dataType, &optType, &subType);
    if (dataType != optDataDouble) {
			snprintf(stmp, 255, "*** Internal Error. Option %s is not a double (it is %d)\n", optname, dataType);
			PrintOut(AllMask, stmp);
      return;
    }
    optSetValuesNr(optionshandle, i, 0, dval, "");
    
    if (optMessageCount(optionshandle)) {
    	char buffer[255];
	    for (i=1; i<=optMessageCount(optionshandle); i++) {
	      optGetMessage(optionshandle, i, stmp, &k);
	      if (k==optMsgUserError || k<optMsgFileEnter) {
					snprintf(buffer, 255, "%d: %s\n", k, stmp);
					PrintOut(AllMask, buffer);
	      }
			}
	    optClearMessages(optionshandle);
		}
		
		return;
  }
  
 	snprintf(stmp, 255, "*** Internal Error. Unknown option %s", optname);
	PrintOut(AllMask, stmp);
}

void GamsOptions::setString(const char *optname, const char *sval) {
	assert(optionshandle!=NULL);
  int i, k, refNum, isDefined, isDefinedRecent, dataType, optType, subType;
  char stmp[255];

  if (optFindStr(optionshandle, optname, &i, &refNum)) {
    optGetInfoNr(optionshandle, i, &isDefined, &isDefinedRecent, &refNum, &dataType, &optType, &subType);
    if (dataType != optDataString) {
			snprintf(stmp, 255, "*** Internal Error. Option %s is not a string (it is %d)\n", optname, dataType);
			PrintOut(AllMask, stmp);
      return;
    }
    optSetValuesNr(optionshandle, i, 0, 0.0, sval);

    if (optMessageCount(optionshandle)) {
    	char buffer[255];
	    for (i=1; i<=optMessageCount(optionshandle); i++) {
	      optGetMessage(optionshandle, i, stmp, &k);
	      if (k==optMsgUserError || k<optMsgFileEnter) {
					snprintf(buffer, 255, "%d: %s\n", k, stmp);
					PrintOut(AllMask, buffer);
	      }
			}
	    optClearMessages(optionshandle);
		}
		
		return;
  }
  
 	snprintf(stmp, 255, "*** Internal Error. Unknown option %s", optname);
	PrintOut(AllMask, stmp);
}