/*
 * protoDeepCopyUserDefinedExpr.cpp
 *
 *  Created on: 2015/12/14
 *      Author: Masakatsu ITO
 */

#include <iostream>
#include <boost/proto/proto.hpp>


namespace mpl = boost::mpl;
namespace proto = boost::proto;

namespace MyEDSL {

	struct DifferenceOperator;

	struct ExprGrammar : proto::or_<
		proto::terminal< DifferenceOperator >
	> {};

	// A wrapper for a user defined expression
	template< typename E > struct ExprWrapper;

	// The above grammar is associated with this domain.
	struct Domain
		: proto::domain< proto::generator< ExprWrapper >, ExprGrammar >
	{};

	// A wrapper template for a user defined expression
	template< typename ExprType >
	struct ExprWrapper
		: proto::extends< ExprType, ExprWrapper< ExprType >, Domain >
	{

		explicit ExprWrapper(const ExprType& e)
			: proto::extends<ExprType, ExprWrapper<ExprType>, Domain>(e)
		{}
	};


	// A user defined expression
	struct DifferenceOperator
	{
		typedef double result_type;
		template <typename Sig> struct result;

//		template <typename This, typename T1, typename T2>
//		struct result< This(T1, T2) > { typedef double type; };
		template < typename This, typename T >
		struct result< This(T, T) > { typedef double type; };

		explicit DifferenceOperator() {}
		DifferenceOperator( const DifferenceOperator & expr) {}

		double operator()( double dx1, double dx2) const {
			return ( 1.0 /dx1 + 1.0 /dx2 );
		}
	};

	static struct DifferenceOperator opr = {};


	// Define a trait for detecting linear algebraic terminals, to be used
	// by the BOOST_PROTO_DEFINE_OPERATORS macro below.
	template<typename> struct IsExpr  : mpl::false_ {};

	template<> struct IsExpr< DifferenceOperator > : mpl::true_  {};

	// This defines all the overloads to make expressions involving
	// Vector and Matrix objects to build Proto's expression templates.
	BOOST_PROTO_DEFINE_OPERATORS(IsExpr, Domain)
}


int main() {
	// This worked well.
	std::cout << MyEDSL::opr( 0.1, 0.1) + 1.0 << std::endl;

	// But this cannot be compiled.
	auto copyOfOperator = proto::deep_copy( MyEDSL::opr );

	return 0;
}



