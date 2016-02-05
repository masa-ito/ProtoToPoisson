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

	struct DifferenceOpr;
	struct SecondDifferenceOpr;

	//
	// The transformation rule for the functions for
	// discretizing the linear combination of
	// idential and 2nd-order differential operators
	//
	struct OprLinCombFuncGrammar : proto::or_<
		// IdentityOpr( _, _)
		proto::when<
			proto::terminal< IdentityOpr >,
			IdentityOpr( proto::_state, proto::_data)
		>,

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
			proto::_make_multiplies(
				proto::_left( proto::_left ),
				SecondDifferenceOpr( proto::_state, proto::_data)
 			)
		>,

		proto::when<
			proto::multiplies< proto::_, OprLinCombFuncGrammar >,
			proto::_make_multiplies(
				proto::_left,
				OprLinCombFuncGrammar( proto::_right,
									   proto::_state, proto::_data)
			)
		>,

		proto::plus< OprLinCombFuncGrammar, OprLinCombFuncGrammar >,
		proto::minus< OprLinCombFuncGrammar, OprLinCombFuncGrammar >
	> {};

	//
	// The transformation rule for the linear combination of
	// the function for discretizing the linear combination of
	// idential and 2nd-order differential operators
	//
	struct FuncLinCombGrammar : proto::or_<
		proto::when<
			proto::function< OprLinCombFuncGrammar,
						 proto::_, proto::_ >,
			OprLinCombFuncGrammar(
				proto::_child0,
				proto::_value( proto::_child1),
				proto::_value( proto::_child2)
			)
		>,

		proto::when<
			proto::multiplies< proto::_, FuncLinCombGrammar >,
			proto::_make_multiplies(
				proto::_left,
				FuncLinCombGrammar( proto::_right )
			)
		>,

		proto::plus< FuncLinCombGrammar, FuncLinCombGrammar >,
		proto::minus< FuncLinCombGrammar, FuncLinCombGrammar >
	> {};

	//
	// The grammar for the linear combination of
	// identical and 2nd-order differential operators
	//
	struct OprLinCombGrammar : proto::or_<
		// IdentityOpr
		proto::terminal< IdentityOpr >,

		// DifferenceOpr * DifferenceOpr
		proto::multiplies< proto::terminal< DifferenceOpr >,
						   proto::terminal< DifferenceOpr > >,
		// ( _ * DifferenceOpr ) * DifferenceOpr
		proto::multiplies<
			proto::multiplies< proto::_,
							   proto::terminal< DifferenceOpr > >,
			proto::terminal< DifferenceOpr >
		>,

		proto::multiplies< proto::_, OprLinCombGrammar >,
		// proto::multiplies< OprLinCombGrammar, proto::_ >,
		proto::plus< OprLinCombGrammar, OprLinCombGrammar >,
		proto::minus< OprLinCombGrammar, OprLinCombGrammar >
	> {};


	//
	// The tranformation rule for finite volume expressions
	//
	struct ExprGrammar : proto::or_<
		proto::when<
			FuncLinCombGrammar,
			proto::_default<>( FuncLinCombGrammar( proto::_) )
		>,
		OprLinCombGrammar
	> {};


}


#endif /* FINITEVOLUMEMETHOD_GRAMMAR_HPP_ */
