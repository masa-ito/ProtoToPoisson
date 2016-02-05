/*
 * FiniteVolumeMethod.hpp
 *
 *  Created on: 2015/11/10
 *      Author: Masakatsu ITO
 */

#ifndef FINITEVOLUMEMETHOD_FINITEVOLUMEMETHOD_HPP_
#define FINITEVOLUMEMETHOD_FINITEVOLUMEMETHOD_HPP_

#include <iostream>
#include <boost/proto/proto.hpp>


#include <FiniteVolumeMethod/Grammar.hpp>
#include <FiniteVolumeMethod/Expression.hpp>

#include <FiniteVolumeMethod/BoundaryCorrector.hpp>
#include <FiniteVolumeMethod/LazyDiscretizer.hpp>
#include <FiniteVolumeMethod/Grid.hpp>

/*
namespace mpl = boost::mpl;
namespace proto = boost::proto;

namespace FiniteVolumeMethod {

	// Define a trait for detecting linear algebraic terminals, to be used
	// by the BOOST_PROTO_DEFINE_OPERATORS macro below.
	template<typename> struct IsExpr  : mpl::false_ {};

	// template<> struct IsExpr< DiffOpr > : mpl::true_  {};
	template<> struct IsExpr< IdentityOpr > : mpl::true_  {};
	// template<> struct IsExpr< SecondDifferenceOperator > : mpl::true_  {};

	// template<> struct IsExpr< FooClass > : mpl::true_  {};

	// This defines all the overloads to make expressions involving
	// Vector and Matrix objects to build Proto's expression templates.
	BOOST_PROTO_DEFINE_OPERATORS(IsExpr, Domain)
}
*/

#endif /* FINITEVOLUMEMETHOD_FINITEVOLUMEMETHOD_HPP_ */
