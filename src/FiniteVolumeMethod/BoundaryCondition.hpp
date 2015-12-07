/*
 * BoundaryCondition.hpp
 *
 *  Created on: 2015/12/05
 *      Author: Masakatsu ITO
 */

#ifndef FINITEVOLUMEMETHOD_BOUNDARYCONDITION_HPP_
#define FINITEVOLUMEMETHOD_BOUNDARYCONDITION_HPP_

#include <iostream>
#include <valarray>
#include <boost/proto/proto.hpp>


#include <DenseLinAlg/DenseLinAlg.hpp>

namespace DLA = DenseLinAlg;


namespace FiniteVolumeMethod {

	class AbstBoundaryCondition
	{
	protected:
		const int outerIdx, innerIdx;
		const double fixedVal, outerInnerSpc;

	public:
		explicit
		AbstBoundaryCondition(int outerIndex_, int innerIndex_,
							double fixedValue_, double outerInnerSpacing_) :
			outerIdx( outerIndex_), innerIdx( innerIndex_),
			fixedVal( fixedValue_), outerInnerSpc( outerInnerSpacing_)
		{}

		virtual ~AbstBoundaryCondition() {}

		int outerIndex() const { return outerIdx; }
		int innerIndex() const { return innerIdx; }
		double outerInnerSpacing() const { return outerInnerSpc; }
	};


	class DirichletCondition : public AbstBoundaryCondition
	{
	public:
		explicit
		DirichletCondition(int outerIndex_, int innerIndex_,
							double fixedValue_, double outerInnerSpacing_) :
			AbstBoundaryCondition( outerIndex_, innerIndex_,
									fixedValue_, outerInnerSpacing_ )
		{}

		double fixedValue() const { return fixedVal; }

	};

	class NeumannCondition : public AbstBoundaryCondition
	{
	public:
		explicit
		NeumannCondition(int outerIndex_, int innerIndex_,
				double fixedValue_, double outerInnerSpacing_) :
					AbstBoundaryCondition( outerIndex_, innerIndex_,
							fixedValue_, outerInnerSpacing_ )
			{}

		double fixedGradient() const { return fixedVal; }
	};
}





#endif /* FINITEVOLUMEMETHOD_BOUNDARYCONDITION_HPP_ */
