/*
 * airCooledCylinder.hpp
 *
 *  Created on: 2016/03/23
 *      Author: Masakatsu ITO
 */

#ifndef AIRCOOLEDCYLINDER_HPP_
#define AIRCOOLEDCYLINDER_HPP_

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


class ExactTempDist
{
private:
	const double CylinderLength, AmbientTemperature, HotTemperature;
	const double n;

public:
	explicit ExactTempDist(double CylinderLength_,
			double AmbientTemperature_, double HotTemperature_,
			double ThermalConductivity, double Area,
			double ConvectiveHeatTransCoeff, double Circumference) :
			CylinderLength( CylinderLength_),
			AmbientTemperature( AmbientTemperature_),
			HotTemperature( HotTemperature_),
			n( sqrt( ConvectiveHeatTransCoeff * Circumference /
							( ThermalConductivity * Area ) ) )
	{}

	double operator()( double x) const {
		return ( AmbientTemperature +
				(HotTemperature - AmbientTemperature ) *
				 cosh( n * ( CylinderLength - x ) ) /
				 cosh( n * CylinderLength ) );
	}
};


const double CylinderLength = 1.0;

const double AmbientTemperature = 20.0,
		HotTemperature = 100.0;

const double ThermalConductivity = 1000.0;

// http://www.engineeringtoolbox.com/convective-heat-transfer-d_430.html
const double ConvectiveHeatTransCoeff = 0.7;

const double CylinderRadius = 2.0 / 25.0 * ConvectiveHeatTransCoeff /
												ThermalConductivity,
			Area = M_PI * CylinderRadius * CylinderRadius,
			Circumference = 2.0 * M_PI *  CylinderRadius;

const double convergenceCriterion = 1.0e-7;

void printConstants()
{
	std::cout << "ThermalConductivity * Area = " <<
			ThermalConductivity * Area << std::endl;

	std::cout << "- ConvectiveHeatTransCoeff * Circumference = " <<
			- ConvectiveHeatTransCoeff * Circumference << std::endl;

	std::cout << "ConvectiveHeatTransCoeff * Circumference * " <<
			"AmbientTemperature = " <<
			ConvectiveHeatTransCoeff * Circumference * AmbientTemperature <<
			std::endl;
}


template < typename MatrixType >
void printCoefficients( const MatrixType & coeffMat, double scale)
{
	int ri, ci;

	std::cout << "Coefficient matrix after applying boundary conditions" <<
			std::endl;
	std::cout << " / scale" << std::endl;
	for (ri = 0; ri < coeffMat.rowSize(); ri++)
	{
		for (ci = 0; ci < coeffMat.columnSize(); ci++)
			std::cout << std::setw(11) << std::fixed << std::setprecision(4) <<
				coeffMat(ri, ci) / scale;
		std::cout << std::endl;
	}

}

template < typename VectorType >
void printRHS( const VectorType & rhsVec, double scale)
{
	std::cout << std::endl;
	std::cout << "RHS vector after applying boundary conditions" <<
			std::endl;
	std::cout << " / scale" << std::endl;
	for (int ci = 0; ci < rhsVec.columnSize(); ci++)
		std::cout << std::setw(11) << std::fixed << std::setprecision(4) <<
						rhsVec( ci) / scale << std::endl;
}


template < typename VectorType >
void printCalculatedAndExactTemperatureDistributions(
		const VectorType & temperature)
{
	ExactTempDist exactDist(CylinderLength,
				AmbientTemperature, HotTemperature,
				ThermalConductivity, Area,
				ConvectiveHeatTransCoeff, Circumference);

	std::cout << "# stationary temperature distribution" << std::endl;
	std::cout << "#     x      FVM       exact " << std::endl;

	for (int ci =0; ci < temperature.columnSize(); ci++) {
		double x = CylinderLength / temperature.columnSize() * (ci + 0.5);
		std::cout << std::setw( 8) << std::fixed << std::setprecision(2) << x;
		std::cout << std::setw(10) << std::fixed << std::setprecision(3) <<
				temperature( ci);
		std::cout << std::setw(10) << std::fixed << std::setprecision(3) <<
				exactDist( x) << std::endl;
	}
}


#endif /* SAIRCOOLEDCYLINDER_HPP_ */
