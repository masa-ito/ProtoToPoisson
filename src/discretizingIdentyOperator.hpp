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

	struct FaceToEastTag {};
	struct FaceToWestTag {};

	struct IdentityOpr : proto::callable
	{
		typedef double result_type;
		template <typename Sig> struct result;

		template <typename This, typename T1, typename T2>
		struct result< This(T1, T2) > { typedef double type; };
//		template <typename This, typename T>
//		struct result< This(T, T) > { typedef double type; };

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

	struct ExprGrammar;

	struct IdentityOprDiscretizationGrammar : proto::or_<
		proto::when<
			proto::terminal< IdentityOpr >,
			IdentityOpr( proto::_state, proto::_data)
		>,
		proto::when<
			proto::multiplies< proto::_, proto::terminal< IdentityOpr > >,
			ExprGrammar(
					proto::_make_multiplies(
							proto::_left,
							proto::_make_function( proto::_right,
									proto::_state, proto::_data) ) )
		>
	> {};

	struct IdentiyOprExprGrammar : proto::or_<
		// IdentityOpr( _, _)
		proto::when<
			proto::function< IdentityOprDiscretizationGrammar,
							 proto::_, proto::_ >,
			IdentityOprDiscretizationGrammar( proto::_child0,
							proto::_value( proto::_child1),
							proto::_value( proto::_child2) )
		>,
//		proto::when<
//			proto::function< proto::terminal< IdentityOpr >,
//							 proto::_, proto::_ >,
//			IdentityOpr( proto::_value( proto::_child1),
//						 proto::_value( proto::_child2) )
//		>,

//		// double * IdentityOpr( _, _)
//		proto::when<
//			proto::multiplies<
//				proto::_,
//				proto::function< proto::terminal< IdentityOpr >,
//								proto::_, proto::_ >
//			>,
//			proto::_default< IdentiyOprExprGrammar >
//		>,

		// IdentityOpr
		proto::terminal< IdentityOpr > /*,

		// IdentiyOprExprGrammar * double, or
		// double * IdentiyOprExprGrammar
		// proto::multiplies< IdentiyOprExprGrammar, proto::terminal< double > >,
		// proto::multiplies< proto::terminal< double >, IdentiyOprExprGrammar >,

		// IdentiyOprExprGrammar +(-) IdentiyOprExprGrammar
		proto::plus< IdentiyOprExprGrammar, IdentiyOprExprGrammar >,
		proto::minus< IdentiyOprExprGrammar, IdentiyOprExprGrammar > */
	> {};


	// The tranformation rule for finite volume expression templates
	struct ExprGrammar : proto::or_<
		// proto::terminal< double >,
		//OprExprGrammar
		IdentiyOprExprGrammar,

		// default
		proto::otherwise< proto::_default< ExprGrammar >  > /* ,

		// plus, minus
		proto::when< proto::plus< ExprGrammar, ExprGrammar >,
					proto::_default< ExprGrammar > >,
		proto::when< proto::minus< ExprGrammar, ExprGrammar >,
					proto::_default< ExprGrammar > >,

		// multiplies
		proto::when< proto::multiplies< ExprGrammar, ExprGrammar >,
					 proto::_default< ExprGrammar > > */
	> {};

	//
	// Expression.hpp
	//

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


	/* // Define a trait for detecting linear algebraic terminals, to be used
	// by the BOOST_PROTO_DEFINE_OPERATORS macro below.
	template<typename> struct IsExpr  : mpl::false_ {};

	template<> struct IsExpr< double > : mpl::true_  {};

	// template<> struct IsExpr< DiffOpr > : mpl::true_  {};
	template<> struct IsExpr< IdentityOpr > : mpl::true_  {};
	// template<> struct IsExpr< SecondDiffQuotinent1D > : mpl::true_  {};

	// template<> struct IsExpr< FooClass > : mpl::true_  {};

	// This defines all the overloads to make expressions involving
	// Vector and Matrix objects to build Proto's expression templates.
	BOOST_PROTO_DEFINE_OPERATORS( IsExpr, Domain) */


	// ExprWrapper< proto::terminal< IdentityOpr >::type > const
	//												identityOpr = {{}};
	// proto::terminal< IdentityOpr >::type const identityOpr = {};

	typedef ExprWrapper< proto::terminal< IdentityOpr >::type >
														IdentityOperator;
	// typedef proto::terminal< IdentityOpr >::type IdentityOperator;
}





#endif /* DISCRETIZINGIDENTYOPERATOR_HPP_ */
