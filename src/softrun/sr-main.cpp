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

    std::vector<std::string> parsed_module_paths;

    std::string name;

    if(argc_ > 1)
    {
        invalid_argument = false;

        // parse arguments

        for(int i(1); !invalid_argument && i < argc_; ++i)
        {
            if(std::string(argv_[i]) == "--module-path")
            {
                if(
                    i+1 < argc_ &&
                    std::string(argv_[i+1]) != "--"
                )
                {
                    parsed_module_paths.push_back(
                        std::string(argv_[++i])
                    );
                }
                else
                {
                    invalid_argument = true;
                }
            }
            else if(std::string(argv_[i]) == "--")
            {
                if(i+1 < argc_)
                {
                    name = std::string(argv_[++i]);
                }
                else
                {
                    invalid_argument = true;
                }
            }
            else if(i+1 == argc_)
            {
                name = std::string(argv_[i]);
            }
            else
            {
                invalid_argument = true;
            }
        }
    }

    if(name.empty())
    {
        invalid_argument = true;
    }

    if(invalid_argument)
    {
        // display usage

        std::cout <<
            "usage: " <<
            argv_[0] <<
            " [options] [--] <name>" <<
            std::endl;
    }
    else
    {
        // pre-process module paths

        std::vector<std::string> processed_module_paths;

        for(std::string path : parsed_module_paths)
        {
            sr::Directory directory(path);

            if(directory.exists())
            {
                // the path is a directory : replace with the contained files

                sr::Directory::EntryList entry_list =
                    directory.get_entry_list();

                for(auto entry : entry_list)
                {
                    processed_module_paths.push_back(
                        directory.get_path() + entry
                    );
                }
            }
            else
            {
                // the path may be a file : just append it

                processed_module_paths.push_back(path);
            }
        }

        // load modules

        for(std::string path : processed_module_paths)
        {
            sr::FunctionPointer sb_main = sr::Library::resolve(
                path,
                "sb_main"
            );

            if(sb_main)
            {
                sb_main();
            }
        }

        // run wanted soft

        {
            auto soft = sb::create_shared_soft(name);
        }

        // make sure to forget dynamically loaded factories,
        // before libraries are unloaded

        sb::unregister_all_objects();
    }

    return 0;
}
