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

#include <FiniteVolumeMethod/Grammar.hpp>
#include <FiniteVolumeMethod/Expression.hpp>
#include <FiniteVolumeMethod/BoundaryCondition.hpp>

namespace DLA = DenseLinAlg;


namespace FiniteVolumeMethod {

	struct LazyConstValueVectorizerOnGrid1D :
			DLA::LazyVectorMaker< LazyConstValueVectorizerOnGrid1D >
	{
		const double val;

		explicit
		LazyConstValueVectorizerOnGrid1D(int size, double val_) :
		DLA::LazyVectorMaker< LazyConstValueVectorizerOnGrid1D >(size),
		val(val_) {}

		void assignDataTo_derived( DLA::Vector& lhs) const
		{
			for (int i = 0; i < sz; i++) lhs(i) = val;
		}
	};

	struct LazyInverseBoundaryConditionOperatorDiscretizerOnGrid1D :
		DLA::LazyDiagonalMatrixMaker<
				LazyInverseBoundaryConditionOperatorDiscretizerOnGrid1D >
	{
		const int dirichletCondNum;
		const std::vector< NeumannCondition > & neumannConds;

		explicit
		LazyInverseBoundaryConditionOperatorDiscretizerOnGrid1D(
				const std::vector< DirichletCondition > & dirichletConds_,
				const std::vector< NeumannCondition > & neumannConds_) :
				DLA::LazyDiagonalMatrixMaker<
					LazyInverseBoundaryConditionOperatorDiscretizerOnGrid1D
				>( dirichletConds_.size() + neumannConds_.size() ),
				dirichletCondNum( dirichletConds_.size()),
				neumannConds( neumannConds_ ) {}

		void assignDataTo_derived( DLA::DiagonalMatrix& lhs) const
		{
			for (int i = 0; i < dirichletCondNum; i++) lhs(i) = 1.0;

			for (int i = 0; i < neumannConds.size(); i++)
				lhs( dirichletCondNum + i) =
						1.0 / neumannConds[i].outerInnerSpacing();
		}
	};

	struct LazyBoundaryConditionOperatorDiscretizerOnGrid1DBoundary
	: DLA::LazyMatrixMaker<
	  	  LazyBoundaryConditionOperatorDiscretizerOnGrid1DBoundary >
	{
		const std::vector< DirichletCondition > & dirichletConds;
		const std::vector< NeumannCondition > & neumannConds;

		explicit
		LazyBoundaryConditionOperatorDiscretizerOnGrid1DBoundary(
				const std::vector< DirichletCondition > & dirichletConds_,
				const std::vector< NeumannCondition > & neumannConds_,
				int numCtrlVol_) :
			DLA::LazyMatrixMaker<
				LazyBoundaryConditionOperatorDiscretizerOnGrid1DBoundary
			>( dirichletConds_.size() + neumannConds_.size(), numCtrlVol_),
			dirichletConds( dirichletConds_),
			neumannConds( neumannConds_) {}

		void assignDataTo_derived( DLA::Matrix& lhs) const
		{
			// Zero-clear for the all matrix elements
			for ( int ri = 0; ri < rowSz; ri++)
				for ( int ci = 0; ci < colSz; ci++)
					lhs( ri, ci) = 0.0;

			// For Neumann conditions
			for ( int i = 0; i < neumannConds.size(); i++)
				lhs( i + dirichletConds.size(), neumannConds[ i].innerIndex() )
					= 1.0 /  neumannConds[ i].outerInnerSpacing() ;
		}
	};

	template < typename OprT >
	struct LazyOperatorDiscretizerOnGrid1DBoundary
	: DLA::LazyMatrixMaker<
	  	  LazyOperatorDiscretizerOnGrid1DBoundary< OprT > >
	 {
		std::vector< DirichletCondition > & dirichletConds;
		std::vector< NeumannCondition > & neumannConds;
		const OprT & opr;

		explicit
		LazyOperatorDiscretizerOnGrid1DBoundary(
				const std::vector< DirichletCondition > & dirichletConds_,
				const std::vector< NeumannCondition > & neumannConds_,
				int numCtrlVol_, const OprT & opr_) :
			DLA::LazyMatrixMaker<
				LazyOperatorDiscretizerOnGrid1DBoundary< OprT >
			>( dirichletConds_.size() + neumannConds_.size(), numCtrlVol_),
			dirichletConds( dirichletConds_),
			neumannConds( neumannConds_),
			opr( opr_) {}

		void assignDataTo_derived( DLA::Matrix& lhs) const
		{
			// Zero-clear for the all matrix elements
			for ( int ri = 0; ri < this->rowSz; ri++)
				// g++ 4.8.4 does not allow me to omit 'this'
				// from 'this->rowSz', althoug rowSz is
				// a data member of the base class.
				for ( int ci = 0; ci < this->colSz; ci++)
					lhs( ri, ci) = 0.0;

			// proto::_default<> trans;

			// For Dirichlet conditions
			for ( int ci = 0; ci < dirichletConds.size(); ci++) {
				int ri = dirichletConds[ ci].innerIndex(),
						oi = dirichletConds[ ci].outerIndex();
				double spacing = dirichletConds[ ci].outerInnerSpacing();

				if ( ri < oi ) {
					lhs( ri, ci) = ExprGrammar()(
										opr( spacing, FaceToEastTag() ) );
				} else {
					lhs( ri, ci) = ExprGrammar()(
										opr( spacing, FaceToWestTag() ) );
				}
			}

			// For Neumann conditions
			for ( int i = 0; i < neumannConds.size(); i++) {
				int ri = neumannConds[ i].innerIndex(),
						oi = neumannConds[ i].outerIndex();
				double spacing = neumannConds[ i].outerInnerSpacing();

				if ( ri < oi ) {
					lhs( ri, i + dirichletConds.size())
							= ExprGrammar()(
									opr( spacing, FaceToEastTag() ) );
				} else {
					lhs( ri, i + dirichletConds.size())
							= ExprGrammar()(
									opr( spacing, FaceToWestTag() ) );
				}
			}
		}
	 };

