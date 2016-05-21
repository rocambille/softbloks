Softbloks Version 0.2 19/02/2016
================================

README contents
---------------

* What is Softbloks?
* How to get the source code?
* How the repository is structured?
* How to set up the build system?
* How to run a build?
* How to help this project?
* Links
* Licensing

What is Softbloks?
------------------

Softbloks is a free/libre cross-platform application framework written in C++.

The aim of Softbloks is to provide a continuum between bottom-up and top-down
design.

### The bottom-up-top-down continuum ###

In a perfect world, a developer will always be provided with a perfect
functional definition from which (s)he will gain a perfect understanding of
the system (s)he has to develop. From this perfect understanding (s)he will be
able to design and implement a perfect architecture with perfectly well
designed and reusable modules. Obviously the architecture will be perfect on
the first try. That's for the perfect world : the world of the top-down
design.

In the real world, you may be provided with a barely usable, day by day
changing definition of your project. On the first try, you may want to develop
your own application from scratch or from esoteric existing code to first
master the project: some kind of bottom-up design. What to do next? You have
an existing, in some way working code: how to reformat your code into a new,
better designed application?

Whatever your code is doing, as a C++ program it must declare a main function:

```c++
int main(...)
{
    // do something

    return 0;
}
```

The quick way to integrate your code into Softbloks is to replace your main
function with a *module* declaration:

```c++
#include "sb-core.h"

SB_MODULE(/*a description should go here*/)
{
    // do something
}
```

Note that your code should now be compiled as a shared library instead of an
executable. Such a module can be executed like a regular program with our
utility *Softrun* (run the executable without parameters for usage
description).

Maybe your architecture defines some kind of main object, e.g. to handle
initialization and finalization or to manage global resources, like in:

```c++
class MainObject
{

public:

    MainObject()
    {
        // do something
    }

};

int main(...)
{
    MainObject main_object;

    return 0;
}
```

Such a main object may be converted into a *soft* whith the following
modifications:

```c++
// 1) inherit the soft interface

class MainObject : public sb::AbstractSoft
{

    // 2) associate a name to the class

    SB_NAME("MainObject")

public:

    MainObject()
    {
        // do something
    }

};

SB_MODULE(/*a description should go here*/)
{
    // 3) register your object when declaring your module

    sb::register_object<MainObject>();
}
```

A soft can be instantiated using Softrun with option `--create`. Our second
utility, called *Software*, can be used to run softs in a Qt-based GUI
environment. To be usable with Software, your soft should declare a property
with the name *Qt.mainview* which gives access to a Qt-based view, the GUI of
your soft:

```c++
class MainObject : public sb::AbstractSoft
{

    SB_NAME("MainObject")

    // 1) declare a property Qt.mainview bound to the method get_qt_mainview()

    SB_PROPERTIES({
        "Qt.mainview",
        &MainObject::get_qt_mainview
    })

public:

    MainObject()
    {
    }

    // 2) implement the accessor

    QWidget* get_qt_mainview() const
    {
        return new QLabel("Hello World!!!");
    }

};

SB_MODULE(/*a description should go here*/)
{
    sb::register_object<MainObject>();
}
```

So begins the continuum. You can develop your own soft from scratch or using
available tools and, once your structures have emerged, transform them into
*bloks*:

```c++
// Foo is a mature object: it's time to make a blok

class Foo : public sb::AbstractSource // inherit a blok interface
{

    // declare the class

    SB_NAME("Foo")

public:

    Foo()
    {
        cout << "Hello world!!!" << endl;
    }

};

// Bar is still a work in progress: let it be

class Bar
{

public:

    Bar()
    {
        cout << "What's going on?" << endl;
    }

};

// MainObject can mix bloks with classical objects

class MainObject : public sb::AbstractSoft
{

    SB_NAME("MainObject")

public:

    MainObject()
    {
        // create components

        auto foo = sb::create_unique_source(
            Foo::get_type_name()
        );

        Bar bar;
    }

};

SB_MODULE(/*a description should go here*/)
{
    // remember to register all your objects when declaring your module

    sb::register_object<Foo>();
    sb::register_object<MainObject>();
}
```

### Why is it called *Softbloks*? ###

