/*
 * LazyDiscretizer.hpp
 *
 *  Created on: 2015/11/24
 *      Author: Masakatsu ITO
 */

#ifndef FINITEVOLUMEMETHOD_LAZYDISCRETIZER_HPP_
#define FINITEVOLUMEMETHOD_LAZYDISCRETIZER_HPP_

#include <iostream>
#include <valarray>
#include <boost/proto/proto.hpp>


#include <DenseLinAlg/DenseLinAlg.hpp>

#include <FiniteVolumeMethod/BoundaryCondition.hpp>
#include <FiniteVolumeMethod/Grammar.hpp>

namespace DLA = DenseLinAlg;


namespace FiniteVolumeMethod {

	struct LazyConstValueVectorizerOnGrid1D : DLA::LazyVectorMaker
	{
		double double val;

		explicit
		LazyConstValueVectorizerOnGrid1D(int size, double val_) :
		DLA::LazyVectorMaker(size) {}

		virtual void assignDataTo(DLA::Vector& lhs) const
		{
			for (int i = 0; i < sz; i++) lhs(i) = val;
		}
	};

	struct LazyInverseBoundaryConditionOperatorDiscretizerOnGrid1D
	: DLA::LazyDiagonalMatrixMaker
	{
		const int direchletCondNum;
		std::vector< NeumannCondition > & neumannConds;

		explicit
		LazyInverseBoundaryConditionOperatorDiscretizerOnGrid1D(
				dirichletConds_, neumannConds_) :
				LazyDiagonalMatrixMaker( dirichletConds_.size() +
										neumannConds_.size() ),
				dirichletCondNum( dirichletConds_.size()),
				neumannConds( neumannConds_ ) {}

		virtual void assignDataTo(DLA::DiagonalMatrix& lhs) const
		{
			for (int i = 0; i < dirichletCondNum; i++) lhs(i) = 1.0;

			for (int i = 0; i < neumannConds.size(); i++)
				lhs(i) = 1.0 / neumannConds[i].outerInnerSpacing();
		}
	};

	struct LazyBoundaryConditionOperatorDiscretizerOnGrid1DBoundary
	: DLA::LazyMatrixMaker
	{
		std::vector< DirichletCondition > & dirichletConds;
		std::vector< NeumannCondition > & neumannConds;

		explicit
		LazyBoundaryConditionOperatorDiscretizerOnGrid1DBoundary(
				const std::vector< DirichletCondition > & dirichletConds_,
				const std::vector< NeumannCondition > & neumannConds_,
				int numCtrlVol_) :
			LazyMatrixMaker( dirichletConds_.size() + neumannConds_.size(),
							numCtrlVol_),
			dirichletConds( dirichletConds_),
			neumannConds( neumannConds_) {}

		virtual void assignDataTo(Matrix& lhs) const
		{
			// Zero-clear for the all matrix elements
			for ( int ri = 0; ri < rowSz; ri++)
				for ( int ci = 0; ci < colSz; ci++) lhs( ri, ci) = 0.0;

			// For Neumann conditions
			for ( int i = 0; i < neumannConds.size(); i++)
				lhs( i + dirichletConds.size(), neumannConds[ i].innerIndex)
					= 1.0 /  neumannConds[ i].outerInnerSpacing() ;
		}
	};

