/*
 * airCooledCylinder.cpp
 *
 *  Created on: 2015/03/31
 *      Author: mito
 */

#include <math.h>

#include <iostream>
#include <boost/proto/proto.hpp>

#include <DenseLinAlg/DenseLinAlg.hpp>
#include <SparseLinAlg/SparseLinAlg.hpp>

namespace DLA = DenseLinAlg;
namespace SLA = SparseLinAlg;
namespace FVM = FiniteVolumeMethod;

int main() {

	const int NumCtrlVol = 5;
	const double CylinderLength = 1.0;

	// Control volumes on 1 dimensional grid
	FVM::CtrlVolGrid1D< FVM::CentralDiff > cv( NumCtrlVol, CylinderLength);

	const double AmbientTemperature = 298.15,
			HotTemperature = AmbientTemperature + 100.0;

	cv.setDirichletCond(-1, 0, AmbientTemperature);
	cv.setNeumannCond( NumCtrlVol - 1, NumCtrlVol, 0.0);

	const double ThermalConductivity = 1000.0;

	// http://www.engineeringtoolbox.com/convective-heat-transfer-d_430.html
	const double ConvectiveHeatTransCoeff = 0.7;

	const double CylinderRadius = 2.0 * ConvectiveHeatTransCoeff /
													ThermalConductivity,
				Area = M_PI * CylinderRadius * CylinderRadius,
				Circumference = 2.0 * M_PI *  CylinderRadius;

	DLA::Vector rhs( NumCtrlVol);

	DLA::Matrix coeff( NumCtrlVol, NumCtrlVol);

	cv.discretize(
		// The operator(s) in the heat equation
	    // whose term can be a differential operator or an identical one.
		ThermalConductivity * Area * FVM::d2dx2
		- ConvectiveHeatTransCoeff * Circumference * FVM::idOpr,

		// The constant term(s) in the heat equation
		// which is(are) independent from the temperature distribution
		ConvectiveHeatTransCoeff * Circumference * AmbientTemperature,

		// Coefficient matrix of the discretized equation
		coeff,
		// RHS of the discreteized equation
		rhs);

	SLA::DiagonalPreconditioner precond( coeff);
	const double convergenceCriterion = 1.0e-5;
	SLA::ConjugateGradient< DLA::Matrix, SLA::DiagonalPreconditioner >
														cg( coeff, precond);
	DLA::Vector tempGuess( NumCtrlVol,
						(  AmbientTemperature + HotTemperature ) / 2.0 );
	DLA::Vector temperature = cg.solve(rhs, tempGuess,
										convergenceCriterion);

	cv.printForGnuPlot( temperature );

	return 0;
}
