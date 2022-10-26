#ifdef _OPENMP
#include <omp.h>
#endif
#include <mpi.h>
#include <pybind11/pybind11.h>
#include <pybind11/numpy.h>
#include <stdio.h>

namespace py = pybind11;
using pymod = pybind11::module;

class Distributed
{
public:
  Distributed() : comm_global(MPI_COMM_WORLD) {
  	MPI_Comm_size(comm_global, &world_size);
  }
  
  ~Distributed() {}
  
  void say_hi() {
    int world_size;
    MPI_Comm_size(comm_global, &world_size);
    int world_rank;
    MPI_Comm_rank(comm_global, &world_rank);
    char processor_name[MPI_MAX_PROCESSOR_NAME] = "localhost";
    int name_len;
    MPI_Get_processor_name(processor_name, &name_len);
    printf("[C++]    Hello from machine %s, MPI rank %d out of %d\n",
	   processor_name,
	   world_rank,
	   world_size);
    if (world_rank == 0) {
      int thread_level;
      MPI_Query_thread( &thread_level );
      switch (thread_level) {
      case MPI_THREAD_SINGLE:
	printf("Detected thread level MPI_THREAD_SINGLE\n");
	fflush(stdout);
	break;
      case MPI_THREAD_FUNNELED:
	printf("Detected thread level MPI_THREAD_FUNNELED\n");
	fflush(stdout);
	break;
      case MPI_THREAD_SERIALIZED:
	printf("Detected thread level MPI_THREAD_SERIALIZED\n");
	fflush(stdout);
	break;
      case MPI_THREAD_MULTIPLE:
	printf("Detected thread level MPI_THREAD_MULTIPLE\n");
	fflush(stdout);
	break;
      }
      int nthreads, tid;
#pragma omp parallel private(nthreads, tid)
      {
	
	/* Obtain thread number */
	tid = omp_get_thread_num();
	printf("Hello World from thread = %d\n", tid);
	
	/* Only master thread does this */
	if (tid == 0 ) 
	  {
	    nthreads = omp_get_num_threads();
	    printf("Number of threads = %d\n", nthreads);
	  }
      }
    }
  }

  int basic_operation(int i, int j) {
    int sum = 0;
    for(int l = 0; l < i; l++){
        sum = sum + j;
    }
    return sum;
  }

  int mpi_basic_operation(int i, int j){
    int local_sum = 0;
    int total = 0;
    for(int l = 0; l < floor(i/world_size+1); l++){
        local_sum = local_sum + j;
    }
    MPI_Reduce (&local_sum, &total, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);
    return total;
  }

private:
	int world_size;

  MPI_Comm comm_global;
};



PYBIND11_MODULE(mpi_lib, mmod)
{
  constexpr auto MODULE_DESCRIPTION = "Just testing out mpi with python.";
  mmod.doc() = MODULE_DESCRIPTION;
  
  py::class_<Distributed>(mmod, "Distributed")    
    //.def(py::init<py::object &>())
    .def(py::init<>())
    .def("say_hi", &Distributed::say_hi, "Each process will say hi")
    .def("basic_operation", &Distributed::basic_operation, "serial sum")
    .def("mpi_basic_operation", &Distributed::mpi_basic_operation, "parellel sum");
  
}
