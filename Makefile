submit-consecutive:
	./dvm_mpi c solve.cdv -o solve_cons
	./dvm_mpi run solve_cons -W 00:30 -- 768 1

submit-parallel:
	./dvm_mpi c solve.cdv -o solve_mpi
	./dvm_mpi run 4 5 1 solve_mpi -W 00:30 -- 768 1
	./dvm_mpi run 8 5 1 solve_mpi -W 00:30 -- 768 1

submit-omp:
	./dvm_omp2 c solve.cdv -o solve_omp2
	./dvm_omp2 run solve_omp2 -W 00:30 -- 768 1
	./dvm_omp2 run 4 5 1 solve_omp2 -W 00:30 -- 768 1
	./dvm_omp2 run 8 5 1 solve_omp2 -W 00:30 -- 768 1
	./dvm_omp4 c solve.cdv -o solve_omp4
	./dvm_omp4 run solve_omp4 -W 00:30 -- 768 1
	./dvm_omp4 run 4 5 1 solve_omp4 -W 00:30 -- 768 1
	./dvm_omp4 run 8 5 1 solve_omp4 -W 00:30 -- 768 1
	./dvm_omp8 c solve.cdv -o solve_omp8
	./dvm_omp8 run solve_omp8 -W 00:30 -- 768 1
	./dvm_omp8 run 4 5 1 solve_omp8 -W 00:30 -- 768 1
	./dvm_omp8 run 8 5 1 solve_omp8 -W 00:30 -- 768 1

submit-gpu:
	./dvm_gpu1 c solve.cdv -o solve_gpu1
	./dvm_gpu1 run solve_gpu1 -W 00:30 -- 768 1
	./dvm_gpu1 run 4 5 1 solve_gpu1 -W 00:30 -- 768 1
	./dvm_gpu1 run 8 5 1 solve_gpu1 -W 00:30 -- 768 1
	./dvm_gpu2 c solve.cdv -o solve_gpu2
	./dvm_gpu2 run solve_gpu2 -W 00:30 -- 768 1
	./dvm_gpu2 run 4 5 1 solve_gpu2 -W 00:30 -- 768 1
	./dvm_gpu2 run 8 5 1 solve_gpu2 -W 00:30 -- 768 1
	./dvm_gpu4 c solve.cdv -o solve_gpu4
	./dvm_gpu4 run solve_gpu4 -W 00:30 -- 768 1
	./dvm_gpu4 run 4 5 1 solve_gpu4 -W 00:30 -- 768 1
	./dvm_gpu4 run 8 5 1 solve_gpu4 -W 00:30 -- 768 1

submit-gpu-omp:
	./dvm_gpuomp1 c solve.cdv -o solve_gpuomp1
	./dvm_gpuomp1 run solve_gpuomp1 -W 00:30 -- 768 1
	./dvm_gpuomp1 run 4 5 1 solve_gpuomp1 -W 00:30 -- 768 1
	./dvm_gpuomp1 run 8 5 1 solve_gpuomp1 -W 00:30 -- 768 1
	./dvm_gpuomp2 c solve.cdv -o solve_gpuomp2
	./dvm_gpuomp2 run solve_gpuomp2 -W 00:30 -- 768 1
	./dvm_gpuomp2 run 4 5 1 solve_gpuomp2 -W 00:30 -- 768 1
	./dvm_gpuomp2 run 8 5 1 solve_gpuomp2 -W 00:30 -- 768 1
	./dvm_gpuomp4 c solve.cdv -o solve_gpuomp4
	./dvm_gpuomp4 run solve_gpuomp4 -W 00:30 -- 768 1
	./dvm_gpuomp4 run 4 5 1 solve_gpuomp4 -W 00:30 -- 768 1
	./dvm_gpuomp4 run 8 5 1 solve_gpuomp4 -W 00:30 -- 768 1
	./dvm_gpuomp8 c solve.cdv -o solve_gpuomp8
	./dvm_gpuomp8 run solve_gpuomp8 -W 00:30 -- 768 1
	./dvm_gpuomp8 run 4 5 1 solve_gpuomp8 -W 00:30 -- 768 1
	./dvm_gpuomp8 run 8 5 1 solve_gpuomp8 -W 00:30 -- 768 1
