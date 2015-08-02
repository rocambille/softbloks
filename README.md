Softbloks Version 0.0 30/12/2014
================================

What is Softbloks?
------------------

Softbloks is a free --- in the sense of *free speech*, not of *free beer* ---
cross-platform SDK inspired by [Phonebloks](http://phonebloks.com), a modular
smartphone concept. The aim of Softbloks is to provide a modular design for
software development. We --- the developpers of Softbloks --- are not part of
the Phonebloks team. We just found the concept inspiring and the Phonebloks
team kindly allowed us to use a *bloks* name.

Softbloks is developped in C++.

Building Softbloks from sources
-------------------------------

System requirements:

* CMake 3.0.0 or later;
* C++ compiler supporting the C++11 standard.

If you are reading this part, we assume you cloned Softbloks from the
[official repository](http://github.com/rita-marylin-raquel/softbloks). The
branching model is based on Vincent Driessen's
[git flow](http://nvie.com/posts/a-successful-git-branching-model/) where the
*master* branch is used to store stable releases.

Assuming **source_directory** is the directory where you cloned the sources,
run the following command from a shadow-build directory
(e.g. **source_directory/build**) to generate build files:

    cmake -D CMAKE_INSTALL_PREFIX=<install_prefix> -G "<generator>" <source_directory>

Where **install_prefix** is the path to your installation directory
(e.g. **source_directory/install**) and **generator** is one of the build
system generator provided by CMake (e.g. "Visual Studio 12 2013" or
"Unix Makefiles"). Use then the following command to make a release build:

    cmake --build . --target install --config Release

Directory Structure
-------------------

Files are installed according to the following directory structure:

* On Windows systems:

        <install_prefix>/
            sb*n*/
                bin/
                    (software and shared libraries)
                include/
                    (include files)
                lib/
                    (import libraries)
                share/
                    cmake/
                        (config-file packages for cmake)
                    doc/
                        (documentation generated from include files)
                    modules/
                        (loadable modules)
                COPYING
                COPYING.LESSER
                README.md

* On Unix systems:

        <install_prefix>/
            bin/
                sb*n*/
                    (software)
            include/
                sb*n*/
                    (include files)
            lib/
                sb*n*/
                    (shared libraries)
            share/
                sb*n*/
                    cmake/
                        (config-file packages for cmake)
                    doc/
                        (documentation generated from include files)
                    modules/
                        (loadable modules)
                    COPYING
                    COPYING.LESSER
                    README.md

Where *n* is the current major version number.

Links
-----

Web site: <http://github.com/rita-marylin-raquel/softbloks>  
E-mail: <rita.marylin.raquel@gmail.com>

Licensing
---------

Copyright (C) 2014 Bastien Oudot

Softbloks is free software: you can redistribute it and/or modify
it under the terms of the GNU Lesser General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

Softbloks is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public License
along with Softbloks.  If not, see <http://www.gnu.org/licenses/>.
