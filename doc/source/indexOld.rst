.. ProtoToPoisson documentation master file, created by
   sphinx-quickstart on Fri Feb 12 14:27:34 2016.
   You can adapt this file completely to your liking, but it should at least
   contain the root `toctree` directive.

Welcome to ProtoToPoisson's documentation!
==========================================

ProtoToPoisson is a research project to design and implement a mini-EDSL (embedded domain-specific language) for developing and parallelizing Poisson equation solvers. 
The implementation of our EDSL is based on Boost.Proto library and other open-source ones.


Goal setting of this project
----------------------------

Actually my collaborators and I have been considering about two different goals of this project, one is what one of my collaboraters likes to set and the other is what I like to set.
    His goal :
        To see if the executable code of Poisson eq. solver from our EDSL can run faster.

    My goal :
        To see if our EDSL can generate low-level parallelized code from user dinfed model. 


Research background
^^^^^^^^^^^^^^^^^^^

But whichever goal we try to achieve, we have beforehand to clarify the research background.

I think the background can be specified by answering to the following thress questions :

+------------------+-----------------------------+---------------------------+
| Questions        | The background of his goal  |  That of mine             |
+==================+=============================+===========================+
|1. To what        |  ?                          | Design of higher-level(**)|
|research field    |                             | software framework        |
|does our project  |                             | for a scientific          |
|belong?           |                             | application domain        |
+------------------+-----------------------------+---------------------------+
|2. In that        | ?                           | a. EDSLs for finite       |
|research field,   |                             | element method            |
|what works        |                             |                           |
|are related to    |                             | b. Meta-programming       |
|ours?             |                             | techniques to transform   |
|                  |                             | higher-level( * ) model   |
|                  |                             | algorithm into            |
|                  |                             | low-level ( ** ) code     |
+------------------+-----------------------------+---------------------------+
|3. In the         | The executable code of      | Our EDSL can generate     |
|boundaries of     | Poisson eq. solver from     | low-level parallelized    |
|those related     | our EDSL can run faster     | code from higher-level    |
|works, what       | than that from other        | model and algorithm       |
|research          | possible implementations of | without siginificant      |
|hypothesis can    | EDSL .                      | performance penalty.      |
|be our target?    |                             |                           |
+------------------+-----------------------------+---------------------------+

( * ) 
    The term 'higher-level' means the software layer closely related to an application domain.

( ** ) 
    The term 'low-level' means the software layer closely related to hardware.

Note that testing one of the hypotheses in the last answer can be a research goal.


Approach to my goal
^^^^^^^^^^^^^^^^^^^

With the background shown in the above, I am writing code to test my hypothesis.



.. toctree::
   :maxdepth: 2




Aproach to his goal
^^^^^^^^^^^^^^^^^^^

Now thinking about what is the background of that research hypothesis ...




Project period
^^^^^^^^^^^^^^

Jan. 2015 - Sep. 2016 (or until my temporary transfer from Fujitsu to JAXA (Japan Aerospace Exploration Agency) ends.)





1. To what research field does our project belong to?
------------------------------------------------
software frameworks for scientific simulations


The goal of this sub-project
-----------------------------
To test the following research hypothesis :

* The designing way of EDSLs for FEM (finite element mothod) can be generalized and can be used to design an EDSL for FVM.







The purpose and hypothesis in the research field 
---------------------------------------------

Purpose
    To help (computational) scientists to focus on simulation models and algorithms by abstracting low level parts of simulation software

        * Actually this is a part of the research purpose of software engineering. It is to help software developers to focus on models and designs by abstracting implementation details.

Hypothesis
    Using an appropriate DSL for your simulation method, you can make a simulation program, as you make a simulation model and algorithm.


How to test the research hypothesis?
---------------------------------------------

  * Using our EDSL, you can make a Poisson equation solver just by describing a FVM model for discretizing that differential equation and by specifing an iterative method algorithm for solving the discretized equation. 
  * Our EDSL can automatically parallelize your Poisson equation solver by restructuring the data structure of your solvers based on your FVM model and by generating low-level code for OpenMP and MPI from your specification of an iterative method algorithm.


Related projects in my research
----------------------------------
This project is a sub-project within my research about a EDSL for FVM (finite volume method), whose project name in GitHub is 'ProtoFVM'.

Project hierarchy
* ProtoFVM

  * ProtoToCFD - for Euler equation solvers (and Navier-Stokes equation sovlers)
  * **ProtoToPoisson** (*this sub-project*) -- for Poisson equation solvers

    * ProtoToMET - for iterative methods for solving linear equations




Indices and tables
==================

* :ref:`genindex`
* :ref:`modindex`
* :ref:`search`

