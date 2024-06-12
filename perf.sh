function execute() {
  method=$1
  #mem_load_uops_retired.l1_hit,mem_load_uops_retired.l2_hit,mem_load_uops_retired.l3_hit,mem_load_uops_retired.l1_miss,mem_load_uops_retired.l2_miss,mem_load_uops_retired.l3_miss
  perf stat -e cycles,instructions,branches,branch-misses,cache-references,cache-misses,L1-dcache-loads,L1-dcache-load-misses,LLC-loads,LLC-load-misses,dTLB-loads,dTLB-load-misses,iTLB-loads,iTLB-load-misses,context-switches,cpu-migrations,page-faults \
  ./builddir/main.exe -C -v -a "$method:30:29" -i resources/images/lena.bmp
}

execute fftw
execute pocketfft