	struct LazyBoundaryConditionSpecificationDiscretizerOnGrid1D
	: DLA::LazyVectorMaker<
	  	  LazyBoundaryConditionSpecificationDiscretizerOnGrid1D >
	{
		const std::vector< DirichletCondition > & dirichletConds;
		const std::vector< NeumannCondition > & neumannConds;

		explicit
		LazyBoundaryConditionSpecificationDiscretizerOnGrid1D(
				const std::vector< DirichletCondition > & dirichletConds_,
				const std::vector<NeumannCondition>& neumannConds_) :
				DLA::LazyVectorMaker<
					LazyBoundaryConditionSpecificationDiscretizerOnGrid1D
				>( dirichletConds_.size() + neumannConds_.size() ),
				dirichletConds( dirichletConds_),
				neumannConds( neumannConds_) {}

		void assignDataTo_derived( DLA::Vector& lhs) const
		{
			for (int ci = 0; ci < dirichletConds.size(); ci++)
				lhs( ci) = dirichletConds[ ci].fixedValue();

			for (int i = 0; i < neumannConds.size(); i++)
				lhs( i + dirichletConds.size())
					= neumannConds[i].fixedGradient();
		}
	};

	template < typename OprT >
	struct LazyOperatorDiscretizerOnGrid1D
	: DLA::LazyMatrixMaker< LazyOperatorDiscretizerOnGrid1D< OprT > >
	{
		const OprT & opr;
		const double westBoundarySpacing, spacing, eastBoundarySpacing;

		explicit
		LazyOperatorDiscretizerOnGrid1D( const OprT & opr_,
										double westBoundarySpacing_,
										double spacing_,
										double eastBoundarySpacing_,
										int numCtrlVol) :
			DLA::LazyMatrixMaker<
				LazyOperatorDiscretizerOnGrid1D< OprT >
			>( numCtrlVol, numCtrlVol),
			opr( opr_),
			westBoundarySpacing( westBoundarySpacing_),
			eastBoundarySpacing( eastBoundarySpacing_),
			spacing( spacing_) {}

		void assignDataTo_derived( DLA::Matrix& lhs) const
		{
			int ri;

			// Zero-clear for the all matrix elements
			for (ri = 0; ri < this->rowSz; ri++)
				// g++ 4.8.4 does not allow me to omit 'this'
				// from 'this->rowSz', althoug rowSz is
				// a data member of the base class.
				// colSz == rowSz
				for ( int ci = 0; ci < this->colSz; ci++) lhs( ri, ci) = 0.0;

			// proto::_default<> trans;

			lhs( 0, 0) = ExprGrammar()(
							opr( westBoundarySpacing, spacing) );
			for (ri = 1; ri < this->rowSz - 1; ri++)
				lhs( ri, ri) = ExprGrammar()(
											opr( spacing, spacing) );
			lhs( this->rowSz - 1, this->rowSz - 1) = ExprGrammar()(
							opr( spacing, eastBoundarySpacing) );

			lhs( 0, 1) = ExprGrammar()(
					opr( spacing, FaceToEastTag() ) );
			for (ri = 1; ri < this->rowSz - 1; ri++) {
				lhs( ri, ri - 1) = ExprGrammar()(
						opr( spacing, FaceToWestTag() ) );
				lhs( ri, ri + 1) = ExprGrammar()(
						opr( spacing, FaceToEastTag() ) );
			}
			lhs( this->rowSz - 1, this->rowSz - 2) = ExprGrammar()(
					opr( spacing, FaceToWestTag() ) );

			/* for (ri = 0; ri < this->rowSz; ri++) {
				int ci;
				for (ci = 0; ci < ri; ci++)
					lhs( ri, ci)
						= ExprGrammar()(
								opr( spacing, FaceToWestTag() ) );

				// colSz == rowSz
				for (ci = ri + 1;  ci < this->rowSz; ci++)
					lhs( ri, ci)
						= ExprGrammar()(
								opr( spacing, FaceToEastTag() ) );
			}*/
		}
	};

}


#endif /* FINITEVOLUMEMETHOD_LAZYDISCRETIZER_HPP_ */
