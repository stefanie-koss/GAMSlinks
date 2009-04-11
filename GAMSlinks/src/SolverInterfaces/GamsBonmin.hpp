// Copyright (C) GAMS Development and others 2009
// All Rights Reserved.
// This code is published under the Common Public License.
//
// $Id$
//
// Author: Stefan Vigerske

#ifndef GAMSBONMIN_HPP_
#define GAMSBONMIN_HPP_

#include "GAMSlinksConfig.h"
#include "GamsSolver.hpp"
#include "GamsMINLP.hpp"

class GamsMessageHandler;
class GamsIpopt;
namespace Bonmin {
	class BonminSetup;
	class OsiTMINLPInterface;
}

class GamsBonmin : public GamsSolver {
private:
	struct gmoRec* gmo;
		
	char           bonmin_message[100];

	GamsMessageHandler*    msghandler;
	Ipopt::SmartPtr<GamsMINLP> minlp;
	Bonmin::BonminSetup*   bonmin_setup;
	GamsIpopt*             gamsipopt;

	void writeSolution(Bonmin::OsiTMINLPInterface& osi_tminlp, int itercount);
	void writeSolutionNoDual(Bonmin::OsiTMINLPInterface& osi_tminlp, int itercount);
	
	bool isNLP();

public:
	GamsBonmin();
	~GamsBonmin();
	
	int readyAPI(struct gmoRec* gmo, struct optRec* opt, struct dctRec* gcd);
	
//	int haveModifyProblem();
	
//	int modifyProblem();
	
	int callSolver();
	
	const char* getWelcomeMessage() { return bonmin_message; }

}; // GamsBonmin

extern "C" DllExport GamsBonmin* STDCALL createNewGamsBonmin();

#endif /*GAMSBONMIN_HPP_*/
