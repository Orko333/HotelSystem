# Distributed under the OSI-approved BSD 3-Clause License.  See accompanying
# file Copyright.txt or https://cmake.org/licensing for details.

cmake_minimum_required(VERSION 3.5)

file(MAKE_DIRECTORY
  "D:/other/Project_on_c++/build/_deps/sqlitecpp-src"
  "D:/other/Project_on_c++/build/_deps/sqlitecpp-build"
  "D:/other/Project_on_c++/build/_deps/sqlitecpp-subbuild/sqlitecpp-populate-prefix"
  "D:/other/Project_on_c++/build/_deps/sqlitecpp-subbuild/sqlitecpp-populate-prefix/tmp"
  "D:/other/Project_on_c++/build/_deps/sqlitecpp-subbuild/sqlitecpp-populate-prefix/src/sqlitecpp-populate-stamp"
  "D:/other/Project_on_c++/build/_deps/sqlitecpp-subbuild/sqlitecpp-populate-prefix/src"
  "D:/other/Project_on_c++/build/_deps/sqlitecpp-subbuild/sqlitecpp-populate-prefix/src/sqlitecpp-populate-stamp"
)

set(configSubDirs )
foreach(subDir IN LISTS configSubDirs)
    file(MAKE_DIRECTORY "D:/other/Project_on_c++/build/_deps/sqlitecpp-subbuild/sqlitecpp-populate-prefix/src/sqlitecpp-populate-stamp/${subDir}")
endforeach()
if(cfgdir)
  file(MAKE_DIRECTORY "D:/other/Project_on_c++/build/_deps/sqlitecpp-subbuild/sqlitecpp-populate-prefix/src/sqlitecpp-populate-stamp${cfgdir}") # cfgdir has leading slash
endif()
