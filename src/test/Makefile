
all:	discretizingIdentity2ndOrderDifferentialOperators \
	discretizingLinCombOfIdAnd2ndDiffOperators \
	discretizingPoissonEqWithNoBC \
	discretizingPoissonEqWithBoundaryConditions \
	airCooledCylinder \
	airCooledCylinder_plainC \
	airCooledCylinder_nonMetaOpenMP \
#


INCDIR = -I.. -I../../../ProtoToMET/src
CPP11STD = -std=c++11

discretizingIdentity2ndOrderDifferentialOperators : \
		discretizingIdentity2ndOrderDifferentialOperators.cpp
	${CXX} ${CPP11STD} ${INCDIR} $< -o $@


discretizingLinCombOfIdAnd2ndDiffOperators : \
		discretizingLinCombOfIdAnd2ndDiffOperators.cpp
	${CXX} ${CPP11STD} ${INCDIR} $< -o $@

discretizingPoissonEqWithNoBC : discretizingPoissonEqWithNoBC.cpp
	${CXX} ${CPP11STD} ${INCDIR} $< -o $@

discretizingPoissonEqWithBoundaryConditions : \
		discretizingPoissonEqWithBoundaryConditions.cpp
#	${CXX} -g ${CPP11STD} ${INCDIR} $< -o $@
	${CXX} ${CPP11STD} ${INCDIR} $< -o $@

airCooledCylinder : airCooledCylinder.cpp
	${CXX} ${CPP11STD} ${INCDIR} $< -o $@

airCooledCylinder_plainC : airCooledCylinder_plainC.cpp \
		../../../ProtoToMET/src/DenseLinAlg/diagPrecondConGrad_plainC.o
	${CXX} ${CPP11STD} ${INCDIR} $^ -o $@

airCooledCylinder_nonMetaOpenMP : airCooledCylinder_nonMetaOpenMP.cpp \
		../../../ProtoToMET/src/DenseLinAlg/diagPrecondConGrad_nonMetaOpenMP.o
	${CXX} ${CPP11STD} ${INCDIR} -fopenmp $^ -o $@


