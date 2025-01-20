# Distributed under the OSI-approved BSD 3-Clause License.  See accompanying
# file Copyright.txt or https://cmake.org/licensing for details.

cmake_minimum_required(VERSION 3.5)

# If CMAKE_DISABLE_SOURCE_CHANGES is set to true and the source directory is an
# existing directory in our source tree, calling file(MAKE_DIRECTORY) on it
# would cause a fatal error, even though it would be a no-op.
if(NOT EXISTS "D:/joshua_aroka/github_repository_projects/projects/unity_school_assignment/Project2/BlockKuzushi_Joshua_Aroka/BlockKuzushi/cmake-build-debug/_deps/raylib-src")
  file(MAKE_DIRECTORY "D:/joshua_aroka/github_repository_projects/projects/unity_school_assignment/Project2/BlockKuzushi_Joshua_Aroka/BlockKuzushi/cmake-build-debug/_deps/raylib-src")
endif()
file(MAKE_DIRECTORY
  "D:/joshua_aroka/github_repository_projects/projects/unity_school_assignment/Project2/BlockKuzushi_Joshua_Aroka/BlockKuzushi/cmake-build-debug/_deps/raylib-build"
  "D:/joshua_aroka/github_repository_projects/projects/unity_school_assignment/Project2/BlockKuzushi_Joshua_Aroka/BlockKuzushi/cmake-build-debug/_deps/raylib-subbuild/raylib-populate-prefix"
  "D:/joshua_aroka/github_repository_projects/projects/unity_school_assignment/Project2/BlockKuzushi_Joshua_Aroka/BlockKuzushi/cmake-build-debug/_deps/raylib-subbuild/raylib-populate-prefix/tmp"
  "D:/joshua_aroka/github_repository_projects/projects/unity_school_assignment/Project2/BlockKuzushi_Joshua_Aroka/BlockKuzushi/cmake-build-debug/_deps/raylib-subbuild/raylib-populate-prefix/src/raylib-populate-stamp"
  "D:/joshua_aroka/github_repository_projects/projects/unity_school_assignment/Project2/BlockKuzushi_Joshua_Aroka/BlockKuzushi/cmake-build-debug/_deps/raylib-subbuild/raylib-populate-prefix/src"
  "D:/joshua_aroka/github_repository_projects/projects/unity_school_assignment/Project2/BlockKuzushi_Joshua_Aroka/BlockKuzushi/cmake-build-debug/_deps/raylib-subbuild/raylib-populate-prefix/src/raylib-populate-stamp"
)

set(configSubDirs )
foreach(subDir IN LISTS configSubDirs)
    file(MAKE_DIRECTORY "D:/joshua_aroka/github_repository_projects/projects/unity_school_assignment/Project2/BlockKuzushi_Joshua_Aroka/BlockKuzushi/cmake-build-debug/_deps/raylib-subbuild/raylib-populate-prefix/src/raylib-populate-stamp/${subDir}")
endforeach()
if(cfgdir)
  file(MAKE_DIRECTORY "D:/joshua_aroka/github_repository_projects/projects/unity_school_assignment/Project2/BlockKuzushi_Joshua_Aroka/BlockKuzushi/cmake-build-debug/_deps/raylib-subbuild/raylib-populate-prefix/src/raylib-populate-stamp${cfgdir}") # cfgdir has leading slash
endif()
