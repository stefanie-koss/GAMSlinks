// Copyright (C) GAMS Development and others 2009-2011
// All Rights Reserved.
// This code is published under the Eclipse Public License.
//
// Author: Stefan Vigerske

#ifndef GAMSCBC_HPP_
#define GAMSCBC_HPP_

#include "GamsSolver.hpp"

#include <cstdlib>

class CbcModel;
class GamsCbcMessageHandler;
class OsiSolverInterface;

/** GAMS interface to CBC */
class GamsCbc : public GamsSolver
{
private:
   struct gmoRec*        gmo;                /**< GAMS modeling object */
   struct gevRec*        gev;                /**< GAMS environment */
   struct optRec*        opt;                /**< GAMS options object */

   GamsCbcMessageHandler* msghandler;        /**< message handler */
   CbcModel*             model;              /**< CBC model object */
   int                   cbc_argc;           /**< number of parameters to pass to CBC */
   char**                cbc_args;           /**< parameters to pass to CBC */

   double                optcr;              /**< relative optimality tolerance */
   double                optca;              /**< absolute optimality tolerance */
   bool                  mipstart;           /**< whether to pass primal solution to MIP solve */
   int                   nthreads;           /**< number of threads to use */
   char*                 writemps;           /**< name of mps file to write instance to */
   bool                  solvefinal;         /**< whether to solve MIP with fixed discrete variables finally */
   char*                 miptrace;           /**< name of trace file to write branch-and-bound info to */
   int                   miptracenodefreq;   /**< node frequency for mip trace */
   double                miptracetimefreq;   /**< time frequency for mip trace */

   bool setupProblem();

   bool setupStartingPoint();

   bool setupParameters();

   bool writeSolution(
      double             cputime,            /**< CPU time spend by solver */
      double             walltime            /**< wallclock time spend by solver */
   );

   bool isLP();

public:
   GamsCbc()
   : gmo(NULL),
     gev(NULL),
     opt(NULL),
     msghandler(NULL),
     model(NULL),
     cbc_argc(0),
     cbc_args(NULL),
     optcr(0.0),
     optca(0.0),
     mipstart(false),
     nthreads(1),
     writemps(NULL),
     solvefinal(true),
     miptrace(NULL),
     miptracenodefreq(100),
     miptracetimefreq(5.0)
   { }

   ~GamsCbc();

   int readyAPI(
      struct gmoRec*     gmo_,               /**< GAMS modeling object */
      struct optRec*     opt_                /**< GAMS options object */
   );

   int callSolver();
};

#endif /*GAMSCBC_HPP_*/
