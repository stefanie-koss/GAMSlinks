// Copyright (C) GAMS Development and others 2009-2011
// All Rights Reserved.
// This code is published under the Eclipse Public License.
//
// Author: Stefan Vigerske

#ifndef GAMSSCIP_HPP_
#define GAMSSCIP_HPP_

#include "GamsSolver.hpp"

#include <cstdlib>

#include "scip/type_retcode.h"

typedef struct Scip             SCIP;
typedef struct SCIP_Messagehdlr SCIP_MESSAGEHDLR;

/** GAMS interface to SCIP */
class GamsScip : public GamsSolver
{
private:
   struct gmoRec*        gmo;                /**< GAMS modeling object */
   struct gevRec*        gev;                /**< GAMS environment */
   bool                  isDemo;             /**< whether we run SCIP in demo mode */

   SCIP*                 scip;               /**< SCIP structure */
   SCIP_MESSAGEHDLR*     scipmsghandler;     /**< SCIP message handler to write through GEV */

   SCIP_RETCODE setupSCIP();
   SCIP_RETCODE freeSCIP();

public:
   GamsScip()
   : gmo(NULL),
     gev(NULL),
     isDemo(false),
     scip(NULL),
     scipmsghandler(NULL)
   { }

   ~GamsScip();

   int readyAPI(
      struct gmoRec*     gmo_,               /**< GAMS modeling object */
      struct optRec*     opt_                /**< GAMS options object */
   );

   int callSolver();
};

#endif /*GAMSSCIP_HPP_*/
