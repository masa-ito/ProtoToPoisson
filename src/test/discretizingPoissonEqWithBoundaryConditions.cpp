/*
 * discretizingPoissonEqWithBoundaryConditions.cpp
 *
 * ref) H. K. Versteeg and W. Malalasekera,
 *     "An Introduction  to Computational Fluid Dynamics,
 *     The Finite Volume Method", 2nd Ed.
 *     Pearson Educational Limited 1995, 2007.
 *
 *     Example 4.3
 *
 *  Created on: 2016/02/03
 *      Author: Masakatsu ITO
 */

#include <math.h>

#include <iostream>
#include <iomanip>

#include <boost/proto/proto.hpp>

#include <DenseLinAlg/DenseLinAlg.hpp>
// #include <SparseLinAlg/SparseLinAlg.hpp>

// #include <FiniteVolumeMethod/FiniteVolumeMethod.hpp>
#include <FiniteVolumeMethod/BoundaryCorrector.hpp>
#include <FiniteVolumeMethod/Grid.hpp>


namespace mpl = boost::mpl;
namespace proto = boost::proto;

namespace DLA = DenseLinAlg;
// namespace SLA = SparseLinAlg;
namespace FVM = FiniteVolumeMethod;

int main()
{
	const int NumCtrlVol = 5;
	const double CylinderLength = 1.0;

	// 1-dimensional grid with cetral differce scheme
	FVM::Grid1D< FVM::CentDiffSchemeTag > grid( NumCtrlVol, CylinderLength);

	const double AmbientTemperature = 20.0,
			HotTemperature = 100.0;

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

	return 0;
}



