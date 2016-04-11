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


.SUFFIXES: .cpp airCooledCylinder_10points.log
.cpp:
	${CXX} ${CPP11STD} ${INCDIR} $< -o $@

all: compile_all airCooledCylinder_10points.eps airCooledCylinder_10points.jpg

compile_all: ${TARGET} airCooledCylinder_plainC airCooledCylinder_nonMetaOpenMP


airCooledCylinder_10points.log : airCooledCylinder_10points
	./airCooledCylinder_10points | sed -n -e '/^# stationary/, +11p' > airCooledCylinder_10points.log

airCooledCylinder_10points.eps airCooledCylinder_10points.jpg  : airCooledCylinder_10points.log
	gnuplot < airCooledCylinder_10points.gnuplot


airCooledCylinder_plainC : airCooledCylinder_plainC.cpp \
 ${PROTOTOMET_INCDIR}/DenseLinAlg/diagPrecondConGrad_plainC.o
	${CXX} ${CPP11STD} ${INCDIR} $^ -o $@

airCooledCylinder_nonMetaOpenMP : airCooledCylinder_plainC.cpp \
 ${PROTOTOMET_INCDIR}/DenseLinAlg/diagPrecondConGrad_nonMetaOpenMP.o
	${CXX} ${CPP11STD} ${INCDIR} -fopenmp $^ -o $@

clean:
	rm -rf ${TARGET} airCooledCylinder_plainC airCooledCylinder_nonMetaOpenMP
