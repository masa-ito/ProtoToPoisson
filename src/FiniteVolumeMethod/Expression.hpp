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

	// A wrapper for a finite volume expression
	template<typename E> struct ExprWrapper;

	struct Domain
		: proto::domain< proto::generator< ExprWrapper > >
		// The above grammar is associated with this domain.
		// : proto::domain< proto::generator< ExprWrapper >, ExprGrammar >
	{};

	// A wrapper template for finite volume expressions
	template< typename ExprType >
	struct ExprWrapper
		: proto::extends< ExprType, ExprWrapper< ExprType>, Domain >
	{
		// explicit
		ExprWrapper(ExprType const & expr = ExprType() )
			: proto::extends<ExprType, ExprWrapper<ExprType>, Domain >( expr)
		{}

	//		friend std::ostream &
	//		operator <<( std::ostream &sout,
	//					 ExprWrapper<ExprType> const &expr)
	//		{
	//			return std::cout << ExprGrammar()(expr);
	//		}
	};


	struct FaceToEastTag {};
	struct FaceToWestTag {};

	//
	// Identitiy operator in a discretized form
	//
	struct IdentityOpr : proto::callable
	{
		typedef double result_type;
		template <typename Sig> struct result;

		template <typename This, typename T1, typename T2>
		struct result< This(T1, T2) > { typedef double type; };

		IdentityOpr() {}
		IdentityOpr( const IdentityOpr & expr) {}

		double operator()( double, double) const {
			return 1.0;
		}

		double operator()( double, FaceToWestTag) const {
			return 0.0;
		}
		double operator()( double, FaceToEastTag) const {
			return 0.0;
		}
	};

	// Identity Operator in a continuum form
	typedef ExprWrapper< proto::terminal< IdentityOpr >::type >
														IdentityOperatorType;

	//
	// Difference operator
	//
	struct DifferenceOpr {};

	//
	// Differential operator
	// that is actually a wrapped object of difference operator
	//
	typedef ExprWrapper< proto::terminal< DifferenceOpr >::type >
												DifferentialOperatorType;

	//
	// 2nd-order difference operator
	//
	struct SecondDifferenceOpr : proto::callable
	{
		typedef double result_type;
		template <typename Sig> struct result;

		template <typename This, typename T1, typename T2>
		struct result< This(T1, T2) > { typedef double type; };

		SecondDifferenceOpr() {}
		SecondDifferenceOpr( const SecondDifferenceOpr & expr) {}

		double operator()( double westSpacing, double eastSpacing) const {
			return ( - 1.0 / westSpacing  - 1.0 / eastSpacing );
		}
		double operator()( double spacing, FaceToWestTag) const {
			return 1.0 / spacing;
		}
		double operator()( double spacing, FaceToEastTag) const {
			return 1.0 / spacing;
		}
	};

}


#endif /* FINITEVOLUMEMETHOD_EXPRESSION_HPP_ */
