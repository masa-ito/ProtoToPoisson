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


	struct DiffOpr
	{
			template <typename Sig> struct result;

			template <typename This, typename T1, typename T2>
			struct result< This(T1, T2) > { typedef double type; };

	};

	DiffOpr diffOpr = {};



	struct FaceToEastTag {};
	struct FaceToWestTag {};

	struct IdentityOpr
	{
		// typedef double result_type;
		template <typename Sig> struct result;

		template <typename This, typename T1, typename T2>
		struct result< This(T1, T2) > { typedef double type; };

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

	static struct IdentityOpr identityOpr = {};

	// Assuming the area of cross section is equal to 1.
	struct SecondDifferenceOpr
	{
		template <typename Sig> struct result;

		template <typename This, typename T1, typename T2>
		struct result< This(T1, T2) > { typedef double type; };


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
