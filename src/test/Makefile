TARGET= \
discretizingIdentity2ndOrderDifferentialOperators \
discretizingLinCombOfIdAnd2ndDiffOperators \
discretizingPoissonEqWithNoBC \
discretizingPoissonEqWithBoundaryConditions \
airCooledCylinder \
airCooledCylinder_10points

PROTOTOMET_INCDIR= ../../../ProtoToMET/src
INCDIR = -I.. -I${PROTOTOMET_INCDIR}
CPP11STD = -std=c++11


.SUFFIXES: .cpp
.cpp:
	${CXX} ${CPP11STD} ${INCDIR} $< -o $@

all: compile_all

compile_all: ${TARGET} airCooledCylinder_plainC airCooledCylinder_nonMetaOpenMP

airCooledCylinder_plainC : airCooledCylinder_plainC.cpp \
 ${PROTOTOMET_INCDIR}/DenseLinAlg/diagPrecondConGrad_plainC.o
	${CXX} ${CPP11STD} ${INCDIR} $^ -o $@

airCooledCylinder_nonMetaOpenMP : airCooledCylinder_nonMetaOpenMP.cpp \
 ${PROTOTOMET_INCDIR}/DenseLinAlg/diagPrecondConGrad_nonMetaOpenMP.o
	${CXX} ${CPP11STD} ${INCDIR} -fopenmp $^ -o $@

clean:
	rm -rf ${TARGET} airCooledCylinder_plainC airCooledCylinder_nonMetaOpenMP
