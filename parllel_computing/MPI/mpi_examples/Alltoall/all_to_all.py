from mpi4py import MPI

comm=MPI.COMM_WORLD
rank=comm.Get_rank()
size=comm.Get_size()

print("Hello world!")
print(f"Rank={rank} of size={size}")

send_data=[rank*10+i for i in range(size)]
print(f"Rank {rank}; sending:{send_data}",flush=True)

# recieving the empty list to hold the received values
recv_data=[0]*size

# Alltoall communication
recv_data=comm.alltoall(send_data)

print(f"Rank {rank} received: {recv_data}",flush=True)
