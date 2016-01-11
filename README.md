# mte7101-proj1
## Volume ray marching - basics & principles

A project work showing the basics & principles of volume ray marching
and especially a procedure known as sphere tracing.

Additionally a prototype based on C++ and OpenGL respectively GLSL was
developed, showing sphere tracing running in real time on the GPU.

This project work was developed within the scope of the module MTE-7101
- 'Vertiefungsprojekt 1' at the University of Applied Science in Bern
and was supervised by Professor Jean-Claude Fuhrer. Details see
http://www.ti.bfh.ch/fileadmin/modules/MTE7101-de.xml

It was written and developed in the autumn semester 2015 and is licensed
under the Creative Commons Attribution-ShareAlike 3.0 License.

The software is licensed under the conditions of the MIT License (MIT).

Directory Structure
-------------------

    bin/   -- Compiled, binary file of the prototype
    build/ -- Temporary directory for build output
    data/  -- GLSL shader files
    doc/   -- Documentation
        img/       -- Images used for the documentation
        inc/       -- LaTeX files used for inclusion to maintain
                      readability and managabilty
            static/ -- Static files used for inclusion, e.g. the
                       bibliography, versioning of the document
                       and so on
                    attachment/     -- Attachments for the documentation,
                                       e.g. the minutes of the held meetings
    inc/   -- External source files for inclusion, needed by the
              prototype
    lib/   -- External libraries for linking against when building
              the prototype
    src/   -- Source code of the prototype
