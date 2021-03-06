
How to install ProtoToPoisson
=============================

Requried Software
-----------------

* Boost library
* ProtoToMET

    * https://github.com/masa-ito/ProtoToMET
    * This library provides an EDSL (embedded domain-specific language) for linear algebra which ProtoToPoisson needs.

        * Makefiles of ProtoToPoisson project assumes that ProtoToMET and ProtoToPoisson share the same parent directory, ${Installation Root}.

            * ${Installation Root}/

                * ProtoToMET/
                * ProtoToPoisson/

Installation steps
------------------

1. Install Boost library and make sure that Boost.Proto works.

    * http://www.boost.org/
    * The version of Boost.Proto should be equal to or later than 1.57.0 .

2. Install ProtoToMET ::

     cd ${Installation Root}
     git clone https://github.com/masa-ito/ProtoToMET.git
     cd ProtoToMET/src/DenseLinAlg
     make

3. Install ProtoToPoisson ::

     cd ${Installation Root}
     git clone https://github.com/masa-ito/ProtoToPoisson.git
     cd ProtoToPoisson/src/test
     make

4. Make sure that our sample program works. ::

   cd ${Installation Root}/ProtoToMET/src/test
   ./airCooledCylinder

* The output of the sample program should include ::

       # stationary temperature distribution
       #     x      FVM       exact
           0.10    64.228    68.526
           0.30    36.911    37.866
           0.50    26.504    26.611
           0.70    22.602    22.536
           0.90    21.301    21.216


