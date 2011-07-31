// Copyright (C) GAMS Development and others 2009-2011
// All Rights Reserved.
// This code is published under the Eclipse Public License.
//
// Author: Stefan Vigerske

#ifndef GAMSIPOPT_HPP_
#define GAMSIPOPT_HPP_

#include "GamsSolver.hpp"

#include "IpTNLP.hpp"
#include "IpIpoptApplication.hpp"

class GamsIpopt : public GamsSolver
{
private:
   struct gmoRec*         gmo;                /**< GAMS modeling object */
   struct gevRec*         gev;                /**< GAMS environment */

   Ipopt::SmartPtr<Ipopt::IpoptApplication> ipopt;
   Ipopt::SmartPtr<Ipopt::TNLP>             nlp;

public:
   GamsIpopt()
   : gmo(NULL),
     gev(NULL)
   { }

   int readyAPI(
      struct gmoRec*     gmo_,
      struct optRec*     opt
   );

   int callSolver();
};

#endif
