

Design and Implementation of our EDSL
=====================================


Domain hierarchy in scientific application programs
----------------------------------------------------

=============  ================================================
               In finite-volume-method-based applications
=============  ================================================
Science        Convection, Diffusion
**Model**      Discretization / Conversion of PDE to a set of 
               algebraic equations
**Algorithm**  Iterative method to solve algebraic equations / 
               preconditioners 
Software       Middleware, libraries for linear algebra , 
               for lower-level arithmetic operations, and
               for parallel programming
Hardware       Network , CPUs, cores, accelerators
=============  ================================================

Our EDSL helps computational scientists to focus on model and algorithm domain by abstracting software and hardware domains, when they develop application programs.


Object model
------------

Model concepts in FVM are represented by the objects our EDSL provides.

+----------------------------+-----------------------------------------------+
|Concepts                    | Objects                                       |
+============================+===============================================+
|Independent variables x,y,z | Grid                                          |
|Discretization              |                                               |
|                            | Iterator objects in a Grid object             |
|Boundary                    | Iterators for boundaries                      |
|                            | Iterators for grid points inside a boundary   |
+----------------------------+-----------------------------------------------+
|Boundary conditions         | Boundary Corrector                            |
+----------------------------+-----------------------------------------------+
|Dependent variables T       | ??? -> Expression templates -> Vector         |
|Function T(x,y,z)           |                                               |
+----------------------------+-----------------------------------------------+
|Mathematical Operators      | Operators -> Expression templates             |
|                            | -> Sparse Matrix                              |
+----------------------------+-----------------------------------------------+

Note that mathematical semantics is missing in an object model and is provided by the semantic model of our EDSL.


* Grid 

  * Dividing a simulation region into control volumes to discretize continuous data into discrete ones

* Mathematical operators

  * Such like partial differential and identity operators
  * Necessary to describe Poisson equation

* Iterative solvers and preconditioners

  * Wrapping linear algebra libraries of third parties 


Semantic model 
-------------------------

* Algorithm domain : Linear Algebra

  * Vector and matrix expressions
  * Solving discretized PDEs

* Model domain : FVM

  * Discretizing PDEs (partial differential equations)
  * This domain is defined with using the algorithm domain.



Semantic model for FVM
----------------------

* Transforming differential and identity operators into matrices
* Transforming spacial functions into vectors
* Parallelizing serial algorithm with OpenMP and MPI based on a user given model
  
  * Restructuring data structure to distribute simulation data to processors, based on the user specified distribution of the control volumes
  * Reordering indices for matrices and vectors, based on the connectivity of the control volumes
  * Combining parallel skeletons (higher-order functions), basic code fragments, and directives

