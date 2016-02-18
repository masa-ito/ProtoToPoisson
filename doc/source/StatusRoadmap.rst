
Current Status and Roadmap of ProtoToPoisson project
===================================================

Problems we are trying to resolve

    A conflict between expressiveness of simulation source code and performace of its executable

    * Even when scientist use C++, they have to write low-level code for avoiding performance penalty of abstracting low-level system, hardware. 
    * That low-level coding prevents them from focusing on their simulation model, so that the cost of prototyping a new model is increased.

Project Goal

    Providing an expressive EDSL(embedded domain-specific language) with a smaller conflict with performance

    * Our EDSL can translate user's description of their model into low-level code, so that they can prototype a new model with ease.
    * Our EDSL can automatically parallelize simulation algorithm based on user given model, so that they focus on the model with less performance penalty.


Research Background
-------------------

Key to resolve the conflict between expressiveness and performace 

   * ETs (expression templates) allow one to write expressive code with no significant performance penalty.

Low-level programming, however, still persists in scientific computing.

   * The abstraction cost of low-level code has not been decreased much, because ordinally ETs cannot optimize a complex kernel.
   * What is worse, they cannot provide good semantics for describing a simulation model in concise code.
   
Recent researches for resolving the conflict

   * Smart ETs for abstracting complex kernels
   * Parallel Skeltons for generating a parallelized program from domain-specific patterns and basic code fragments
   * EDSLs for FEM(finite element method) provide good semantic models, so that one can describe her/his simulation model in cocise code.



Design and implementation of an EDSL for FVM (finite volume method)
-------------------------------------------------------------------

Features of problem domains

   * Algorithm domain : Linear Algebra

     * Vector and matrix expressions
     * Solving descretized PDEs

   * Model damain : FVM

     * Descretizing PDEs (partial differential equations)
     * This domain is defined with using the algorithm domain.


Back Ends -- Semantic model



Front Ends -- Utility classes






Application of our EDSL
-----------------------


Benchmarks
----------

* Serial performance

* Parrallel performance

  * PC cluster

    * OpenMP
    * MPI

  * Fujitsu supercomputer PRIMEHPC FX100

    http://www.fujitsu.com/global/products/computing/servers/supercomputer/primehpc-fx100/


Acknowledgement
---------------

( JAXA JSS2 system )

