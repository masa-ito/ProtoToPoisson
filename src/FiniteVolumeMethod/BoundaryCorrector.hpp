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
		DLA::DiagonalMatrix inverseOuter;
		DLA::Matrix outerInner, innerOuter;
		DLA::Vector outerVec;

	public:
		template < typename GridT , typename OprT >
		explicit
		BoundaryCorrector( GridT & grid, const OprT & opr) :
			inverseOuter( grid.boundaryConditionNum() ),
			outerInner( grid.boundaryConditionNum(), grid.ctrlVolNum() ),
			innerOuter( grid.ctrlVolNum(), grid.boundaryConditionNum() ),
			outerVec( grid.boundaryConditionNum() )
		{
			inverseOuter = grid.discretizeInverseBoundaryConditionOperator();
			outerInner = grid.disretizeBoundaryConditionOperatorOnBoundary();
			innerOuter = grid.discretizeOperatorOnBoundary( opr );
			outerVec = grid.discretizeBoundaryConditionSpecification();

			// For debugging
			std::cout << "inverseOuter" << std::endl;
			int ri, ci;
			for (ri = 0; ri < inverseOuter.size(); ri++)
				std::cout << inverseOuter(ri) << " ";
			std::cout << std::endl;

			std::cout << "outerInner" << std::endl;
			for (ri = 0; ri < outerInner.rowSize(); ri++) {
				for (ci = 0; ci < outerInner.columnSize(); ci++)
					std::cout << outerInner(ri, ci) << " ";
				std::cout << std::endl;
			}

			std::cout << "innerOuter" << std::endl;
			for (ri = 0; ri < innerOuter.rowSize(); ri++) {
				for (ci = 0; ci < innerOuter.columnSize(); ci++)
					std::cout << innerOuter(ri, ci) << " ";
				std::cout << std::endl;
			}

			std::cout << "outerVec" << std::endl;
			for (ci = 0; ci < outerVec.size(); ci++)
				std::cout << outerVec(ci) << std::endl;
		}

		void applyTo(DLA::Matrix & coeff) const
		{
			coeff -= innerOuter * inverseOuter * outerInner;
		}

		void applyTo(DLA::Vector & rhs) const
		{
			DLA::Vector temp( inverseOuter.size() );
			temp = inverseOuter * outerVec;
			rhs -= innerOuter * temp;
		}
	};

}



#endif /* FINITEVOLUMEMETHOD_BOUNDARYCORRECTOR_HPP_ */
