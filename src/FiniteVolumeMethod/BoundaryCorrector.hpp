/*
 * BoundaryCorrector.hpp
 *
 *  Created on: 2015/11/23
 *      Author: Masakatsu ITO
 */

#ifndef FINITEVOLUMEMETHOD_BOUNDARYCORRECTOR_HPP_
#define FINITEVOLUMEMETHOD_BOUNDARYCORRECTOR_HPP_

#include <iostream>
#include <valarray>
#include <boost/proto/proto.hpp>

#include <DenseLinAlg/DenseLinAlg.hpp>


namespace DLA = DenseLinAlg;


namespace FiniteVolumeMethod {

	class BoundaryCorrector
	{
	private :
		DLA::DiagonalMatrix & inverseOuter;
		DLA::Matrix & outerInner, innerOuter;
		DLA::Vector & outerVec;

	public:
		template < typename GridT , typename OprT >
		explicit
		BoundaryCorrector( GridT & grid, const OprT & opr) :
			inverseOuter( grid.discretizeInverseBoundaryConditionOperator() ),
			outerInner( grid.disretizeBoundaryConditionOperatorOnBoundary() ),
			innerOuter( grid.discretizeOperatorOnBoundary( opr ) ),
			outerVec( grid.discretizeBoundaryConditionSpecification() ) {}

		void applyTo(DLA::Matrix & coeff) const
		{
			coeff -= innerOuter * inverseOuter * outerInner;
		}

		void applyTo(DLA::Vector & rhs) const
		{
			rhs -= inverseOuter * outerVec;
		}
	};

}



#endif /* FINITEVOLUMEMETHOD_BOUNDARYCORRECTOR_HPP_ */
