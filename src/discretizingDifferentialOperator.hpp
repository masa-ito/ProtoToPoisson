/*
 * discretizingDifferentialOperator.hpp
 *
 *  Created on: 2016/01/21
 *      Author: Masakatsu ITO
 */

#ifndef DISCRETIZINGDIFFERENTIALOPERATOR_HPP_
#define DISCRETIZINGDIFFERENTIALOPERATOR_HPP_

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

	struct DifferenceOpr {};

	// Assuming the area of cross section is equal to 1.
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


	struct ExprGrammar;

	struct DoubleDifferenceOprProductFuncGrammar : proto::or_<
		// ( DifferenceOpr * DifferenceOpr )( _, _)
		proto::when<
			proto::multiplies< proto::terminal< DifferenceOpr >,
								proto::terminal< DifferenceOpr > >,
			SecondDifferenceOpr( proto::_state, proto::_data)
		> /*
		// ( _ *  DifferenceOpr * DifferenceOpr )( _, _)
		// -> _ * ( DifferenceOpr * DifferenceOpr )( _, _)
		*/
	> {};

	// Implment this struct !!!
	struct DoubleDifferenceOprTermGrammar : proto::or_<
		// ( DifferenceOpr * DifferenceOpr )( _, _)
		proto::when<
			proto::function< DoubleDifferenceOprProductFuncGrammar,
							 proto::_, proto::_ >,
			DoubleDifferenceOprProductFuncGrammar( proto::_child0,
					proto::_value( proto::_child1),
					proto::_value( proto::_child2) )
		>,
		/* // DifferenceOpr * DifferenceOpr( _, _)
		//  -> ( DifferenceOpr * DifferenceOpr )( _, _)
		proto::when<
			proto::multiplies<
				proto::terminal< DifferenceOpr >,
				proto::function< proto::terminal< DifferenceOpr >,
								 proto::_, proto::_  >
			>,
			ExprGrammar(
				proto::_make_function(
						proto::_make_multiplies( proto::_left,
									proto::_child0( proto::_right ) ),
						proto::_child1( proto::_right ),
						proto::_child2( proto::_right )
				)
			)
		>, */
		// DifferenceOpr * DifferenceOpr
		proto::multiplies< proto::terminal< DifferenceOpr >,
						   proto::terminal< DifferenceOpr > >
	> {};


	// The tranformation rule for finite volume expression templates
	struct ExprGrammar : proto::or_<
		DoubleDifferenceOprTermGrammar,

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


	typedef ExprWrapper< proto::terminal< DifferenceOpr >::type >
												DifferentialOperatorType;
}




#endif /* DISCRETIZINGDIFFERENTIALOPERATOR_HPP_ */
