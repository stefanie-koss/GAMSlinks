// Copyright (C) GAMS Development 2006
// All Rights Reserved.
// This code is published under the Common Public License.
//
// $Id$
//
// Authors:  Michael Bussieck, Stefan Vigerske

#include "GAMSlinksConfig.h"

#ifdef HAVE_CSTDIO
#include <cstdio>
#else
#ifdef HAVE_STDIO_H
#include <stdio.h>
#else
#error "don't have header file for stdio"
#endif
#endif

#include "OsiGlpkSolverInterface.hpp"
#include "CoinPackedVector.hpp"

// GAMS
#include "GamsModel.hpp"
#include "GamsMessageHandler.hpp"
#include "GamsFinalize.hpp"

int printme(void *info, char *msg) {
	GamsMessageHandler *myout;

	myout = (GamsMessageHandler*) info;
	*myout << msg << CoinMessageEol;

	return 1;
}

void write_mps(GamsModel& gm, OsiSolverInterface& solver, GamsMessageHandler& myout, char* filename);

//#############################################################################

int main (int argc, const char *argv[]) {
#if defined(_MSC_VER)
  /* Prevents hanging "Application Error, Click OK" Windows in case something bad happens */
  { UINT oldMode = SetErrorMode(SEM_FAILCRITICALERRORS | SEM_NOGPFAULTERRORBOX); }
#endif

	if (argc==1) {
		fprintf(stderr, "usage: %s <control_file_name>\nexiting ...\n",  argv[0]);
		exit(EXIT_FAILURE);
	}	
	int j;
	char buffer[255]; 

	OsiGlpkSolverInterface solver;

	// Read in the model defined by the GAMS control file passed in as the first
	// argument to this program
	GamsModel gm(argv[1],-solver.getInfinity(),solver.getInfinity());

	// Pass in the GAMS status/log file print routines 
	GamsMessageHandler myout, slvout;
	myout.setGamsModel(&gm);
	lib_set_print_hook(&myout, printme);

	slvout.setGamsModel(&gm); slvout.setPrefix(0);
	solver.passInMessageHandler(&slvout);

	myout << "\nGAMS/CoinGlpk Lp/Mip Solver (Glpk Library 4.9)\nwritten by A. Makhorin\n " << CoinMessageEol;
	
	if (gm.nSOS1() || gm.nSOS2()) {
		myout << "GLPK cannot handle special ordered sets (SOS)" << CoinMessageEol;
		myout << "Exiting ..." << CoinMessageEol;
		exit(EXIT_FAILURE);
	}
	
#ifdef GAMS_BUILD	
	if (!gm.ReadOptionsDefinitions("coinglpk"))
#else
	if (!gm.ReadOptionsDefinitions("glpk"))
#endif
		myout << "Error intializing option file handling or reading option file definitions!" << CoinMessageEol
			<< "Processing of options is likely to fail!" << CoinMessageEol;  
	gm.ReadOptionsFile();

	/* Overwrite GAMS Options */
	if (!gm.optDefined("reslim")) gm.optSetDouble("reslim", gm.getResLim());
	if (!gm.optDefined("iterlim")) gm.optSetInteger("iterlim", gm.getIterLim());
	if (!gm.optDefined("optcr")) gm.optSetDouble("optcr", gm.getOptCR());

	gm.TimerStart();

	// OsiSolver needs rowrng for the loadProblem call
	double *rowrng = new double[gm.nRows()];
	for (j=0; j<gm.nRows(); ++j) rowrng[j] = 0.0;
	
	// Glpk does not like zeros in the problem matrix
	gm.matSqueezeZeros();
		
	solver.setObjSense(gm.ObjSense());
	solver.setDblParam(OsiObjOffset, gm.ObjRhs()); // obj constant

	solver.loadProblem(gm.nCols(), gm.nRows(), gm.matStart(), 
															gm.matRowIdx(), gm.matValue(), 
															gm.ColLb(), gm.ColUb(), gm.ObjCoef(), 
															gm.RowSense(), gm.RowRhs(), rowrng);
 	
	// We don't need these guys anymore
	delete[] rowrng;

	// Tell solver which variables are discrete
	int *discVar=gm.ColDisc();
	for (j=0; j<gm.nCols(); j++) 
		if (discVar[j]) solver.setInteger(j);

	// Write MPS file
	if (gm.optDefined("writemps"))
		write_mps(gm, solver, myout, gm.optGetString("writemps", buffer));

	// why an LP solver cannot minimize a linear function over a box?
	if (!gm.nRows()) {
		myout << "Problem has no rows. Adding fake row..." << CoinMessageEol;
		int index=0; double coeff=1;
		CoinPackedVector vec(1, &index, &coeff);
		solver.addRow(vec, -solver.getInfinity(), solver.getInfinity());
	}
	
	// set variable and constraint names in glpk
	for (j=gm.nCols(); j; j--)
		if (gm.ColName(j-1, buffer, 255))
			lpx_set_col_name(solver.getModelPtr(), j, buffer);
	for (j=gm.nRows(); j; j--)
		if (gm.RowName(j-1, buffer, 255))
			lpx_set_row_name(solver.getModelPtr(), j, buffer);
	if (!gm.nRows()) {
		strcpy(buffer,"fake");
		lpx_set_row_name(solver.getModelPtr(), 1, buffer);
	}

	// Some tolerances and limits
	solver.setIntParam(OsiMaxNumIteration, gm.optGetInteger("iterlim"));
	lpx_set_real_parm(solver.getModelPtr(), LPX_K_TMLIM, gm.optGetDouble("reslim"));
	
	// One needs to change the glkp source because the range for optcr is enforced
	// to be between 1e-7 and 1e-3
	//		 case LPX_K_TOLOBJ:
	//				/* if (!(DBL_EPSILON <= val && val <= 0.001)) */	 <- Original
	//				if (DBL_EPSILON > val)														 <- GAMS change
	lpx_set_real_parm(solver.getModelPtr(), LPX_K_TOLOBJ, max(1e-7,gm.optGetDouble("optcr")));
	lpx_set_real_parm(solver.getModelPtr(), LPX_K_TOLINT, gm.optGetDouble("tol_integer"));
	solver.setDblParam(OsiDualTolerance, gm.optGetDouble("tol_dual"));
	solver.setDblParam(OsiPrimalTolerance, gm.optGetDouble("tol_primal"));

	// more parameters
	gm.optGetString("scaling", buffer);
	if (strcmp(buffer, "off")==0)
		lpx_set_int_parm(solver.getModelPtr(), LPX_K_SCALE, 0);
	else if (strcmp(buffer, "equilibrium")==0)
		lpx_set_int_parm(solver.getModelPtr(), LPX_K_SCALE, 1); // default
	else if (strcmp(buffer, "mean")==0)
		lpx_set_int_parm(solver.getModelPtr(), LPX_K_SCALE, 2); // default
	else if (strcmp(buffer, "meanequilibrium")==0)
		lpx_set_int_parm(solver.getModelPtr(), LPX_K_SCALE, 3); // default

	gm.optGetString("startalg", buffer);
	if (strcmp(buffer, "dual")==0)
		lpx_set_int_parm(solver.getModelPtr(), LPX_K_DUAL, 1);
	
	gm.optGetString("pricing", buffer);
	if (strcmp(buffer, "textbook")==0)
		lpx_set_int_parm(solver.getModelPtr(), LPX_K_PRICE, 0);
	else if	(strcmp(buffer, "steepestedge")==0)
		lpx_set_int_parm(solver.getModelPtr(), LPX_K_PRICE, 1);

	solver.messageHandler()->setLogLevel(3);

	if (gm.nDCols())
		myout << "Solving root problem... " << CoinMessageEol;

	solver.initialSolve();
	
	if (0 != gm.nDCols()) {
		myout << "\n" << CoinMessageEol;

		gm.optGetString("backtracking", buffer);
		if (strcmp(buffer, "depthfirst")==0)
			lpx_set_int_parm(solver.getModelPtr(), LPX_K_BTRACK, 0);
		else if	(strcmp(buffer, "breadthfirst")==0)
			lpx_set_int_parm(solver.getModelPtr(), LPX_K_BTRACK, 1);
		else if	(strcmp(buffer, "bestprojection")==0)
			lpx_set_int_parm(solver.getModelPtr(), LPX_K_BTRACK, 2);
			
		lpx_set_int_parm(solver.getModelPtr(), LPX_K_USECUTS, gm.optGetBool("cuts"));

		// Do complete search
		myout << "Starting Branch and Bound... " << CoinMessageEol;
		solver.branchAndBound();

		int mipstat = lpx_mip_status(solver.getModelPtr());
		if (mipstat == LPX_I_FEAS || mipstat == LPX_I_OPT) {
			const double *colLevel=solver.getColSolution();
			// We are loosing colLevel after a call to lpx_set_* when using the Visual compiler. So we save the levels.
			double *colLevelsav = new double[gm.nCols()];
			for (j=0; j<gm.nCols(); j++) colLevelsav[j] = colLevel[j];
			
			// No time or iteration limit for fixed run
			lpx_set_real_parm(solver.getModelPtr(), LPX_K_TMLIM, -1.0);
			lpx_set_int_parm(solver.getModelPtr(), LPX_K_ITLIM, -1);
			
			for (j=0; j<gm.nCols(); j++) 
				if (discVar[j])
					solver.setColBounds(j,colLevelsav[j],colLevelsav[j]);
			delete[] colLevelsav;
				
			solver.messageHandler()->setLogLevel(1);
			myout << "\nSolving fixed problem... " << CoinMessageEol;
			solver.resolve();
			if (!solver.isProvenOptimal()) 
			myout << "Problems solving fixed problem. No solution returned." << CoinMessageEol; 
		}
	}

	// Determine status and write solution
	GamsFinalizeOsi(&gm, &myout, &solver, 0);

	return 0;
}

void write_mps(GamsModel& gm, OsiSolverInterface& solver, GamsMessageHandler& myout, char* filename) {
	const char **colnames=new const char *[gm.nCols()];
	const char **rownames=new const char *[gm.nRows()];
	char namebuf[10];
	int j;

	for (j=gm.nCols()-1; j>=0; --j) {
		sprintf(namebuf,"X%d",j);
		colnames[j] = strdup(namebuf);
	}

	for (j=gm.nRows()-1; j>=0; --j) {
		sprintf(namebuf,"E%d",j);
		rownames[j] = strdup(namebuf);
	}

	myout << "Writing MPS file " << filename << "... " << CoinMessageEol;
	solver.writeMpsNative(filename,rownames,colnames,0,2,gm.ObjSense());

	// We don't need these guys anymore
	for (j=gm.nRows()-1; j>=0; j--)
		free((void*)rownames[j]);
	for (j=gm.nCols()-1; j>=0; j--)
		free((void*)colnames[j]);
	delete[] rownames;
	delete[] colnames;
}
