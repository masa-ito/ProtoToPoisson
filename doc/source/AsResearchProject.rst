
ProtoToPoisson as a Research Project
====================================

Problems we are trying to resolve

    A conflict between expressiveness of simulation source code and performace of its executable

    * Object-oriented programming (OOP) language like C++ can enhance the expressiveness of user's source code, so that they can focus on their simulation model.
    * But OOP style in C++ sometimes affects the performace, so that scientists have given up expressiveness and written low-level code.
    * That low-level coding prevents them from focusing on their model, so that the cost of prototyping a new model is increased.

Project Goal

    To provide an expressive EDSL(embedded domain-specific language) with automatic parallelization

    * Expressiveness : Our EDSL is going to translate user's description of their model into low-level code, so that they can focus on their model.
    * Performance : Our EDSL is going to automatically parallelize simulation algorithm based on user given model.


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
   * EDSLs for FEM(finite element method) provide good semantic models, so that one can describe her/his simulation model in concise code.



Design and implementation of an EDSL for FVM (finite volume method)
-------------------------------------------------------------------

Features of problem domains

   * Algorithm domain : Linear Algebra

     * Vector and matrix expressions
     * Solving descretized PDEs

   * Model damain : FVM

     * Descretizing PDEs (partial differential equations)
     * This domain is defined with using the algorithm domain.


Front Ends -- Utility classes

    * Grid 

      * Dividing a simulation region into cotrol volumes to descretize continuous data into discrete ones

    * Mathematical operators

      * Such like partial differential and identity operators
      * Necessary to describe Poisson equation

    * Iterative solvers and preconditioners

      * Wrapping linear algebra libraries of third parties 


Back Ends -- Semantic model

   * Transforming differential and identity operators into matrices
   * Transforming spacial functions into vectors
   * Parallelizing serial algorithm with OpenMP and MPI based on a user given model
     
     * Restructuring data structure to distribute simulation data to processors, based on the user specified distribution of the control volumes
     * Reordering indices for matrices and vectors, based on the connectivity of the control volumes
     * Combining parallel skeltons (higher-order functions), basic code fragments, and directives



Sample code written in our EDSL
------------------------------

1. 1D steady state heat conduction

   Cooling of a cylinder by means of convective heat transfer along its length

   * https://github.com/masa-ito/ProtoToPoisson/blob/master/src/test/airCooledCylinder.cpp

   * Its compiled code reproduces the example calculation in a finite volume method textbook, 

     H. K. Versteeg and W. Malalasekera,"An Introduction to Computational Fluid Dynamics, The Finite Volume Method", 2nd Ed. Pearson Educational Limited 1995, 2007, page 125 - 129, example 4.3 .
 

2. 3D steady state heat conduction

   Cooling of a cuboid with internal heat generation

.. math::   k \nabla^2 T(x,y,z) + g | x + y | = 0
.
   where
     * Thermal conductivity : :math:`k`
     * Heat generation rate : :math:`g | x + y |`

   The cuboid size

.. math:: L_x, L_y, L_z
.
   Boundary conditions :

.. math:: \frac{\partial T}{\partial x} = 0 \ \mbox{for} \ x= 0, L_x

.. math:: \frac{\partial T}{\partial y} = 0 \ \mbox{for} \ y = 0, L_y

.. math:: \frac{\partial T}{\partial z} = 0 \ \mbox{for} \ z = 0

.. math:: T(x,y, L_z) = T_c



Benchmarks
----------

* Scalability comparison

   * Refererence program

      * Its hot spot code is rewritten in plain-C style.

   * Manually parallelized program
   * Automatically parallelized program with our EDSL

     * OpenMP
     * MPI

* Hardware architecture comparison

  * PC cluster
  * Fujitsu supercomputer PRIMEHPC FX100

    http://www.fujitsu.com/global/products/computing/servers/supercomputer/primehpc-fx100/

* C++ compiler comparison

  * GNU
  * Intel
  * Fujitsu



ChangeLog
-------------

Feb. 5. 2016
    Sample code 1 was confirmed to work.




