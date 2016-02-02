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

#include <FiniteVolumeMethod/FiniteVolumeMethod.hpp>

namespace DLA = DenseLinAlg;
namespace SLA = SparseLinAlg;
namespace FVM = FiniteVolumeMethod;

int main() {

	const int NumCtrlVol = 5;
	const double CylinderLength = 1.0;

	// 1-dimensional grid with cetral differce scheme
	FVM::Grid1D< FVM::CentDiffSchemeTag > grid( NumCtrlVol, CylinderLength);

	const double AmbientTemperature = 298.15,
			HotTemperature = AmbientTemperature + 100.0;

	grid.addDirichletBoundary(-1, 0, AmbientTemperature);
	grid.addNeumannBoundary( NumCtrlVol - 1, NumCtrlVol, 0.0);

	const double ThermalConductivity = 1000.0;

	// http://www.engineeringtoolbox.com/convective-heat-transfer-d_430.html
	const double ConvectiveHeatTransCoeff = 0.7;

	const double CylinderRadius = 2.0 * ConvectiveHeatTransCoeff /
													ThermalConductivity,
				Area = M_PI * CylinderRadius * CylinderRadius,
				Circumference = 2.0 * M_PI *  CylinderRadius;

	FVM::DifferentialOperatorType const DiffOpr =
								FVM::DifferentialOperatorType();
	FVM::IdentityOperatorType const IdOpr = FVM::IdentityOperatorType();

	// The operator part in the heat equation
    // which consists of a differential operator and identical one.
	auto opr = proto::deep_copy(
			ThermalConductivity * Area * DiffOpr * DiffOpr
			- ConvectiveHeatTransCoeff * Circumference * IdOpr );
	// double prefactor = ThermalConductivity * Area;
	// auto opr = proto::deep_copy(
	//		 prefactor * FVM::diffOpr * FVM::diffOpr
	//		- ConvectiveHeatTransCoeff * Circumference * FVM::identityOpr );

	FVM::BoundaryCorrector bCorrector( grid, opr);
//	FVM::BoundaryCorrector bCorrector( grid,
//			ThermalConductivity * Area * FVM::diffOpr * FVM::diffOpr
//			- ConvectiveHeatTransCoeff * Circumference * FVM::identityOpr );

	DLA::Matrix coeffMat = grid.discretizeOperator( opr );
//	DLA::Matrix coeffMat = grid.discretizeOperator(
//			ThermalConductivity * Area * FVM::diffOpr * FVM::diffOpr
//			- ConvectiveHeatTransCoeff * Circumference * FVM::identityOpr );
//	bCorrector.applyTo( coeffMat);

	// The constant term(s) in the heat equation
	// which is(are) independent from the temperature distribution
	DLA::Vector rhsVec = grid.discretizeFunction(
			ConvectiveHeatTransCoeff * Circumference * AmbientTemperature );
	bCorrector.applyTo( rhsVec);

	SLA::DiagonalPreconditioner precond( coeffMat);
	SLA::ConjugateGradient< DLA::Matrix, SLA::DiagonalPreconditioner >
													cg( coeffMat, precond);

	// Initial guess of discretized temperature distribution
	const DLA::Vector tempGuess = grid.discretizeFunction(
					( AmbientTemperature + HotTemperature ) / 2.0 );

	const double convergenceCriterion = 1.0e-5;
	DLA::Vector temperature = cg.solve(rhsVec, tempGuess,
										convergenceCriterion);

	grid.printForGnuPlot( temperature );

	return 0;
}
