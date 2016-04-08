/*
 * airCooledCylinder.cpp
 *
 * ref) H. K. Versteeg and W. Malalasekera,
 *     "An Introduction  to Computational Fluid Dynamics,
 *     The Finite Volume Method", 2nd Ed.
 *     Pearson Educational Limited 1995, 2007.
 *
 *     Example 4.3
 *
 *  Created on: 2015/03/31
 *      Author: mito
 */

#include "airCooledCylinder.hpp"

typedef DLA::Matrix Matrix;
typedef DLA::Vector Vector;


int main() {

	const int NumCtrlVol = 5;
	std::cout << "The number of control volumes = " <<
			NumCtrlVol << std::endl;

	printConstants();


	// 1-dimensional grid with cetral differce scheme
	FVM::Grid1D< FVM::CentDiffSchemeTag > grid( NumCtrlVol, CylinderLength);

	grid.addDirichletBoundary(-1, 0, HotTemperature);
	grid.addNeumannBoundary( NumCtrlVol, NumCtrlVol - 1, 0.0);

	FVM::IdentityOperatorType const IdOpr = FVM::IdentityOperatorType();
	FVM::DifferentialOperatorType const DiffOpr =
								FVM::DifferentialOperatorType();

	// The operator part in the heat equation
    // which consists of a differential operator and identical one.
	auto opr = proto::deep_copy(
			ThermalConductivity * Area * DiffOpr * DiffOpr
			- ConvectiveHeatTransCoeff * Circumference * IdOpr );


	Matrix coeffMat( NumCtrlVol, NumCtrlVol);
	coeffMat = grid.discretizeOperator( opr );

	Vector rhsVec( NumCtrlVol);
	rhsVec = grid.discretizeFunction(
			- ConvectiveHeatTransCoeff * Circumference * AmbientTemperature );

	FVM::BoundaryCorrector bCorrector( grid, opr);
	bCorrector.applyTo( coeffMat);
	bCorrector.applyTo( rhsVec);


	const double scale = - ThermalConductivity * Area;
	std::cout << "scale = - ThermalConductivity * Area" <<
			std::endl;
	std::cout << "= " << scale << std::endl;

	printCoefficients( coeffMat, scale);
	printRHS( rhsVec, scale);


	SLA::DiagonalPreconditioner precond( coeffMat);
	SLA::ConjugateGradient< DLA::Matrix, SLA::DiagonalPreconditioner >
	                   							cg( coeffMat, precond);

	const Vector tempGuess( NumCtrlVol, (100.0 + 20.0) / 2.0);

	Vector temperature( NumCtrlVol);
	temperature = cg.solve(rhsVec, tempGuess, convergenceCriterion);

	printCalculatedAndExactTemperatureDistributions< Vector >(
														temperature);

	return 0;
}

