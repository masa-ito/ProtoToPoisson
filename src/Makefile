
all:	protoDeepCopyUserDefinedExpr discretizingIdentyOperator \
		airCooledCylinder


INCDIR = -I. -I../../ProtoToMET/src
CPP11STD = -std=c++11

protoDeepCopyUserDefinedExpr : protoDeepCopyUserDefinedExpr.cpp
	${CXX} ${CPP11STD} ${INCDIR} $< -o $@

discretizingIdentyOperator : discretizingIdentyOperator.cpp
	${CXX} ${CPP11STD} ${INCDIR} $< -o $@

airCooledCylinder : airCooledCylinder.cpp
	${CXX} ${CPP11STD} ${INCDIR} $< -o $@

