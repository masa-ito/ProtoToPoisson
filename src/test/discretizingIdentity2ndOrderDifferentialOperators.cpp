/*
 * discretizingOperatorsInPoissonEq.cpp
 *
 *  Created on: 2016/02/01
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

	const double AnotherPrefactor = 0.1;

	//
	// Testging Identity Operator
	//
	FVM::IdentityOperatorType const IdOpr = FVM::IdentityOperatorType();

	std::cout <<
		"- AnotherPrefactor * ConvectiveHeatTransCoeff * Circumference" <<
		std::endl;
	std::cout <<
		- AnotherPrefactor * ConvectiveHeatTransCoeff * Circumference  <<
		std::endl;

	std::cout << std::endl <<
		"- AnotherPrefactor * ConvectiveHeatTransCoeff * Circumference * " <<
		"IdOpr" << std::endl;
	proto::display_expr(
		- AnotherPrefactor * ConvectiveHeatTransCoeff * Circumference *
		IdOpr
	);

	std::cout << std::endl <<
		"- AnotherPrefactor * ConvectiveHeatTransCoeff * Circumference * " <<
		"IdOpr( Spacing, Spacing)" << std::endl;
	std::cout <<
		FVM::ExprGrammar()(
			- AnotherPrefactor * ConvectiveHeatTransCoeff * Circumference *
			IdOpr( Spacing, Spacing)
		)  << std::endl;

	std::cout << std::endl <<
		"- AnotherPrefactor * ConvectiveHeatTransCoeff * Circumference * " <<
		"IdOpr( Spacing, FVM::FaceToEastTag()) )" << std::endl;
	std::cout <<
		FVM::ExprGrammar()(
			- AnotherPrefactor * ConvectiveHeatTransCoeff * Circumference *
			IdOpr( Spacing, FVM::FaceToEastTag())
		)  << std::endl;

	std::cout << std::endl <<
		"- AnotherPrefactor * ConvectiveHeatTransCoeff * Circumference * " <<
		"IdOpr( Spacing, FVM::FaceToWestTag()) )" << std::endl;
	std::cout <<
		FVM::ExprGrammar()(
			- AnotherPrefactor * ConvectiveHeatTransCoeff * Circumference *
			IdOpr( Spacing, FVM::FaceToWestTag())
		)  << std::endl;


	auto idOprCopy = proto::deep_copy(
			- AnotherPrefactor * ConvectiveHeatTransCoeff * Circumference *
			IdOpr
		);

	std::cout << std::endl <<
		"proto::deep_copy( ... )( Spacing, Spacing)" << std::endl;
	std::cout << FVM::ExprGrammar()( idOprCopy( Spacing, Spacing) )  <<
		std::endl;

	std::cout <<
		"proto::deep_copy( ... )( Spacing, FVM::FaceToEastTag())" <<
		std::endl;
	std::cout <<
		FVM::ExprGrammar()( idOprCopy( Spacing, FVM::FaceToEastTag()) )  <<
		std::endl;

	std::cout <<
		"proto::deep_copy( ... )( Spacing, FVM::FaceToWestTag())" <<
		std::endl;
	std::cout <<
		FVM::ExprGrammar()( idOprCopy( Spacing, FVM::FaceToWestTag()) )  <<
		std::endl;


	//
	// Testging Differential Operator
	//
	FVM::DifferentialOperatorType const DiffOpr =
								FVM::DifferentialOperatorType();

	std::cout <<
		"AnotherPrefactor * ThermalConductivity * Area" <<
		std::endl;
	std::cout <<
		AnotherPrefactor * ThermalConductivity * Area  <<
		std::endl;

	std::cout << std::endl <<
		"AnotherPrefactor * ThermalConductivity * Area * " <<
		"DiffOpr * DiffOpr" << std::endl;
	proto::display_expr(
		AnotherPrefactor * ThermalConductivity * Area *
		( DiffOpr * DiffOpr )
	);


	std::cout << std::endl <<
		"AnotherPrefactor * ThermalConductivity * Area * " <<
		"( DiffOpr * DiffOpr)( Spacing, Spacing)" << std::endl;
	std::cout <<
		FVM::ExprGrammar()(
			AnotherPrefactor * ThermalConductivity * Area *
			( DiffOpr * DiffOpr )( Spacing, Spacing)
		)  << std::endl;

	std::cout << std::endl <<
		"AnotherPrefactor * ThermalConductivity * Area * " <<
		"( DiffOpr * DiffOpr )( Spacing, FVM::FaceToEastTag()) )" << std::endl;
	std::cout <<
		FVM::ExprGrammar()(
			AnotherPrefactor * ThermalConductivity * Area *
			( DiffOpr * DiffOpr )( Spacing, FVM::FaceToEastTag())
		)  << std::endl;

	std::cout << std::endl <<
		"AnotherPrefactor * ThermalConductivity * Area * " <<
		"( DiffOpr * DiffOpr )( Spacing, FVM::FaceToWestTag()) )" << std::endl;
	std::cout <<
		FVM::ExprGrammar()(
			AnotherPrefactor * ThermalConductivity * Area *
			( DiffOpr * DiffOpr )( Spacing, FVM::FaceToWestTag())
		)  << std::endl;

	//
	//  A grammar for ( _ * ( DiffOpr * DiffOpr) )( _, _ )
	//  is NOT YET defined.
    //
	// auto diffOprCopy = proto::deep_copy(
	//		AnotherPrefactor * ThermalConductivity * Area *
	//		( DiffOpr * DiffOpr )
	//	);

	auto diffOprCopy = proto::deep_copy(
			AnotherPrefactor * ThermalConductivity * Area *
			 DiffOpr * DiffOpr
		);
	std::cout << std::endl << "proto::deep_copy( ... )" << std::endl;
	proto::display_expr( diffOprCopy);

	std::cout << std::endl <<
		"proto::deep_copy( ... )( Spacing, Spacing)" << std::endl;
	proto::display_expr( diffOprCopy( Spacing, Spacing) );

	std::cout << FVM::ExprGrammar()( diffOprCopy( Spacing, Spacing) )  <<
		std::endl;

	std::cout <<
		"proto::deep_copy( ... )( Spacing, FVM::FaceToEastTag())" <<
		std::endl;
	std::cout <<
		FVM::ExprGrammar()( diffOprCopy( Spacing, FVM::FaceToEastTag()) )  <<
		std::endl;

	std::cout <<
		"proto::deep_copy( ... )( Spacing, FVM::FaceToWestTag())" <<
		std::endl;
	std::cout <<
		FVM::ExprGrammar()( diffOprCopy( Spacing, FVM::FaceToWestTag()) )  <<
		std::endl;

	return 0;
}

