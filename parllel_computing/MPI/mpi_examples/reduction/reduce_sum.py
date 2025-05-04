from mpi4py import MPI
import sys
def main(argv):
	comm=MPI.COMM_WORLD
	rank=comm.Get_rank()
	size=comm.Get_size()
	print("Hello world!")
	print(f"rank={rank}; size={size}")

	n=rank+1
	comm.Barrier()
	starttime=MPI.Wtime()
	sum=comm.reduce(n,op=MPI.SUM,root=0)
	
	if rank==0:
		print(f" Received total sum at rank {rank} ={sum}")
	endtime=MPI.Wtime()
	print(f"Timetaken for reduction is {endtime-starttime}")

if __name__=="__main__":
	main(sys.argv)
	MPI.Finalize()



