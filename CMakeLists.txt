cmake_minimum_required(VERSION 3.9) # 3.9 To rely on modern OpenMP support
project(test_pybind_mpi)

find_package(PythonLibs)
include_directories(${PYTHON_INCLUDE_DIRS})

find_package(OpenMP)
find_package(MPI REQUIRED)

set(CMAKE_CXX_COMPILE_FLAGS ${CMAKE_CXX_COMPILE_FLAGS} ${MPI_COMPILE_FLAGS})
set(CMAKE_CXX_LINK_FLAGS ${CMAKE_CXX_LINK_FLAGS} ${MPI_LINK_FLAGS})

include_directories(${MPI_INCLUDE_PATH})


set(PATH_TO_PYBIND "/root/pacs_mnt/pyfex/pybind11")
include_directories(SYSTEM pybind11/include)
include_directories(SYSTEM $ENV{mkPythonInc})
add_subdirectory(${PATH_TO_PYBIND} ../)

pybind11_add_module(mpi_lib mpi_lib.cpp)
target_link_libraries(mpi_lib PRIVATE mpi)
if(OpenMP_CXX_FOUND)
  target_link_libraries(mpi_lib PUBLIC OpenMP::OpenMP_CXX)
else()
  message( FATAL_ERROR "Your compiler does not support OpenMP" )
endif()

file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/helloWorld.py
  DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/)

