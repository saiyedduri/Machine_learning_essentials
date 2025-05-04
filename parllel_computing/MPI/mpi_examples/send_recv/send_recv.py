from mpi4py import MPI

def main():
	comm=MPI.COMM_WORLD
	rank=comm.Get_rank()
	size=comm.Get_size()
	print("Hello World!")
	print(f"rank={rank}; size={size}")

	if rank==0:
		n="data"
		comm.send(n,dest=1,tag=0)
		print(f"Rank 0 still has n={n}", flush=True)

	elif rank==1:
		n=comm.recv(source=0,tag=0)
		print(f"n={n}")

if __name__=="__main__":
	main()
	MPI.Finalize()
