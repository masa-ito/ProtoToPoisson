
How to write documents in ProtoToPoisson project
==================================

**I'd really appreciate it if you would write document in this project for helping one to prototype their simulation model.**


Requried Software
-----------------

* Sphinx 

  * http://www.sphinx-doc.org/en/stable/
  * http://docs.sphinx-users.jp/contents.html ( in Japanese )

* platex ( only for those who can access to the private repositoy of our project. )


Making the html files of these github pages on your PC
------------------------------------------------------

1. Install Sphinx and make sure it works on your PC.

2. Compile the rst formatted files of our project ::

     cd ${project root}/ProtoToPoisson/doc
     make html

     # ${project root} = the root directory where you 
     #                   placed the files of ProtoToPoisson 
     #                   project
 
3. Open the html files in ${project root}/doc/build/html using a web browser 



Compiling the platex source files about our research project
-------------------------------------------------------------

Note : Since the copyright of some conference proceedings and research papers were transferred to academic societies, the platex source files of those papers are not placed at github.com .

1. Install platex and make sure it works on your PC.

2. Compile the platex source files ::

     cd ${project root}/ProtoToPoisson/doc/${Conference Name}/
     make 

3. Check the generated .dvi and .pdf files

