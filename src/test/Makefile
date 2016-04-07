EXECUTABLES_FOLLOWING_SUFFIX_RULE= \
discretizingIdentity2ndOrderDifferentialOperators \
discretizingLinCombOfIdAnd2ndDiffOperators \
discretizingPoissonEqWithNoBC \
discretizingPoissonEqWithBoundaryConditions

TARGET = ${EXECUTABLES_FOLLOWING_SUFFIX_RULE} \ 
airCooledCylinder \
airCooledCylinder_metaOpenMP \
airCooledCylinder_plainC \
airCooledCylinder_nonMetaOpenMP


PROTOTOMET_INCDIR= ../../../ProtoToMET/src
INCDIR = -I.. -I${PROTOTOMET_INCDIR}
CPP11STD = -std=c++11


.SUFFIXES: .cpp
.cpp:
	${CXX} ${CPP11STD} ${INCDIR} $< -o $@

all: compile_all

compile_all: ${TARGET} 

airCooledCylinder : airCooledCylinder.cpp airCooledCylinder.hpp  
	${CXX} ${CPP11STD} ${INCDIR} $< -o $@
	
airCooledCylinder_metaOpenMP : airCooledCylinder_metaOpenMP.cpp \
 airCooledCylinder.hpp  
	${CXX} ${CPP11STD} ${INCDIR} $< -o $@

airCooledCylinder_plainC : airCooledCylinder.hpp \
 airCooledCylinder_plainC.hpp \
 ${PROTOTOMET_INCDIR}/DenseLinAlg/diagPrecondConGrad_plainC.o
	${CXX} ${CPP11STD} ${INCDIR} $< \
	${PROTOTOMET_INCDIR}/DenseLinAlg/diagPrecondConGrad_plainC.o -o $@

airCooledCylinder_nonMetaOpenMP : airCooledCylinder.hpp \
 airCooledCylinder_nonMetaOpenMP.cpp \
 ${PROTOTOMET_INCDIR}/DenseLinAlg/diagPrecondConGrad_nonMetaOpenMP.o
	${CXX} ${CPP11STD} ${INCDIR} -fopenmp $< \
	${PROTOTOMET_INCDIR}/DenseLinAlg/diagPrecondConGrad_nonMetaOpenMP.o -o $@

clean:
	rm -rf ${TARGET}
