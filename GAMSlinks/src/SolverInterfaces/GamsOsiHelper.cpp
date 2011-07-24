// Copyright (C) GAMS Development and others 2009-2011
// All Rights Reserved.
// This code is published under the Eclipse Public License.
//
// Author: Stefan Vigerske

#include "GamsOsiHelper.hpp"

#include <cstdlib>

#include "CoinHelperFunctions.hpp"
#include "CoinWarmStartBasis.hpp"
#include "OsiSolverInterface.hpp"

#include "gmomcc.h"
#include "gevmcc.h"

#include "GamsCompatibility.h"

bool gamsOsiLoadProblem(
   struct gmoRec*        gmo,                /**< GAMS modeling object */
   OsiSolverInterface&   solver,             /**< OSI solver interface */
   bool                  setupnames          /**< should col/row names be setup in Osi? */
)
{
   assert(gmo != NULL);

	struct gevRec* gev = (gevRec*)gmoEnvironment(gmo);
	assert(gev != NULL);

	// objective
	double* objcoeff = new double[gmoN(gmo)];
	gmoGetObjVector(gmo, objcoeff, NULL);
	solver.setDblParam(OsiObjOffset, -gmoObjConst(gmo));

	// matrix
	int nz = gmoNZ(gmo);
	double* values  = new double[nz];
	int* colstarts  = new int[gmoN(gmo)+1];
	int* rowindexes = new int[nz];
	int* nlflags    = new int[nz];

	gmoGetMatrixCol(gmo, colstarts, rowindexes, values, nlflags);
	colstarts[gmoN(gmo)] = nz;

	// squeeze zero elements
	int shift = 0;
	for( int col = 0; col < gmoN(gmo); ++col )
	{
	   colstarts[col+1] -= shift;
	   for( int k = colstarts[col]; k < colstarts[col+1]; )
	   {
	      values[k] = values[k+shift];
	      rowindexes[k] = rowindexes[k+shift];
	      if( !values[k] )
	      {
	         ++shift;
	         --colstarts[col+1];
	      }
	      else
	      {
	         ++k;
	      }
	   }
	}
	nz -= shift;

	// variable bounds
	double* varlow = new double[gmoN(gmo)];
	double* varup  = new double[gmoN(gmo)];
	gmoGetVarLower(gmo, varlow);
	gmoGetVarUpper(gmo, varup);

	// row sense
	char* rowsense = new char[gmoM(gmo)];
	for( int i = 0; i < gmoM(gmo); ++i )
		switch( (enum gmoEquType)gmoGetEquTypeOne(gmo, i) )
		{
			case gmoequ_E: rowsense[i] = 'E'; break;
			case gmoequ_G: rowsense[i] = 'G'; break;
			case gmoequ_L: rowsense[i] = 'L'; break;
			case gmoequ_N: rowsense[i] = 'N'; break;
			case gmoequ_C:
				gevLogStat(gev, "Error: Conic constraints not supported by OSI.");
				return false;
			default:
				gevLogStat(gev, "Error: Unsupported equation type.");
				return false;
		}
   // right-hand-side
   double* rhs = new double[gmoM(gmo)];
   gmoGetRhs(gmo, rhs);

//	printf("%d columns:\n", gmoN(gmo));
//	for (int i = 0; i < gmoN(gmo); ++i)
//		printf("lb %g\t ub %g\t obj %g\t colstart %d\n", varlow[i], varup[i], objcoeff[i], colstarts[i]);
//	printf("%d rows:\n", gmoM(gmo));
//	for (int i = 0; i < gmoM(gmo); ++i)
//		printf("rhs %g\t sense %c\t rng %g\n", rhs[i], rowsense[i], rowrng[i]);
//	printf("%d nonzero values:", nz);
//	for (int i = 0; i < nz; ++i)
//		printf("%d:%g ", rowindexes[i], values[i]);

	solver.loadProblem(gmoN(gmo), gmoM(gmo), colstarts, rowindexes, values, varlow, varup, objcoeff, rowsense, rhs, NULL);

	delete[] colstarts;
	delete[] rowindexes;
	delete[] values;
	delete[] nlflags;
	delete[] varlow;
	delete[] varup;
	delete[] objcoeff;
	delete[] rowsense;
	delete[] rhs;

	// objective sense
	switch( gmoSense(gmo) )
	{
	   case gmoObj_Min:
	      solver.setObjSense(1.0);
	      break;
	   case gmoObj_Max:
	      solver.setObjSense(-1.0);
	      break;
	   default:
	      gevLogStat(gev, "Error: Unsupported objective sense.");
	      return false;
	}

	// tell solver which variables are discrete
	if( gmoNDisc(gmo) )
	{
	   int* discrind = new int[gmoNDisc(gmo)];
	   int j = 0;
	   for( int i = 0; i < gmoN(gmo); ++i )
	   {
	      switch( (enum gmoVarType)gmoGetVarTypeOne(gmo, i) )
	      {
	         case gmovar_B:  // binary
	         case gmovar_I:  // integer
	         case gmovar_SI: // semiinteger
	            assert(j < gmoNDisc(gmo));
	            discrind[j++] = i;
	            break;
	         case gmovar_X:  // probably this means continuous variable
	         case gmovar_S1: // in SOS1
	         case gmovar_S2: // in SOS2
	         case gmovar_SC: // semicontinuous
	            break;
	      }
	   }
	   solver.setInteger(discrind, j);
	   delete[] discrind;
	}

	char inputname[1024];
	gmoNameInput(gmo, inputname);
	solver.setStrParam(OsiProbName, inputname);

   // setup column/row names, if available
   if( setupnames && gmoDict(gmo) != NULL )
   {
      solver.setIntParam(OsiNameDiscipline, 2);
      char buffer[255]; // TODO how long should var/col names be
      for( int j = 0; j < gmoN(gmo); ++j )
      {
         gmoGetVarNameOne(gmo, j, buffer);
         solver.setColName(j, buffer);
      }
      for( int j = 0; j < gmoM(gmo); ++j )
      {
         gmoGetEquNameOne(gmo, j, buffer);
         solver.setRowName(j, buffer);
      }
   }

	return true;
}

