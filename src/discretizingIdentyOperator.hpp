/*
 * discretizingIdentyOperator.hpp
 *
 *  Created on: 2015/12/14
 *      Author: Masakatsu ITO
 */

#ifndef DISCRETIZINGIDENTYOPERATOR_HPP_
#define DISCRETIZINGIDENTYOPERATOR_HPP_

#include <iostream>
#include <boost/proto/proto.hpp>


// #include <FiniteVolumeMethod/Grammar.hpp>
// #include <FiniteVolumeMethod/Expression.hpp>

namespace mpl = boost::mpl;
namespace proto = boost::proto;

namespace FiniteVolumeMethod {

	//
	// Grammar.hpp
	//

	struct IdentityOpr;

	struct IdentiyOprExprGrammar : proto::or_<
		// IdentityOpr
		proto::terminal< IdentityOpr >
	> {};

	// The tranformation rule for finite volume expression templates
	struct ExprGrammar : proto::or_<
		//OprExprGrammar
		IdentiyOprExprGrammar
	> {};


	//
	// Expression.hpp
	//

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



	// Define a trait for detecting linear algebraic terminals, to be used
	// by the BOOST_PROTO_DEFINE_OPERATORS macro below.
	template<typename> struct IsExpr  : mpl::false_ {};

	// template<> struct IsExpr< DiffOpr > : mpl::true_  {};
	template<> struct IsExpr< IdentityOpr > : mpl::true_  {};
	// template<> struct IsExpr< SecondDiffQuotinent1D > : mpl::true_  {};

	// template<> struct IsExpr< FooClass > : mpl::true_  {};

	// This defines all the overloads to make expressions involving
	// Vector and Matrix objects to build Proto's expression templates.
	BOOST_PROTO_DEFINE_OPERATORS(IsExpr, Domain)


}





#endif /* DISCRETIZINGIDENTYOPERATOR_HPP_ */
