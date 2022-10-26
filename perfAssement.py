#!/usr/bin/env python3

import mpi4py
mpi4py.rc.threaded = True
mpi4py.rc.thread_level = "funneled"
from mpi4py  import  MPI
from mpi_lib import Distributed
import time
import matplotlib.pyplot as plt

def basic_operation(i, j):
    sum = 0;
    for l in range(0,i):
        sum = sum + j
    return sum;

def comparaison(distrib ,i,j, rank = 0):
    print("id after call", id(distrib))
    sum = 0
    py_time = 0
    cpp_time = 0
    mpi_time = 0

    start_time = time.time()
    sum = basic_operation(i, j)
    py_time = time.time() - start_time
    print("result :", sum, "python time",py_time)

    start_time = time.time()
    sum = distrib.basic_operation(i, j)
    cpp_time = time.time() - start_time
    print("result :", sum, "c++ time",cpp_time)

    start_time = time.time()
    sum = distrib.mpi_basic_operation(i, j)
    mpi_time = time.time() - start_time
    print("result :", sum, "mpi time",mpi_time)


    return py_time, cpp_time, mpi_time




# Main program that delegates some work to a C++/MPI routine: say_hi
def  main ():
    comm =   MPI.COMM_WORLD
    rank = comm.Get_rank ()
    size = comm.Get_size ()
    name = MPI.Get_processor_name()
    #print("[Python] Hello from machine " + name + ", MPI rank " + str( rank ) + " out of " + str( size ))
    distrib = Distributed()
    #distrib.say_hi()



    py_times = []
    cpp_times = []
    mpi_times = []
    iterations = [pow(10,i) for i in range(1,8)]
    for n in iterations:
        print("_________", n)
        print("id before call", id(distrib))
        py_time, cpp_time, mpi_time = comparaison(distrib, n, 2, rank)
        py_times.append(py_time)
        cpp_times.append(cpp_time)
        mpi_times.append(mpi_time)


    plt.plot(iterations, py_times, label = "python")
    plt.plot(iterations, cpp_times, label = "cpp")
    plt.plot(iterations, mpi_times, label = "mpi")
    plt.xlabel("number of operations")
    plt.ylabel("time")
    plt.legend();
    plt.savefig('./test.png')

    plt.clf()

    plt.plot(iterations, cpp_times, label = "cpp")
    plt.plot(iterations, mpi_times, label = "mpi")
    plt.xlabel("number of operations")
    plt.ylabel("time")
    plt.legend();
    plt.savefig('./test2.png')

if  __name__  == "__main__":
    main()
