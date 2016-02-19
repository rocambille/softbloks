/*
Copyright (C) 2014-2015 Bastien Oudot and Romain Guillemot

This file is part of Softbloks.
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
*/
#include <sb-core/sb-core.h>

#include "sr-directory.h"
#include "sr-library.h"

#include <iostream>

int
main
(
    int argc_,
    char** argv_
)
{
    bool invalid_argument = true;
    bool reached_end_of_options = false;
    bool get_descriptors = false;

    std::vector<std::string> parsed_names;
    std::vector<std::string> parsed_module_paths;

    if(argc_ > 1)
    {
        invalid_argument = false;

        // parse arguments

        for(int i(1); !invalid_argument && i < argc_; ++i)
        {
            std::string current = std::string(argv_[i]);

            bool parsed = true;

            if(!reached_end_of_options)
            {
                // check if current is an option

                if(current == "--get-descriptors")
                {
                    get_descriptors = true;
                }
                else if(current == "--create")
                {
                    // next argument should be a valid name for an object to
                    // create

                    if(
                        // check there is a next argument
                        i+1 < argc_ &&
                        // check the next argument is not an option
                        std::string(argv_[i+1])[0] != '-'
                            // any argument given by the system should contain
                            // at least 1 character?
                    )
                    {
                        parsed_names.push_back(
                            std::string(argv_[++i])
                        );
                    }
                    else
                    {
                        invalid_argument = true;
                    }
                }
                else
                {
                    parsed = false;
                }
            }

            if(!parsed)
            {
                if(current == "--")
                {
                    reached_end_of_options = true;
                }
                else
                {
                    parsed_module_paths.push_back(
                        current
                    );
                }
            }
        }
    }

    if(parsed_module_paths.empty())
    {
        invalid_argument = true;
    }

    if(invalid_argument)
    {
        // display usage

        std::cout <<
            "usage: " <<
            argv_[0] <<
            " [--get-descriptors]" <<
            " [--create <name>]" <<
            " [--] <paths>" <<
            std::endl;

        // TODO : add explanations for options
    }
    else
    {
        // process module paths

        std::vector<std::string> processed_module_paths;

        for(std::string path : parsed_module_paths)
        {
            sr::Directory directory(path);

            if(directory.exists())
            {
                // the path is a directory : replace with the contained files
                // (maybe should add an option for some recursivity)

                sr::Directory::EntryList entry_list =
                    directory.get_entry_list();

                for(auto entry : entry_list)
                {
                    if(
                        entry != "." &&
                        entry != ".."
                    )
                    {
                        processed_module_paths.push_back(
                            directory.get_path() + entry
                        );
                    }
                }
            }
            else
            {
                // the path may be a file : just append it

                processed_module_paths.push_back(path);
            }
        }

        // we are ready

        if(get_descriptors)
        {
            // print descriptors

            for(std::string path : processed_module_paths)
            {
                using GetDescriptorFunctionPointer = const char*(*)();

                GetDescriptorFunctionPointer sb_get_module_descriptor =
                    reinterpret_cast<GetDescriptorFunctionPointer>(
                        sr::Library::resolve(
                            path,
                            "sb_get_module_descriptor"
                        )
                    );

                if(sb_get_module_descriptor)
                {
                    std::cout <<
                        "module: " <<
                        path <<
                        std::endl <<
                        sb_get_module_descriptor() <<
                        std::endl <<
                        std::endl;
                }
            }
        }
        else
        {
            // run modules

            for(std::string path : processed_module_paths)
            {
                std::cout <<
                    "running " <<
                    path <<
                    std::endl;

                sr::FunctionPointer sb_main = sr::Library::resolve(
                    path,
                    "sb_main"
                );

                if(sb_main)
                {
                    sb_main();

                    std::cout <<
                        "\tOK" <<
                        std::endl;
                }
                else
                {
                    std::cout <<
                        "\tFAILED" <<
                        std::endl;
                }
            }

            // create wanted objects

            for(std::string name : parsed_names)
            {
                std::cout <<
                    "creating " <<
                    name <<
                    std::endl;

                auto object = sb::create_unique_object(name);

                if(object)
                {
                    std::cout <<
                        "\tOK" <<
                        std::endl;
                }
                else
                {
                    std::cout <<
                        "\tFAILED" <<
                        std::endl;
                }
            }
        }

        // make sure to forget dynamically loaded factories,
        // before libraries are unloaded

        sb::unregister_all_objects();
    }

    return 0;
}
