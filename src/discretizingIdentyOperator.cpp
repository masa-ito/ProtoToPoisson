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

	const double AmbientTemperature = 298.15,
			HotTemperature = AmbientTemperature + 100.0;

	const double ThermalConductivity = 1000.0;

	// http://www.engineeringtoolbox.com/convective-heat-transfer-d_430.html
	const double ConvectiveHeatTransCoeff = 0.7;

	const double CylinderRadius = 2.0 * ConvectiveHeatTransCoeff /
													ThermalConductivity,
				Area = M_PI * CylinderRadius * CylinderRadius,
				Circumference = 2.0 * M_PI *  CylinderRadius;

	struct foo_tag {};
	proto::terminal< foo_tag >::type const foo = {};
	3.0 * foo;

	FVM::IdentityOperatorType const IdOpr = FVM::IdentityOperatorType();

	proto::display_expr( 3.0 * IdOpr );
	proto::display_expr( IdOpr( 0.1, 0.1) );
	proto::display_expr( 3.0 * IdOpr( 0.1, 0.1) );
	// proto::display_expr( FVM::ExprGrammar()( 3.0 * IdOpr( 0.1, 0.1) ) );
	std::cout << FVM::ExprGrammar()( IdOpr( 0.1, 0.1) )  << std::endl;
	std::cout << FVM::ExprGrammar()( 3.0 * IdOpr( 0.1, 0.1) ) << std::endl;

	proto::display_expr( Circumference  * IdOpr( 0.1, 0.1) );

	std::cout << FVM::ExprGrammar()(
			- ConvectiveHeatTransCoeff *
			Circumference *
			IdOpr( 0.1, 0.1) )
			<< std::endl;

	auto opr = proto::deep_copy(
			- ConvectiveHeatTransCoeff * Circumference * IdOpr
			);

	proto::display_expr( opr);
	proto::display_expr( opr( 0.1, 0.1) );
	// proto::display_expr( FVM::ExprGrammar()( opr( 0.1, 0.1) ) );

	double ans = FVM::ExprGrammar()( opr( 0.1, 0.1) ) ;
	std::cout << ans << std::endl;

	std::cout << - ConvectiveHeatTransCoeff * Circumference << std::endl;

	return 0;
}
