/*
 * Expression.hpp
 *
 *  Created on: 2015/11/25
 *      Author: Masakatsu ITO
 */

#ifndef FINITEVOLUMEMETHOD_EXPRESSION_HPP_
#define FINITEVOLUMEMETHOD_EXPRESSION_HPP_

#include <iostream>
#include <valarray>
#include <boost/proto/proto.hpp>

#include <FiniteVolumeMethod/Grammar.hpp>


namespace FiniteVolumeMethod {

	// A wrapper for a linear algebraic expression
	template<typename E> struct ExprWrapper;

	// The above grammar is associated with this domain.
	struct Domain
		: proto::domain<proto::generator<ExprWrapper>, ExprGrammar>
	{};

	// A wrapper template for linear algebraic expressions
	// including matrices and vectors
	template<typename ExprType>
	struct ExprWrapper
		: proto::extends<ExprType, ExprWrapper<ExprType>, Domain>
	{
		/* typedef double result_type; */

		explicit ExprWrapper(const ExprType& e)
			: proto::extends<ExprType, ExprWrapper<ExprType>, Domain>(e)
		{}
	};


	struct Derivative {};
	static static Derivative derivative();




	struct FaceToEastTag {};
	struct FaceToWestTag {};

	struct IdentityOpr
	{
		typedef double result_type;

		result_type operator()( double, double) const {
			return 1.0;
		}

		result_type operator()( double, FaceToWestTag) const {
			return 0.0;
		}
		result_type operator()( double, FaceToEastTag) const {
			return 0.0;
		}
	};

	static struct IdentityOpr identityOpr();

	struct SecondDiffQuotinent1D
	{
		typedef double result_type;

		result_type operator()( double westSpacing, double eastSpacing) const {
			return ( - 1.0 / westSpacing  - 1.0 / eastSpacing );
		}
		result_type operator()( double spacing, FaceToWestTag) const {
			return 1.0 / spacing;
		}
		result_type operator()( double spacing, FaceToEastTag) const {
			return 1.0 / spacing;
		}
	};


}


#endif /* FINITEVOLUMEMETHOD_EXPRESSION_HPP_ */
