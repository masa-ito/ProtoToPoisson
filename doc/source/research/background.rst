
Research Background of ProtoPoisson project
===========================================


A dilemma between expressiveness and performance within OOP limit
----------------------------------------------------------------------

 * OOP style code can be expressive for describing a solver algorithm, because it can encapsulate low-level arithmetics into linear-algebra-level obejcts.
 * But such encapsulation sometimes lowers the performace, so that not a few scientist have given up the expressiveness of OOP style to improve the performance.
 * Morever OOP style code in C++ is not so expressive for describing a discretization model, because the semantics of the operators and functions in a modeling method such like FVM is totally different from that in C++.


Expression templates made the dilemma false.
--------------------------------------------

* ETs (expression templates) allow one to use matrix and vector objects with no significant performance penalty.


Recent researches for more expressiveness and performance
----------------------------------------------------------

* Smart ETs for abstracting complex kernels
* Parallel Skeltons for generating a parallelized program from domain-specific patterns and basic code fragments
* EDSLs for FEM(finite element method) provide good semantic models, so that one can describe her/his simulation model in concise code.


Our research is for turning expressiveness into scalability.
----------------------------------------------------
is on expressiveness in a discretization model and parallel scalability.

