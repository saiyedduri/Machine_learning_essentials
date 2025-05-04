#broadcast_list.py
from mpi4py import MPI
import numpy as np

list=[0,1,2,3]
count=4
comm=MPI.COMM_WORLD
rank=comm.Get_rank()

if rank!=0:
	list=None

print("data before broadcasting to other threads")
print(f"received_data={list},rank={rank}")
received_data=comm.bcast(list,root=0)
print(f"received_data={received_data}; rank={rank}")

