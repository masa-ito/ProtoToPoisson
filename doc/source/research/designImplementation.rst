

Design and Implementation of our EDSL
=====================================


Mathematical semantics is missing in an object model.
---------------------------------------------------------



Object model
------------

* Grid 

  * Dividing a simulation region into cotrol volumes to descretize continuous data into discrete ones

* Mathematical operators

  * Such like partial differential and identity operators
  * Necessary to describe Poisson equation

* Iterative solvers and preconditioners

  * Wrapping linear algebra libraries of third parties 


Sematic model hierarchy
-------------------------

* Algorithm domain : Linear Algebra

  * Vector and matrix expressions
  * Solving descretized PDEs

* Model domain : FVM

  * Descretizing PDEs (partial differential equations)
  * This domain is defined with using the algorithm domain.


Semantic model for FVM
----------------------

* Transforming differential and identity operators into matrices
* Transforming spacial functions into vectors
* Parallelizing serial algorithm with OpenMP and MPI based on a user given model
  
  * Restructuring data structure to distribute simulation data to processors, based on the user specified distribution of the control volumes
  * Reordering indices for matrices and vectors, based on the connectivity of the control volumes
  * Combining parallel skeltons (higher-order functions), basic code fragments, and directives

