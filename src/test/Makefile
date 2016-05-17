
EXECUTABLES_FOLLOWING_SUFFIX_RULE= \
discretizingIdentity2ndOrderDifferentialOperators \
discretizingLinCombOfIdAnd2ndDiffOperators \
discretizingPoissonEqWithNoBC \
discretizingPoissonEqWithBoundaryConditions

TARGET= ${EXECUTABLES_FOLLOWING_SUFFIX_RULE} \
airCooledCylinder \
airCooledCylinder_metaOpenMP \
airCooledCylinder_plainC \
airCooledCylinder_nonMetaOpenMP \
airCooledCylinder_10points

PROTOTOMET_INCDIR= ../../../ProtoToMET/src
INCDIR = -I.. -I${PROTOTOMET_INCDIR}
CPP11STD = -std=c++11

FIGURES= airCooledCylinder_10points.eps airCooledCylinder_10points.jpg



all: ${TARGET} ${FIGURES}

compile_all: ${TARGET} 

plot_all : ${FIGURES}


.SUFFIXES: .cpp
.cpp:
	${CXX} ${CPP11STD} ${INCDIR} $< -o $@


airCooledCylinder : airCooledCylinder.cpp airCooledCylinder.hpp  
	${CXX} ${CPP11STD} ${INCDIR} $< -o $@

airCooledCylinder_metaOpenMP : airCooledCylinder.cpp airCooledCylinder.hpp  
	${CXX} ${CPP11STD} ${INCDIR} -fopenmp $< -o $@
	

airCooledCylinder_plainC : airCooledCylinder_plainC.cpp \
 airCooledCylinder.hpp \
 ${PROTOTOMET_INCDIR}/DenseLinAlg/diagPrecondConGrad_plainC.o
	${CXX} ${CPP11STD} ${INCDIR} $< \
	${PROTOTOMET_INCDIR}/DenseLinAlg/diagPrecondConGrad_plainC.o -o $@


airCooledCylinder_nonMetaOpenMP : airCooledCylinder_plainC.cpp \
 airCooledCylinder.hpp \
 ${PROTOTOMET_INCDIR}/DenseLinAlg/diagPrecondConGrad_nonMetaOpenMP.o
	${CXX} ${CPP11STD} ${INCDIR} -fopenmp $< \
	${PROTOTOMET_INCDIR}/DenseLinAlg/diagPrecondConGrad_nonMetaOpenMP.o -o $@

airCooledCylinder_10points : airCooledCylinder_10points.cpp \
 airCooledCylinder.hpp  
	${CXX} ${CPP11STD} ${INCDIR} $< -o $@


airCooledCylinder_10points.log : airCooledCylinder_10points
	./airCooledCylinder_10points | sed -n -e '/^# stationary/, +11p' > airCooledCylinder_10points.log

airCooledCylinder_10points.eps airCooledCylinder_10points.jpg  : airCooledCylinder_10points.log
	gnuplot < airCooledCylinder_10points.gnuplot


clean:
	rm -rf ${TARGET} ${FIGURES}
