from mpi4py import MPI
import sys

def main(argv):
	comm=MPI.COMM_WORLD
	rank=comm.Get_rank()
	size=comm.Get_size()
	print("Hello world")
	print(f"rank={rank}; size={size}")
	comm.Barrier()
	
	n=rank+1
	sum=comm.reduce(n,op=MPI.SUM,root=0)
	
	if rank==0:
		print(f"  Received={sum}")
	sum_allreduce=comm.allreduce(n,op=MPI.SUM)
	print(f" Received all reduce={sum_allreduce}")

if __name__=="__main__":
	main(sys.argv)
	MPI.Finalize()
