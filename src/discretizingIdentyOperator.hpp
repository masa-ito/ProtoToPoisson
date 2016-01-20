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
		// IdentityOpr( _, _)
		proto::when<
			proto::terminal< IdentityOpr >,
			IdentityOpr( proto::_state, proto::_data)
		>,
		// (_ * IdentityOpr)( _, _)  -> _ * IdentityOpr( _, _)
		proto::when<
			proto::multiplies< proto::_, proto::terminal< IdentityOpr > >,
			ExprGrammar(
					proto::_make_multiplies(
							proto::_left,
							proto::_make_function( proto::_right,
									proto::_state, proto::_data) ) )
		>
	> {};

	struct IdentiyOprTermGrammar : proto::or_<
		// IdentityOpr( _, _) or (_ * IdentityOpr)( _, _)
		proto::when<
			proto::function< IdentityOprDiscretizationGrammar,
							 proto::_, proto::_ >,
			IdentityOprDiscretizationGrammar( proto::_child0,
							proto::_value( proto::_child1),
							proto::_value( proto::_child2) )
		>,
		// IdentityOpr
		proto::terminal< IdentityOpr >
	> {};

	// The tranformation rule for finite volume expression templates
	struct ExprGrammar : proto::or_<
		IdentiyOprTermGrammar,

		// default
		proto::otherwise< proto::_default< ExprGrammar >  >
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


	typedef ExprWrapper< proto::terminal< IdentityOpr >::type >
														IdentityOperatorType;
}


#endif /* DISCRETIZINGIDENTYOPERATOR_HPP_ */