bool gamsOsiStoreSolution(
   struct gmoRec*        gmo,                /**< GAMS modeling object */
   const OsiSolverInterface& solver          /**< OSI solver interface */
)
{
   assert(gmo != NULL);

   struct gevRec* gev = (gevRec*)gmoEnvironment(gmo);
   assert(gev != NULL);

   const double* colLevel  = solver.getColSolution();
   const double* colMargin = solver.getReducedCost();
   const double* rowLevel  = solver.getRowActivity();
   const double* rowMargin = solver.getRowPrice();

   assert(gmoN(gmo) == 0 || colLevel);
   assert(gmoN(gmo) == 0 || colMargin);
   assert(gmoM(gmo) == 0 || rowLevel);
   assert(gmoM(gmo) == 0 || rowMargin);

   int* colBasis = new int[solver.getNumCols()];
   int* rowBasis = new int[solver.getNumRows()];

   // workaround for gmo if there are no rows or columns  TODO: still needed?
   double dummy2;
   if( !gmoN(gmo) )
   {
      colLevel  = &dummy2;
      colMargin = &dummy2;
   }
   if( !gmoM(gmo) )
   {
      rowLevel  = &dummy2;
      rowMargin = &dummy2;
   }

   if( solver.optimalBasisIsAvailable() )
   {
      solver.getBasisStatus(colBasis, rowBasis);

      // translate from OSI codes to GAMS codes
      for( int j = 0; j < gmoN(gmo); ++j )
      {
         // only for fully continuous variables we can give a reliable basis status
         if( gmoGetVarTypeOne(gmo, j) != gmovar_X )
            colBasis[j] = gmoBstat_Super;
         else
            switch( colBasis[j] )
            {
               // change onbound to super if value is not on bound as it should be
               case 3: colBasis[j] = (colLevel[j] > gmoGetVarLowerOne(gmo, j) + 1e-6) ? gmoBstat_Super : gmoBstat_Lower; break;
               case 2: colBasis[j] = (colLevel[j] < gmoGetVarUpperOne(gmo, j) - 1e-6) ? gmoBstat_Super : gmoBstat_Upper; break;
               case 1: colBasis[j] = gmoBstat_Basic; break;
               case 0: colBasis[j] = gmoBstat_Super; break;
               default: gevLogStat(gev, "Column basis status unknown!"); return false;
            }
      }
      for( int i = 0; i < gmoM(gmo); ++i )
      {
         switch( rowBasis[i] )
         {
            case 2: rowBasis[i] = gmoBstat_Lower; break;
            case 3: rowBasis[i] = gmoBstat_Upper; break;
            case 1: rowBasis[i] = gmoBstat_Basic; break;
            case 0: rowBasis[i] = gmoBstat_Super; break;
            default: gevLogStat(gev, "Row basis status unknown!"); return false;
         }
      }
   }
   else
   {
      CoinWarmStart* ws = solver.getWarmStart();
      CoinWarmStartBasis* wsb = dynamic_cast<CoinWarmStartBasis*>(ws);
      if( wsb != NULL )
      {
         for( int j = 0; j < gmoN(gmo); ++j )
            if( gmoGetVarTypeOne(gmo, j) != gmovar_X )
               colBasis[j] = gmoBstat_Super;
            else
               switch( wsb->getStructStatus(j) )
               {
                  // change onbound to super if value is not on bound as it should be
                  case CoinWarmStartBasis::atLowerBound: colBasis[j] = (colLevel[j] > gmoGetVarLowerOne(gmo, j) + 1e-6) ? gmoBstat_Super : gmoBstat_Lower; break; // change to super if value is not on bound as it should be
                  case CoinWarmStartBasis::atUpperBound: colBasis[j] = (colLevel[j] < gmoGetVarUpperOne(gmo, j) - 1e-6) ? gmoBstat_Super : gmoBstat_Upper; break;
                  case CoinWarmStartBasis::basic:        colBasis[j] = gmoBstat_Basic; break;
                  case CoinWarmStartBasis::isFree:       colBasis[j] = gmoBstat_Super; break;
                  default: gevLogStat(gev, "Column basis status unknown!"); return false;
               }
         for( int j = 0; j < gmoM(gmo); ++j )
         {
            switch (wsb->getArtifStatus(j))
            {
               // for Cbc, the basis status seem to be flipped in CoinWarmStartBasis, but not in getBasisStatus
               case CoinWarmStartBasis::atLowerBound: rowBasis[j] = gmoBstat_Upper; break;
               case CoinWarmStartBasis::atUpperBound: rowBasis[j] = gmoBstat_Lower; break;
               case CoinWarmStartBasis::basic:        rowBasis[j] = gmoBstat_Basic; break;
               case CoinWarmStartBasis::isFree:       rowBasis[j] = gmoBstat_Super; break;
               default: gevLogStat(gev, "Row basis status unknown!"); return false;
            }
         }
      }
      else
      {
         CoinFillN(colBasis, gmoN(gmo), (int)gmoBstat_Super);
         CoinFillN(rowBasis, gmoM(gmo), (int)gmoBstat_Super);
      }
      delete ws;
   }

   /* empty array for not having variable/equation status TODO: still needed? */
   int* dummy = CoinCopyOfArray((int*)NULL, CoinMax(gmoN(gmo), gmoM(gmo)), 0);

   gmoSetHeadnTail(gmo, gmoHobjval, solver.getObjValue());
   gmoSetSolution8(gmo, colLevel, colMargin, rowMargin, rowLevel, colBasis, dummy, rowBasis, dummy);

   delete[] colBasis;
   delete[] rowBasis;
   delete[] dummy;

   return true;
}
