
all: protoDeepCopyUserDefinedExpr discretizingIdentyOperator \
		discretizingDifferentialOperator \
		# airCooledCylinder
		#tinyLambdaLib \

INCDIR = -I. -I../../ProtoToMET/src
CPP11STD = -std=c++11

tinyLambdaLib : tinyLambdaLib.cpp
	${CXX} ${CPP11STD} ${INCDIR} $< -o $@

protoDeepCopyUserDefinedExpr : protoDeepCopyUserDefinedExpr.cpp
	${CXX} ${CPP11STD} ${INCDIR} $< -o $@

discretizingIdentyOperator : discretizingIdentyOperator.cpp \
								discretizingIdentyOperator.hpp
	${CXX} ${CPP11STD} ${INCDIR} $< -o $@

discretizingDifferentialOperator : discretizingDifferentialOperator.cpp \
									discretizingDifferentialOperator.hpp
	${CXX} ${CPP11STD} ${INCDIR} $< -o $@

airCooledCylinder : airCooledCylinder.cpp
	${CXX} ${CPP11STD} ${INCDIR} $< -o $@