	template < typename OprT >
	struct LazyOperatorDiscretizerOnGrid1DBoundary
	: DLA::LazyMatrixMaker
	 {
		std::vector< DirichletCondition > & dirichletConds;
		std::vector< NeumannCondition > & neumannConds;
		const OprT & opr;

		explicit
		LazyOperatorDiscretizerOnGrid1DBoundary(
				const std::vector< DirichletCondition > & dirichletConds_,
				const std::vector< NeumannCondition > & neumannConds_,
				int numCtrlVol_, const OprT & opr_) :
			LazyMatrixMaker( dirichletConds_.size() + neumannConds_.size(),
							 numCtrlVol_),
			dirichletConds( dirichletConds_),
			neumannConds( neumannConds_),
			opr( opr_) {}

		virtual void assignDataTo(Matrix& lhs) const
		{
			// Zero-clear for the all matrix elements
			for ( int ri = 0; ri < rowSz; ri++)
				for ( int ci = 0; ci < colSz; ci++) lhs( ri, ci) = 0.0;

			proto::_default<> trans;

			// For Dirichlet conditions
			for ( int ci = 0; ci < dirichletConds.size(); ci++) {
				int ri = dirichletConds[ i].innerIndex(),
						oi = dirichletConds[ i].outerIndex();
				double spacing = dirichletConds[ i].outerInnerSpacing();

				if ( ri < oi ) {
					lhs( ri, ci) = trans( OprExprGrammar()(
										opr( spacing, FaceToEastTag() ) ) );
				} else {
					lhs( ri, ci) = trans( OprExprGrammar()(
										opr( spacing, FaceToWestTag() ) ) );
				}
			}

			// For Neumann conditions
			for ( int i = 0; i < neumannConds.size(); i++) {
				int ri = neumannConds[ i].innerIndex(),
						oi = neumannConds[ i].outerIndex();
				double spacing = neumannConds[ i].outerInnerSpacing();

				if ( ri < oi ) {
					lhs( ri, i + dirichletConds.size())
							= trans( OprExprGrammar()(
									opr( spacing, FaceToEastTag() ) ) );
				} else {
					lhs( ri, i + dirichletConds.size())
							= trans( OprExprGrammar()(
									opr( spacing, FaceToWestTag() ) ) );
				}
			}
		}
	 };

	struct LazyBoundaryConditionSpecificationDiscretizerOnGrid1D
	: DLA::LazyVectorMaker
	{
		std::vector< DirichletCondition > & dirichletConds;
		std::vector< NeumannCondition > & neumannConds;

		explicit
		LazyBoundaryConditionSpecificationDiscretizerOnGrid1D(
				const std::vector< DirichletCondition > & dirichletConds_,
				const std::vector<NeumannCondition>& neumannConds_) :
				DLA::LazyVectorMaker( dirichletConds_.size()
										+ neumannConds_.size() ),
				dirichletConds( dirichletConds_),
				neumannConds( neumannConds_) {}

		virtual void assignDataTo(DLA::Vector& lhs) const
		{
			for (int ci = 0; ci < dirichletConds.size(); ci++)
				lhs( ci) = dirichletConds[ i].fixedValue();

			for (int i = 0; i < neumannConds.size(); i++)
				lhs( i + dirichletConds.size())
					= neumannConds[i].fixedGradient();
		}
	 };

	template < typename OprT >
	struct LazyOperatorDiscretizerOnGrid1D
	: DLA::LazyMatrixMaker
	{
		const OprT & opr;
		const double westBoundarySpacing, spacing, eastBoundarySpacing;

		explicit
		LazyOperatorDiscretizerOnGrid1D( const OprT & opr_,
										double westBoundarySpacing_,
										double spacing_,
										double eastBoundarySpacing_,
										int numCtrlVol) :
			LazyMatrixMaker( numCtrlVol, numCtrlVol),
			opr( opr_),
			westBoundarySpacing( westBoundarySpacing_),
			eastBoundarySpacing( eastBoundarySpacing_),
			spacing( spacing_) {}

		virtual void assignDataTo(Matrix& lhs) const
		{
			int ri;

			// Zero-clear for the all matrix elements
			for (ri = 0; ri < rowSz; ri++)
				// colSz == rowSz
				for ( int ci = 0; ci < rowSz; ci++) lhs( ri, ci) = 0.0;

			proto::_default<> trans;

			lhs( 0, 0) = trans( OprExprGrammar()(
							opr( westBoundarySpacing, spacing) ) );
			for (ri = 1; ri < rowSz - 1; ri++)
				lhs( ri, ri) = trans( OprExprGrammar()(
											opr( spacing, spacing) ) );
			lhs( rowSz - 1, rowSz - 1) = trans( OprExprGrammar()(
							opr( spacing, eastBoundarySpacing) ) );


			for (ri = 0; ri < rowSz; ri++) {
				int ci;
				for (ci = 0; ci < ri; ci++)
					lhs( ri, ci)
						= trans( OprExprGrammar()(
								opr( spacing, FaceToWestTag() ) ) );

				// colSz == rowSz
				for (ci = ri + 1;  ci < rowSz; ci++)
					lhs( ri, ci)
						= trans( OprExprGrammar()(
								opr( spacing, FaceToEastTag() ) ) );
			}
		}
	};

}


#endif /* FINITEVOLUMEMETHOD_LAZYDISCRETIZER_HPP_ */
