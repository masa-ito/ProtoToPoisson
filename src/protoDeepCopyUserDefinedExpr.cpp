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

     // A user defined expression
     struct DifferenceOperator : proto::callable
     {
         typedef double result_type;
         template < typename Sig > struct result;

         template < typename This, typename T >
         struct result< This(T, T) > { typedef double type; };

         DifferenceOperator() {}
         DifferenceOperator( const DifferenceOperator & expr) {}

         double operator()( double dx1, double dx2) const {
             return ( 1.0 /dx1 + 1.0 /dx2 );
         }
     };

     struct ExprGrammar : proto::or_<
         proto::when< proto::function< proto::terminal< DifferenceOperator >,
		 	 	 	 	 proto::_, proto::_>,
         DifferenceOperator( proto::_value(proto::_child1),
        		 	 	 	 proto::_value(proto::_child2)) >
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

         ExprWrapper(const ExprType& e)
             : proto::extends<ExprType, ExprWrapper<ExprType>, Domain>(e)
         {}

         friend std::ostream &operator <<(std::ostream &sout, ExprWrapper<ExprType> const &expr)
        {
            return std::cout << ExprGrammar()(expr);
        }
     };

     // Define a trait for detecting linear algebraic terminals, to be used
     // by the BOOST_PROTO_DEFINE_OPERATORS macro below.
     template< typename > struct IsExpr  : mpl::false_ {};

     template< > struct IsExpr< DifferenceOperator > : mpl::true_  {};

     // This defines all the overloads to make expressions involving
     // Vector and Matrix objects to build Proto's expression templates.
     BOOST_PROTO_DEFINE_OPERATORS(IsExpr, Domain)

     ExprWrapper< boost::proto::terminal< DifferenceOperator >::type >
     	 	 	 	 	 	 	 	 	 const opr = {{}};
}


int main() {
     // This worked well.
     std::cout << MyEDSL::opr( 0.1, 0.1) << std::endl;

     // Now this can be compiled.
     auto copyOfOperator = proto::deep_copy( MyEDSL::opr );

     return 0;
}
