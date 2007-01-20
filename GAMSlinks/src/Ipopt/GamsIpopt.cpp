// Copyright (C) GAMS Development 2007
// All Rights Reserved.
// This code is published under the Common Public License.
//
// $Id$
//
// Authors:  Steve Dirkse, Stefan Vigerske

#include "GAMSlinksConfig.h"

#include <iostream>
#include "IpIpoptApplication.hpp"
#include "SmagNLP.hpp"
#include "SmagJournal.hpp"

using namespace std;
using namespace Ipopt;

int main (int argc, char* argv[]) {
#if defined(_MSC_VER)
  /* Prevents hanging "Application Error, Click OK" Windows in case something bad happens */
  { UINT oldMode = SetErrorMode(SEM_FAILCRITICALERRORS | SEM_NOGPFAULTERRORBOX); }
#endif
  smagHandle_t prob;

  if (argc < 2) {
    cout << "usage: " << argv[0] << " control_file_name" << endl;
    cout << "exiting . . . " << endl;
    return EXIT_FAILURE;
  }

  prob = smagInit (argv[1]);
  if (!prob) {
  	cerr << "Error reading control file " << argv[1] << endl;
  	cerr << "exiting . . . " << endl;
		return EXIT_FAILURE;  	
  }

	smagOpenLog(prob);
	FILE* fpStatus;
	char buffer[255];
	if (smagOpenStatusC(prob, 0, &fpStatus, buffer, 255)) {
		cerr << "Error opening status file: " << buffer << endl;
		fpStatus=NULL;
	} else {
		prob->fpStatus=fpStatus;
	}
	cout << prob->gms.statusFileName << endl;
  smagReadModelStats (prob);
  smagSetObjFlavor (prob, OBJ_FUNCTION);
  smagSetSqueezeFreeRows (prob, 1);	/* don't show me =n= rows */
  smagReadModel (prob);
  smagHessInit (prob);

	smagPrint(prob, SMAG_ALLMASK, "\nGAMS/IPOPT NLP Solver (IPOPT Library 3.2.3)\nwritten by A. Waechter\n");
	if (prob->fpLog)
		fflush(prob->fpLog);

  // Create a new instance of your nlp (use a SmartPtr, not raw)
  SmartPtr<TNLP> smagnlp = new SMAG_NLP (prob);
  // Create a new instance of IpoptApplication (use a SmartPtr, not raw)
  SmartPtr<IpoptApplication> app = new IpoptApplication(false);
  
  SmartPtr<Journal> smag_jrnl=new SmagJournal(prob, prob->logOption ? J_SUMMARY : J_ITERSUMMARY);
	if (!IsNull(smag_jrnl)) smag_jrnl->SetPrintLevel(J_DBG, J_NONE);  	
	if (!app->Jnlst()->AddJournal(smag_jrnl))
		smagPrint(prob, SMAG_ALLMASK, "Failed to register SmagJournal for IPOPT output.\n");  

	// Change some options
  app->Options()->SetNumericValue("bound_relax_factor", 0);
	app->Options()->SetIntegerValue("max_iter", prob->gms.itnlim);
  app->Options()->SetStringValue("mu_strategy", "adaptive");
 	app->Options()->SetNumericValue("nlp_lower_bound_inf", -prob->inf, false);
 	app->Options()->SetNumericValue("nlp_upper_bound_inf",  prob->inf, false);

	if (prob->gms.useopt)
		app->Initialize(prob->gms.optFileName);
	else
		app->Initialize("");
	
  // Ask Ipopt to solve the problem
  ApplicationReturnStatus status = app->OptimizeTNLP(smagnlp);

	switch (status) {
		case Solve_Succeeded:
		case Solved_To_Acceptable_Level:
		case Infeasible_Problem_Detected:
		case Search_Direction_Becomes_Too_Small:
		case Diverging_Iterates:
		case User_Requested_Stop:
		case Maximum_Iterations_Exceeded:
		case Restoration_Failed:
		case Error_In_Step_Computation:
			break; // these should have been catched by FinalizeSolution already

		case Not_Enough_Degrees_Of_Freedom:
			smagReportSolBrief(prob, 13, 10);
			break;
		case Invalid_Problem_Definition:
			smagReportSolBrief(prob, 13, 9);
			break;
		case Invalid_Option:
			smagReportSolBrief(prob, 13, 9);
			break;
		case Invalid_Number_Detected:
			smagReportSolBrief(prob, 13, 10);
			break;
		case Unrecoverable_Exception:
			smagReportSolBrief(prob, 13, 11);
			break;
		case NonIpopt_Exception_Thrown:
			smagReportSolBrief(prob, 13, 13);
			break;
		case Insufficient_Memory:
			smagReportSolBrief(prob, 13, 10);
			break;
		case Internal_Error:
			smagReportSolBrief(prob, 13, 11);
			break;
		default:
			smagReportSolBrief(prob, 13, 13);
			break;
	}
	
	smag_jrnl=NULL;
	if (fpStatus && fpStatus!=stdout) {
		fclose(fpStatus);
		prob->fpStatus=NULL;
	}
	smagCloseLog(prob);
	smagClose(prob);

  return EXIT_SUCCESS;
} // main


// enum ApplicationReturnStatus
//   {
//     Solve_Succeeded=0,
//     Solved_To_Acceptable_Level=1,
//     Infeasible_Problem_Detected=2,
//     Search_Direction_Becomes_Too_Small=3,
//     Diverging_Iterates=4,
//     User_Requested_Stop=5,
// 
//     Maximum_Iterations_Exceeded=-1,
//     Restoration_Failed=-2,
//     Error_In_Step_Computation=-3,
//     Not_Enough_Degrees_Of_Freedom=-10,
//     Invalid_Problem_Definition=-11,
//     Invalid_Option=-12,
//     Invalid_Number_Detected=-13,
// 
//     Unrecoverable_Exception=-100,
//     NonIpopt_Exception_Thrown=-101,
//     Insufficient_Memory=-102,
//     Internal_Error=-199
//   };

// SOLVER STATUS CODE  	DESCRIPTION
// 1 	Normal Completion
// 2 	Iteration Interrupt
// 3 	Resource Interrupt
// 4 	Terminated by Solver
// 5 	Evaluation Error Limit
// 6 	Capability Problems
// 7 	Licensing Problems
// 8 	User Interrupt
// 9 	Error Setup Failure
// 10 	Error Solver Failure
// 11 	Error Internal Solver Error
// 12 	Solve Processing Skipped
// 13 	Error System Failure
// MODEL STATUS CODE  	DESCRIPTION
// 1 	Optimal
// 2 	Locally Optimal
// 3 	Unbounded
// 4 	Infeasible
// 5 	Locally Infeasible
// 6 	Intermediate Infeasible
// 7 	Intermediate Nonoptimal
// 8 	Integer Solution
// 9 	Intermediate Non-Integer
// 10 	Integer Infeasible
// 11 	Licensing Problems - No Solution
// 12 	Error Unknown
// 13 	Error No Solution
// 14 	No Solution Returned
// 15 	Solved Unique
// 16 	Solved
// 17 	Solved Singular
// 18 	Unbounded - No Solution
// 19 	Infeasible - No Solution