The name was inspired by [Phonebloks](http://phonebloks.com), a modular
smartphone concept. The aim of Softbloks is to provide a modular framework for
software development. We --- the developers of Softbloks --- are not part of
the Phonebloks team. We just found the concept inspiring and the Phonebloks
team kindly allowed us to use a *bloks* name.

How to get the source code?
---------------------------

Suppose [Git](http://git-scm.com/downloads) is installed on your system and
you want to download Softbloks in directory `<root>`, you can clone the
repository from GitHub using the following commands in a Git console:

    cd <root>
    git clone git://github.com/rita-marylin-raquel/softbloks.git .

How the repository is structured?
---------------------------------

The branching model is based on Vincent Driessen's
[git flow](http://nvie.com/posts/a-successful-git-branching-model/) which
defines among others the following branches:

* *master* which is the main branch for the stable releases;
* *develop* which is an integration branch for the latest and potentially
  unstable development.

The repository uses the following directory structure:

    <root>/
        3rdparty/
            (third party libraries)
        share/
            cmake/
                (config files for cmake packages)
            doc/
                (documentation generated with Doxygen)
            examples/
                (source files of the examples)
        src/
            (source files of softbloks)
        CMakeLists.txt  (top CMakeLists.txt of the project)
        COPYING         (copy of the GNU GPL)
        COPYING.LESSER  (copy of the GNU LGPL)
        README.md       (this file)

How to set up the build system?
-------------------------------

System requirements:

* [CMake](http://cmake.org/) 3.1.0 or newer;
* A C++11-standard-compliant compiler (compliance is tested with CMake using
  [target_compile_features()](http://cmake.org/cmake/help/v3.1/command/target_compile_features.html)).

Optional requirements:

* To build Qt5-based components: [Qt](http://www.qt.io/) 5.2 or newer.

Given `<root>` is the directory where you cloned the sources, we recommend to
create a separate directory for the build, e.g. `<root>/build`:

    cd <root>
    cmake -E make_directory build
    cd build

You can then run the following command to generate build files:

    cmake -D CMAKE_INSTALL_PREFIX="<install_prefix>" -G "<generator>" <root>

Where `<install_prefix>` is the path to an installation directory
(e.g. `<root>/install`) and `<generator>` is one of the build system generator
provided by CMake (e.g. "Visual Studio 12 2013" or "Unix Makefiles").

You can enable the compilation of the examples by adding the following option:

    cmake ... -D BUILD_EXAMPLES=ON ...

You can enable the compilation of the unit tests by adding the following
option:

    cmake ... -D BUILD_TESTING=ON ...

Suppose you installed Qt in directory `<Qt_install_prefix>`, you can enable
the compilation of the Qt-based components by adding the following options:

    cmake ... -D Qt5_DIR=<Qt_install_prefix>/lib/cmake/Qt5 -D BUILD_SOFTWARE=ON -D BUILD_QT5_EXAMPLES=ON ...

How to run a build?
-------------------

Once you generated the build files for your system (cf. previous section), the
following command can be used to make and install a release build:

    cmake --build . --target install --config Release

Files are installed according to the following directory structure:

* On Windows systems:

        <install_prefix>/
            sb*n*/
                bin/
                    (executables and shared libraries)
                include/
                    (include files)
                lib/
                    (import libraries)
                share/
                    cmake/
                        (config-file packages for cmake)
                    doc/
                        (documentation generated with Doxygen)
                    modules/
                        (loadable modules)
                COPYING
                COPYING.LESSER
                README.md

* On Unix systems:

        <install_prefix>/
            bin/
                sb*n*/
                    (executables)
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
                        (documentation generated with Doxygen)
                    modules/
                        (loadable modules)
                    COPYING
                    COPYING.LESSER
                    README.md

Where *n* is the current major version number.

How to help this project?
-------------------------

You can help the project in many ways, but basically you can tell us what's
wrong with it.
Tell us what goes wrong when you build the code.
Tell us about that missing feature or about the bad documentation.
By the way, if you like the project, please tell your friends.

Links
-----

Web site: <http://github.com/rita-marylin-raquel/softbloks>  
E-mail: <rita.marylin.raquel@gmail.com>

Cite us: [![DOI](https://zenodo.org/badge/20687/rita-marylin-raquel/softbloks.svg)](https://zenodo.org/badge/latestdoi/20687/rita-marylin-raquel/softbloks)

Licensing
---------

Copyright (C) 2014-2015 Bastien Oudot and Romain Guillemot

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
