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

	struct DiffOpr;
	struct IdentityOpr;

	struct SecondDiffQuotinent1D;


	struct DiffOprFactor : proto::when<
		proto::multiplies< proto::_, proto::terminal< DiffOpr > >,
		proto::_left
	> {};

	// The transformation rule for discretized representation of
	// second differential operator containing expressions
	struct DdSecondDiffOprGrammar : proto::or_<
		// diffOpr * diffOpr
		proto::when<
			proto::multiplies< proto::terminal< DiffOpr >,
							proto::terminal< DiffOpr > >,
			proto::_make_function(
				proto::_make_terminal( SecondDiffQuotinent1D ),
				proto::_state, proto::_data )
		>,
		// double * diffOpr
		proto::when<
			proto::multiplies<
				proto::multiplies< proto::terminal< double > ,
									proto::terminal< DiffOpr > >,
				proto::terminal< DiffOpr >
			>,
			proto::_make_multiplies(
				DiffOprFactor( proto::_left),
				proto::_make_function(
						proto::_make_terminal( SecondDiffQuotinent1D ),
						proto::_state, proto::_data )
			)
		>,
		// DdSecondDiffOprGrammar +(-) DdSecondDiffOprGrammar
		proto::plus< DdSecondDiffOprGrammar, DdSecondDiffOprGrammar >,
		proto::minus< DdSecondDiffOprGrammar, DdSecondDiffOprGrammar >
	> {};

	// The transformation rule for discretized operators
	struct DdOprGrammar : proto::or_<
		// IdentityOpr
		proto::when<
			proto::terminal< IdentityOpr >,
			proto::_make_function( proto::_,
							proto::_state, proto::_data)
		>,
		// inlucing Derivative * Derivative
		DdSecondDiffOprGrammar,
		// double * IdentityOpr
		proto::when<
			proto::multiplies< proto::terminal< double >,
								proto::terminal< IdentityOpr > >,
			proto::_make_multiplies(
				proto::_left,
				proto::_make_function( proto::_right,
										proto::_state, proto::_data) )
		>,
		// OprExprGrammar +(-) OprExprGrammar
		proto::plus< DdOprGrammar, DdOprGrammar >,
		proto::minus< DdOprGrammar, DdOprGrammar >
	> {};

	// The transformation rule for second differential operator
	// containing expressions
	struct SecondDiffOprGrammar : proto::or_<
		// diffOpr * diffOpr
		proto::when<
			proto::multiplies< proto::terminal< DiffOpr >,
								proto::terminal< DiffOpr > >,
			proto::_make_terminal( SecondDiffQuotinent1D )
		>,
		// double * DiffOOpr * DiffOpr
		proto::when<
			proto::multiplies<
				proto::multiplies< proto::terminal< double > ,
									proto::terminal< DiffOpr > >,
				proto::terminal< DiffOpr >
			>,
			proto::_make_multiplies(
				DiffOprFactor( proto::_left),
				proto::_make_terminal( SecondDiffQuotinent1D )
			)
		>,
		// SecondDiffOprGrammar +(-) SecondDiffOprGrammar
		proto::plus< SecondDiffOprGrammar, SecondDiffOprGrammar >,
		proto::minus< SecondDiffOprGrammar, SecondDiffOprGrammar >
	> {};

	// The tranformation rule for operator expressions
	struct OprExprGrammar : proto::or_<
		// OprActionGrammar( _ )
		proto::when< proto::function< DdOprGrammar, proto::_ >,
					 DdOprGrammar( proto::_child0,
									proto::_child1, proto::_child2) >,
		// IdentityOpr
		proto::terminal< IdentityOpr >,
		// inlucing Derivative * Derivative
		SecondDiffOprGrammar,
		// (Any expression) * OprExprGrammar
		proto::multiplies< proto::_ , OprExprGrammar >,
		// OprExprGrammar +(-) OprExprGrammar
		proto::plus< OprExprGrammar, OprExprGrammar >,
		proto::minus< OprExprGrammar, OprExprGrammar >
	> {};

	// The tranformation rule for finite volume expression templates
	struct ExprGrammar : proto::or_<
		OprExprGrammar
	> {};

}


#endif /* FINITEVOLUMEMETHOD_GRAMMAR_HPP_ */
