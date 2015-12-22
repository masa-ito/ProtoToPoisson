/*
 * discretizingIdentyOperator.cpp
 *
 *  Created on: 2015/12/07
 *      Author: Masakatsu ITO
 */

#include <math.h>

#include <iostream>
#include <boost/proto/proto.hpp>

// #include <DenseLinAlg/DenseLinAlg.hpp>
// #include <SparseLinAlg/SparseLinAlg.hpp>

// #include <FiniteVolumeMethod/FiniteVolumeMethod.hpp>
#include "discretizingIdentyOperator.hpp"

// namespace DLA = DenseLinAlg;
// namespace SLA = SparseLinAlg;
namespace FVM = FiniteVolumeMethod;

int main() {

	const int NumCtrlVol = 5;
	const double CylinderLength = 1.0;

	// 1-dimensional grid with cetral differce scheme
	// FVM::Grid1D< FVM::CentDiffSchemeTag > grid( NumCtrlVol, CylinderLength);

	const double AmbientTemperature = 298.15,
			HotTemperature = AmbientTemperature + 100.0;

	// grid.addDirichletBoundary(-1, 0, AmbientTemperature);
	// grid.addNeumannBoundary( NumCtrlVol - 1, NumCtrlVol, 0.0);

	const double ThermalConductivity = 1000.0;

	// http://www.engineeringtoolbox.com/convective-heat-transfer-d_430.html
	const double ConvectiveHeatTransCoeff = 0.7;

	const double CylinderRadius = 2.0 * ConvectiveHeatTransCoeff /
													ThermalConductivity,
				Area = M_PI * CylinderRadius * CylinderRadius,
				Circumference = 2.0 * M_PI *  CylinderRadius;

	// The operator part in the heat equation
    // which consists of a differential operator and identical one.
	auto opr = proto::deep_copy(
			// - ConvectiveHeatTransCoeff * Circumference * FVM::identityOpr
			FVM::identityOpr
			);

	// DLA::Matrix coeffMat = grid.discretizeOperator( opr );

	return 0;
}
