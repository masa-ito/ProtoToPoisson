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

namespace DLA = DenseLinAlg;



class HeatReleaseRateFunctor {
	private:
		const double hccCc, ambientTemperature;
public:
	HeatReleaseRateFunctor(double heatConvectiveCoeff,
			double cylinderCircumference,
			double _ambientTemperature) :
		hccCc(heatConvectiveCoeff * cylinderCircumference ),
		ambientTemperature(_ambientTemperature) {}

	double operator()(double temperature) {
		return -  hccCc * ( temperature - ambientTemperature);
	}
};

class ExactTemperatureDistributionFunc {
	private:
		const double n, cyliderLength, ambientTemperature,
			bufferTemperature;
	public:
		ExactTemperatureDistributionFunc(double heatConvectiveCoeff,
			double cylinderCircumFerence,
			double thermalConductivity, double cylinderXSecArea,
			double _ambientTemperature, double _bufferTemperature,
			double _cylinderLength) :
				n(sqrt(heatConvectiveCoeff * cylinderCircumFerence /
						(thermalConductivity * cylinderXSecArea ))),
				ambientTemperature(_ambientTemperature),
				bufferTemperature(_bufferTemperature),
				cylinderLength(_cylinderLenght) {}

		double operator()(double position) {
			return ambiendTemperature +
					(bufferTemperature - ambientTemperature) *
					cosh( n * (cylinderLength - position)) /
					cosh( n * cylinderLength) ;
		}
};

int main() {

	const int numCtrlVol = 100;
	const double cylinderXSecArea = 0.01,
			cylinderLength = 1.0;

	FieldVariable temp(numCtrlVol), heatSrc(numCtrlVol);
	DiscretizedOperator d2tdx2(numCtrlVol, numCtrlVol);

	CntrlVolRange allCtrlVols(simRegion.cntrlVolBegin(),
							simRegion.cntrlVolEnd());

	const double heatConvectiveCoeff = 1.0,
			cylinderCircumFerence = 2.0 * M_PI * 0.1,
			ambientTemperature = 298.15;
	HeatReleaseRateFunctor heatRate(heatConvectiveCoeff,
			cylinderCircumFerence, ambientTemperature);

	heatSrc( allCtrlVols ) = heatRate( temp(allCtrlVols) );

	CntrlVolFaceRange innerFaces(simRegion.faceBegin() + 1,
						simRegion.faceEnd() - 1);

	const double thermalConductivity = 1.0;
	d2tdx2( innerFaces ) += thermalConductivity *
			simRegion.ddx( innerFaces );

	CntrlVolFaceIterator leftmostFace = simRegion.faceBegin(),
			rightmostFace = simRegion.faceEnd() - 1;

	d2tdx2(leftmostFace) += thermalConductivity *
			simRegion.ddxDirichlet( innerFaces );

	// In this problem Neumann condition dt/dx = 0 is set
	// at the rightmost face. But the condition dt/dx
	// is automaticaly set in Finite Volume Method.
	// Let's thank FVM!!

	auto resid = heatSrc - d2tdx2 * temp;
	auto inversePrecond = 1.0 / d2tdx2.digonal( allCtrlVols );

	auto p = inversePrecond * resid;
	double rho = resid.dot(p);

	auto prevTemp = temp;
	auto q = d2tdx2 * p;
	double alpha = rho / p.dot(q) ;
	temp = prevTemp + alpha * p;

	auto prevHeatSrc = heatSrc;
	heatSrc( allCtrlVols ) = heatRate( temp(allCtrlVols) );

	auto prevResdid = resid;
	// I'm still not sure if the folowing is right...
	// Need to extend CG algorithm for this temperature-dependent
	// heat source term.
	resid = heatSrc - prevHeatSrc + prevResid - alpha * q;

	while ( abs(resid) / abs(heatSrc) /doulbe(numCtrlVol) > 1.0e-4 ) {

		auto z = inversePrecond * resid;

		double prevRho = rho;
		rho = resid.dot(p);

		double beta = rho / prevRho;
		auto prevP = p;
		p = z + beta * prevP;

		q = d2tdx2 * p;
		alpha = rho / p.dot(q) ;
		prevTemp = temp;
		temp = prevTemp + alpha * p;

		prevHeatSrc = heatSrc;
		heatSrc( allCtrlVols ) = heatRate( temp(allCtrlVols) );

		prevResdid = resid;
		// I'm still not sure if the folowing is right...
		// Need to extend CG algorithm for this temperature-dependent
		// heat source term.
		resid = heatSrc - prevHeatSrc + prevResid - alpha * q;
	}

	std::cout << "temperature distribution" << std::endl;
	std::cout << temp ;
	std::cout << endl;

	ExactTemperatureDistributionFunc
		analyticalaAnswer(heatConvectiveCoeff,
				cylinderCircumFerence,
				thermalConductivity, cylinderXSecArea,
				ambientTemperature, bufferTemperature,
				cylinderLength)
	FieldVarialbe exactTemp(numCtrlVol);

	exactTemp(allCtrlVols) =
			analyticalAnswer( simRegion.position(allCntrlVols) );

	std::cout << "Error in finite volume method calc." << std::endl;
	std::cout << abs(temp - exactTemp);
	std::cout << endl;

    return 0;
}
