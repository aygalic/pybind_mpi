#!/usr/bin/env python3

from mpi_lib import Pet, Dog



# Main program that delegates some work to a C++/MPI routine: say_hi
def  main ():
    print("ok")
    p = Pet("Molly")
    print(p)
    print(p.getName())
    p.setName("Charly")
    print(p.getName())

    p2 = Dog("Molly")
    print(p2.bark())
    print(p2.getName())

if  __name__  == "__main__":
    main()
