 from mpi4py import MPI

comm=MPI.COMM_WORLD
rank=comm.Get_rank()
size=comm.Get_size()

# Defining the categeries for distinction 
color=0 if rank==0 else 1

key=rank
# Creating new communicator
subcomm=comm.Split(color=color, key=key)

if color==1:
	sub_rank=subcomm.Get_rank()
	sub_size=subcomm.Get_size()
	print(f"World rank {rank} is in subcomm with rank {sub_rank}")
else:
	print(f"World rank {rank}is not part of subcomm (color==0)")



