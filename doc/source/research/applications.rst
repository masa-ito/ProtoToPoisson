
Application Examples
====================

1D steady state heat conduction
-------------------------------

Cooling of a cylinder by means of convective heat transfer along its length

* https://github.com/masa-ito/ProtoToPoisson/blob/master/src/test/airCooledCylinder.cpp

* Its compiled code reproduces the example calculation in a finite volume method textbook, 

  H. K. Versteeg and W. Malalasekera,"An Introduction to Computational Fluid Dynamics, The Finite Volume Method", 2nd Ed. Pearson Educational Limited 1995, 2007, page 125 - 129, example 4.3 .
 

3D steady state heat conduction
-------------------------------

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

