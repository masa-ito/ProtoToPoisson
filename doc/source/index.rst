.. ProtoToPoisson documentation master file, created by
   sphinx-quickstart on Fri Feb 12 14:27:34 2016.
   You can adapt this file completely to your liking, but it should at least
   contain the root `toctree` directive.

Welcome to ProtoToPoisson's documentation!
==========================================

Summary
-------

What is ProtoToPoisson? 

    * A mini-EDSL (embedded domain-specific language) for prototyping and parallelizing Poisson equation solvers with FVM (finite volume method)

Problems we are trying to resolve

    * Even when scientist use C++, they have to write low-level code for avoiding performance penalty of abstracting low-level system, hardware. 
    * That low-level coding prevents them from focusing on their simulation model, so that the cost of prototyping a new model is increased.

Project Goal

    * Our EDSL can translate user's description of their model into low-level code, so that they can prototype a new model with ease.
    * Our EDSL can automatically parallelize simulation algorithm based on user given model, so that they focus on the model with less performance penalty.


Project period
    * Jan. 2015 - Sep. 2016 (or until my temporary transfer from Fujitsu to JAXA (Japan Aerospace Exploration Agency) ends.)



Contents
========

.. toctree::
   :maxdepth: 2

   How to install <howToInstall>
   How to join <howToJoin>




Indices and tables
==================

* :ref:`genindex`
* :ref:`modindex`
* :ref:`search`

