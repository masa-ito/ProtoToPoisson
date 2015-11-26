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

	struct Derivative;
	struct IdentityOpr;

	struct OprActionGrammar : proto::or_<
		// Not yet implemented
	> {};

	struct DerivativeFactor : proto::when<
		proto::multiplies< proto::_, proto::terminal< Derivative > >,
		proto::_left
	> {};

	struct SecondDerivativeGrammar : proto::or_<
		proto::when<
			proto::multiplies< proto::terminal< Derivative >,
								proto::terminal< Derivative > >,
			proto::make_terminal( SecondDiffQuotinent1D() )
		>,
		proto::when<
			proto::multiplies<
				proto::multiplies< proto::terminal< proto::_ >,
									proto::terminal< Derivative > >,
				proto::terminal< Derivative >
			>,
			proto::make_multiplies<
				DerivativeFactor( proto::_left),
				proto::make_terminal( SecondDiffQuotinent1D() )
			>
		>,
		proto::plus< SecondDerivativeGrammar, SecondDerivativeGrammar >,
		proto::minus< SecondDerivativeGrammar, SecondDerivativeGrammar >
	> {};

	// The tranformation rule for operator expressions
	struct OprExprGrammar : proto::or_<
		// OprActionGrammar( _ )
		proto::when< proto::function< OprActionGrammar, proto::_ >,
					OprActionGrammar( proto::_child0,
									proto::_child1, proto::_child2) >,
		// IdentityOpr
		proto::termina< IdentityOpr >,
		// inlucing Derivative * Derivative
		SecondDerivativeGrammar,
		// double * IdentityOpr
		proto::multiplies< proto::terminal< _ >,
							proto::termina< IdentityOpr > >,
		// OprExprGrammar +(-) OprExprGrammar
		proto::plus< OprExprGrammar, OprExprGrammar >,
		proto::minus< OprExprGrammar, OprExprGrammar >
	> {};

	// The tranformation rule for finite volume expressions
	struct ExprGrammar : proto::or_<
		OprExprGrammar
	> {};

}


#endif /* FINITEVOLUMEMETHOD_GRAMMAR_HPP_ */
