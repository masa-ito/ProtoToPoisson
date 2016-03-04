/*
 * heatGeneratingCuboid.cpp
 *
 *  Created on: 2016/02/18
 *      Author: Masakatsu ITO
 */

#include <math.h>

#include <iostream>
#include <iomanip>

#include <boost/proto/proto.hpp>

#include <DenseLinAlg/DenseLinAlg.hpp>
#include <SparseLinAlg/SparseLinAlg.hpp>

#include <FiniteVolumeMethod/FiniteVolumeMethod.hpp>

namespace DLA = DenseLinAlg;
namespace SLA = SparseLinAlg;
namespace FVM = FiniteVolumeMethod;



int main() {
	// The numbers of control volumes along x, y ,z axes
	const int NumCtrlVolX = 7, NumCtrlVolY = 5, NumCtrlZ = 3;
	// Then lengths of cuboid sides
	const double LenX = 7.0, LenY = 5.0, LenZ = 3.0;

	// 3-dimensional grid with cetral differce scheme
	FVM::Grid3D< FVM::CentDiffSchemeTag > grid( NumCtrlVolX, LenX,
								NumCtrlVolY, LenY, NumCtrlVolZ, LenZ);

	const double CoolingTemperature = 0.0;

	// You should resume coding from this point !!!  18 Feb.

	grid.addDirichletBoundary(-1, 0, HotTemperature);
	grid.addNeumannBoundary( NumCtrlVol, NumCtrlVol - 1, 0.0);

	const double ThermalConductivity = 1000.0;

	// http://www.engineeringtoolbox.com/convective-heat-transfer-d_430.html
	const double ConvectiveHeatTransCoeff = 0.7;

	const double CylinderRadius = 2.0 / 25.0 * ConvectiveHeatTransCoeff /
													ThermalConductivity,
				Area = M_PI * CylinderRadius * CylinderRadius,
				Circumference = 2.0 * M_PI *  CylinderRadius;

	FVM::IdentityOperatorType const IdOpr = FVM::IdentityOperatorType();
	FVM::DifferentialOperatorType const DiffOpr =
								FVM::DifferentialOperatorType();

	// The operator part in the heat equation
    // which consists of a differential operator and identical one.
	auto opr = proto::deep_copy(
			ThermalConductivity * Area * DiffOpr * DiffOpr
			- ConvectiveHeatTransCoeff * Circumference * IdOpr );

	std::cout << "ThermalConductivity * Area = " <<
			ThermalConductivity * Area << std::endl;

	std::cout << "- ConvectiveHeatTransCoeff * Circumference = " <<
			- ConvectiveHeatTransCoeff * Circumference << std::endl;

	std::cout << "ConvectiveHeatTransCoeff * Circumference * " <<
			"AmbientTemperature = " <<
			ConvectiveHeatTransCoeff * Circumference * AmbientTemperature <<
			std::endl;

	DLA::Matrix coeffMat( NumCtrlVol, NumCtrlVol);
	coeffMat = grid.discretizeOperator( opr );

	DLA::Vector rhsVec( NumCtrlVol);
	rhsVec = grid.discretizeFunction(
			- ConvectiveHeatTransCoeff * Circumference * AmbientTemperature );

	FVM::BoundaryCorrector bCorrector( grid, opr);
	bCorrector.applyTo( coeffMat);
	bCorrector.applyTo( rhsVec);

	int ri, ci;

	const double scale = - ThermalConductivity * Area;
	std::cout << "scale = - ThermalConductivity * Area" <<
			std::endl;
	std::cout << "= " << scale << std::endl;

	std::cout << "Coefficient matrix after applying boundary conditions" <<
			std::endl;
	std::cout << " / scale" << std::endl;
	for (ri = 0; ri < NumCtrlVol; ri++)
	{
		for (ci = 0; ci < NumCtrlVol; ci++)
			std::cout << std::setw(11) << std::fixed << std::setprecision(4) <<
				coeffMat(ri, ci) / scale;
		std::cout << std::endl;
	}

	std::cout << std::endl;
	std::cout << "RHS vector after applying boundary conditions" <<
			std::endl;
	std::cout << " / scale" << std::endl;
	for (ci = 0; ci < NumCtrlVol; ci++)
		std::cout << std::setw(11) << std::fixed << std::setprecision(4) <<
						rhsVec( ci) / scale << std::endl;

	SLA::DiagonalPreconditioner precond( coeffMat);
	SLA::ConjugateGradient< DLA::Matrix, SLA::DiagonalPreconditioner >
	                   							cg( coeffMat, precond);

	const DLA::Vector tempGuess( NumCtrlVol, (100.0 + 20.0) / 2.0);
	const double convergenceCriterion = 1.0e-7;

	DLA::Vector temperature( NumCtrlVol);
	temperature = cg.solve(rhsVec, tempGuess, convergenceCriterion);


	// grid.printForGnuPlot( temperature );

	ExactTempDist exactDist(CylinderLength,
				AmbientTemperature, HotTemperature,
				ThermalConductivity, Area,
				ConvectiveHeatTransCoeff, Circumference);

	std::cout << "# stationary temperature distribution" << std::endl;
	std::cout << "#     x      FVM       exact " << std::endl;

	for (ci =0; ci < NumCtrlVol; ci++) {
		double x = CylinderLength / NumCtrlVol * (ci + 0.5);
		std::cout << std::setw( 8) << std::fixed << std::setprecision(2) << x;
		std::cout << std::setw(10) << std::fixed << std::setprecision(3) <<
				temperature( ci);
		std::cout << std::setw(10) << std::fixed << std::setprecision(3) <<
				exactDist( x) << std::endl;
	}

	return 0;
}




