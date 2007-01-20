// Copyright (C) GAMS Development 2007
// All Rights Reserved.
// This code is published under the Common Public License.
//
// $Id$
//
// Author: Stefan Vigerske
// from the IpFileJournal class in IPOPT


#ifndef __SMAGJOURNALIST_HPP__
#define __SMAGJOURNALIST_HPP__

#include "GAMSlinksConfig.h"
#include "smag.h"
#include "IpJournalist.hpp"

//#include <cstdio>

using namespace Ipopt;

//#include <string>

/** SmagJournal class. This is a particular Journal implementation that
 *  uses the SMAG routines for output.
 */
class SmagJournal : public Journal {
public:
  /** Constructor. */
  SmagJournal(smagHandle_t smag_, EJournalLevel default_level)
  : Journal("SmagJournal", default_level), smag(smag_)
  { }

  /** Destructor. */
  ~SmagJournal() { }

protected:
  /**@name Implementation version of Print methods - Overloaded from Journal base class.
   */
  //@{
  /** Print to the designated output location */
  virtual void PrintImpl(const char* str) {
		if (str) smagPrint(smag, SMAG_LOGMASK, "%s", str);
  }

  /** Printf to the designated output location */
  virtual void PrintfImpl(const char* pformat, va_list ap) {
	/**	The following does not work.
	 * I get a segmentation fault in strlen() from inside the VSNPRINTF call at line 5 of smagPrint.
	 * Also the output coming before looks scrappy.
	 * if (pformat) smagPrint(smag, SMAG_LOGMASK, pformat, ap);
	 */
		if (smag->fpLog) vfprintf(smag->fpLog, pformat, ap);
	}

  /** Flush output buffer.*/
  virtual void FlushBufferImpl() {
		if (smag->fpLog) fflush(smag->fpLog);
	}
  //@}

private:
	smagHandle_t smag;
  /**@name Default Compiler Generated Methods
   * (Hidden to avoid implicit creation/calling).
   * These methods are not implemented and 
   * we do not want the compiler to implement
   * them for us, so we declare them private
   * and do not define them. This ensures that
   * they will not be implicitly created/called. */
  //@{
  /** Default Constructor */
  SmagJournal();

  /** Copy Constructor */
  SmagJournal(const SmagJournal&);

  /** Overloaded Equals Operator */
  void operator=(const SmagJournal&);
  //@}
};

#endif
