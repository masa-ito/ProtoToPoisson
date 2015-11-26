/*
 * CtrlVolGrid.hpp
 *
 *  Created on: 2015/11/10
 *      Author: Masakatsu ITO
 */

#ifndef FINITEVOLUMEMETHOD_GRID_HPP_
#define FINITEVOLUMEMETHOD_GRID_HPP_

#include <iostream>
#include <valarray>
#include <boost/proto/proto.hpp>


#include <DenseLinAlg/DenseLinAlg.hpp>

#include <FiniteVolumeMethod/BoundaryCondition.hpp>
#include <FiniteVolumeMethod/LazyDiscretizer.hpp>

namespace DLA = DenseLinAlg;


namespace FiniteVolumeMethod {

	struct CentDiffSchemeTag {};

	//
	// Control Volumes on a 1-Dimensional Grid
	//
	template < typename DiffSchemeTag >
	class Grid1D
	{
	private :
		const int numCtrlVol;
		const double startPos, regionLen, spacing;
		std::slice ctrlVolSlice;
		std::vector< DirichletCondition > dirichletConds;
		std::vector< NeumannCondition > neumannConds;

	public :
		explicit
		Grid1D( int controlVolumeNunmber,
				double regionLength, double startPosition = 0.0) :
				numCtrlVol( controlVolumeNunmber),
				startPos( startPosition), regionLen( regionLength),
				spacing( regionLength / controlVolumeNunmber),
				ctrlVolSlice( 0, controlVolumeNunmber, 1),
				regionLen( regionLength),
				dirichletConds(), neumannConds() {}

		int ctrlVolNum() const { return numCtrlVol; }
		int boundaryConditionNum() const
		{
			return dirichletConds.size() + neumannConds.size() ;
		}

		void addDirichletBoundary( int outerCtrlVolIdx,
									int innerCtrlVolIdx,
									double fixedValue)
		{
			dirichletConds.push_back(
					DirichletCondition( outerCtrlVolIdx,
										innerCtrlVolIdx,
										fixedValue, spacing / 2 ) );
		}

		void addNeumannBoundary( int outerCtrlVolIdx,
								int innerCtrlVolIdx,
								double fixedGradient = 0.0) {
			neumannConds.push_back(
					NeumannCondition( outerCtrlVolIdx,
								innerCtrlVolIdx,
								fixedGradient, spacing) );
		}

		LazyConstValueVectorizerOnGrid1D discretizeFunction(double val) const
		{
			return LazyConstValueVectorizerOnGrid1D(numCtrlVol, val);
		}

		LazyInverseBoundaryConditionOperatorDiscretizerOnGrid1D
		discretizeInverseBoundaryConditionOperator() const
		{
			return
				LazyInverseBoundaryConditionOperatorDiscretizerOnGrid1D(
						dirichletConds, neumannConds);
		}

		LazyBoundaryConditionOperatorDiscretizerOnGrid1DBoundary
		disretizeBoundaryConditionOperatorOnBoundary() const
		{
			return
				LazyBoundaryConditionOperatorDiscretizerOnGrid1DBoundary(
						dirichletConds, neumannConds, numCtrlVol);
		}

		template < typename OprT >
		LazyOperatorDiscretizerOnGrid1DBoundary< OprT >
		discretizeOperatorOnBoundary( const OprT & opr) const
		{
			return
				LazyOperatorDiscretizerOnGrid1DBoundary(
						dirichletConds, neumannConds, numCtrlVol, opr);
		}

		LazyBoundaryConditionSpecificationDiscretizerOnGrid1D
		discretizeBoundaryConditionSpecification() const
		{
			return
				LazyBoundaryConditionSpecificationDiscretizerOnGrid1D(
						dirichletConds, neumannConds);
		}

		template < typename OprT >
		LazyOperatorDiscretizerOnGrid1D< OprT >
		discretizeOperator( const OprT & opr) const
		{
			double eastBoundarySpacing = spacing,
					westBoundarySpacing = spacing;

			for (int di = 0; di < dirichletConds.size(); di++) {
				int ii = dirichletConds[ di].innerIndex(),
						oi = dirichletConds[ di].outerIndex();
				double s = dirichletConds[di].outerInnerSpacing();
				if ( ii == 0 &&  oi < 0 ) {
					westBoundarySpacing = s;
				} else if ( ii == numCtrlVol - 1 &&  oi >= numCtrlVol ) {
					eastBoundarySpacing = s;
				}
			}

			for (int ni = 0; ni < neumannConds.size(); ni++) {
				int ii = neumannConds[ ni].innerIndex(),
						oi = neumannConds[ ni].outerIndex();
				double s = neumannConds[ ni].outerInnerSpacing();
				if ( ii == 0 &&  oi < 0 ) {
					westBoundarySpacing = s;
				} else if ( ii == numCtrlVol - 1 &&  oi >= numCtrlVol ) {
					eastBoundarySpacing = s;
				}
			}


			return LazyOperatorDiscretizerOnGrid1D< OprT >(
					opr, westBoundarySpacing, spacing, eastBoundarySpacing,
					numCtrlVol);
		}

		void printForGnuPlot(const DLA::Vector & vec) const {
			// not yet implemented
		}
	};

}





#endif /* FINITEVOLUMEMETHOD_GRID_HPP_ */
