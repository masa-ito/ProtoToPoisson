/*
 * discretizingLinCombOfIdAnd2ndDiffOperators.cpp
 *
 *  Created on: 2016/02/02
 *      Author: Masakatsu ITO
 */

#include <math.h>

#include <iostream>
#include <boost/proto/proto.hpp>

// #include <DenseLinAlg/DenseLinAlg.hpp>
// #include <SparseLinAlg/SparseLinAlg.hpp>

// #include <FiniteVolumeMethod/FiniteVolumeMethod.hpp>
#include <FiniteVolumeMethod/Grammar.hpp>
#include <FiniteVolumeMethod/Expression.hpp>


namespace mpl = boost::mpl;
namespace proto = boost::proto;

namespace DLA = DenseLinAlg;
// namespace SLA = SparseLinAlg;
namespace FVM = FiniteVolumeMethod;


int main()
{
	const int NumCtrlVol = 5;
	const double CylinderLength = 1.0;

	const double AmbientTemperature = 298.15,
			HotTemperature = AmbientTemperature + 100.0;

	const double ThermalConductivity = 1000.0;

	// http://www.engineeringtoolbox.com/convective-heat-transfer-d_430.html
	const double ConvectiveHeatTransCoeff = 0.7;

	const double CylinderRadius = 2.0 * ConvectiveHeatTransCoeff /
													ThermalConductivity,
				Area = M_PI * CylinderRadius * CylinderRadius,
				Circumference = 2.0 * M_PI *  CylinderRadius;

	const double Spacing = 0.1;
	std::cout << "Spacing = " << Spacing << std::endl;

	FVM::IdentityOperatorType const IdOpr = FVM::IdentityOperatorType();
	FVM::DifferentialOperatorType const DiffOpr =
								FVM::DifferentialOperatorType();

	std::cout << std::endl <<
		"ThermalConductivity * Area * " <<
			"( DiffOpr * DiffOpr)( Spacing, Spacing)" << std::endl <<
		"- ConvectiveHeatTransCoeff * Circumference * " <<
			"IdOpr( Spacing, Spacing)" <<
		std::endl;
	proto::display_expr(
		ThermalConductivity * Area * ( DiffOpr * DiffOpr )( Spacing, Spacing)
		- ConvectiveHeatTransCoeff * Circumference * IdOpr( Spacing, Spacing)
	);
	std::cout << FVM::ExprGrammar()(
		ThermalConductivity * Area * ( DiffOpr * DiffOpr )( Spacing, Spacing)
		- ConvectiveHeatTransCoeff * Circumference * IdOpr( Spacing, Spacing)
	) << std::endl;


	// The operator part in the heat equation
	// which consists of a differential operator and identical one.
	auto opr = proto::deep_copy(
				ThermalConductivity * Area * DiffOpr * DiffOpr
				- ConvectiveHeatTransCoeff * Circumference * IdOpr );

	std::cout << std::endl <<
		"proto::deep_copy(" << std::endl <<
		"	ThermalConductivity * Area * DiffOpr * DiffOpr" << std::endl <<
		"	- ConvectiveHeatTransCoeff * Circumference * IdOpr )" <<
		std::endl;
	proto::display_expr( opr );

	std::cout << std::endl <<
		"proto::deep_copy( ... )( Spacing, Spacing)" << std::endl;
	proto::display_expr( opr( Spacing, Spacing) );


	std::cout << FVM::ExprGrammar()( opr( Spacing, Spacing) )  <<
		std::endl;

	std::cout <<
		"proto::deep_copy( ... )( Spacing, FVM::FaceToEastTag())" <<
		std::endl;
	std::cout <<
		FVM::ExprGrammar()( opr( Spacing, FVM::FaceToEastTag()) )  <<
		std::endl;

	std::cout <<
		"proto::deep_copy( ... )( Spacing, FVM::FaceToWestTag())" <<
		std::endl;
	std::cout <<
		FVM::ExprGrammar()( opr( Spacing, FVM::FaceToWestTag()) )  <<
		std::endl;

	return 0;
}

