/*
 * Grammar.hpp
 *
 *  Created on: 2015/11/10
 *      Author: Masakatsu ITO
 */

#ifndef FINITEVOLUMEMETHOD_GRAMMAR_HPP_
#define FINITEVOLUMEMETHOD_GRAMMAR_HPP_

#include <iostream>
#include <valarray>
#include <boost/proto/proto.hpp>


#include <DenseLinAlg/DenseLinAlg.hpp>

namespace DLA = DenseLinAlg;


namespace FiniteVolumeMethod {

	struct ExprGrammar;

	//
	// Grammars for discretizging identity operator
	//

	struct IdentityOpr;

	struct IdentityOprFuncGrammar : proto::or_<
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
			proto::function< IdentityOprFuncGrammar,
							 proto::_, proto::_ >,
			IdentityOprFuncGrammar( proto::_child0,
							proto::_value( proto::_child1),
							proto::_value( proto::_child2) )
		>,
		// IdentityOpr
		proto::terminal< IdentityOpr >
	> {};

	//
	// Grammars for discretizging 2nd-order differential operator
	//
	struct DifferenceOpr;
	struct SecondDifferenceOpr;


	struct DoubleDifferenceOprProductFuncGrammar : proto::or_<
		// ( DifferenceOpr * DifferenceOpr )( _, _)
		proto::when<
			proto::multiplies< proto::terminal< DifferenceOpr >,
								proto::terminal< DifferenceOpr > >,
			SecondDifferenceOpr( proto::_state, proto::_data)
		>,
		// ( ( _ *  DifferenceOpr ) * DifferenceOpr )( _, _)
		// -> _ * ( DifferenceOpr * DifferenceOpr )( _, _)
		proto::when<
			proto::multiplies<
				proto::multiplies< proto::_,
									proto::terminal< DifferenceOpr > >,
				proto::terminal< DifferenceOpr >
			>,
			ExprGrammar(
				proto::_make_multiplies(
					proto::_left( proto::_left ),
					proto::_make_function(
						proto::_make_multiplies(
								proto::_right( proto::_left ),
								proto::_right
						),
						proto::_state, proto::_data
					)
 				)
			)
		>
	> {};


	struct DoubleDifferenceOprProductGrammar : proto::or_<
		// ( DifferenceOpr * DifferenceOpr )( _, _)
		proto::when<
			proto::function< DoubleDifferenceOprProductFuncGrammar,
							 proto::_, proto::_ >,
			DoubleDifferenceOprProductFuncGrammar( proto::_child0,
					proto::_value( proto::_child1),
					proto::_value( proto::_child2) )
		>,
		// DifferenceOpr * DifferenceOpr
		proto::multiplies< proto::terminal< DifferenceOpr >,
						   proto::terminal< DifferenceOpr > >,
		// ( _ * DifferenceOpr ) * DifferenceOpr
		proto::multiplies<
			proto::multiplies< proto::_,
							   proto::terminal< DifferenceOpr > >,
			proto::terminal< DifferenceOpr >
		>
	> {};


	//
	// The tranformation rule for finite volume expression templates
	// This grammar integarates the grammar for the identiy operator
	// and that for differential operators.
	//
	struct ExprGrammar : proto::or_<
		IdentiyOprTermGrammar,
		DoubleDifferenceOprProductGrammar,

		// default
		proto::otherwise< proto::_default< ExprGrammar >  >
	> {};


}


#endif /* FINITEVOLUMEMETHOD_GRAMMAR_HPP_ */
