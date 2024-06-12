F=30
d=29
baseimg=resources/images/lena.bmp

function scale_image() {
  N=$1
  convert $baseimg -resize $Nx$N /tmp/dcct.benchmark.png
}

function benchmark_method() {
  N=$1
  method=$2
  ./builddir/main.exe -v -C -i /tmp/dcct.benchmark.png -a "$method:$F:$d" 2> out.log
  TIME=$(grep "Compressed" out.log | grep "[0-9.]\+" -o | tail -n 1) 
  echo $method,$N,$TIME >> benchmark.$method.log
  echo $method,$N,$TIME
}

function benchmark_round() {
  N=$1
  scale_image $N
  benchmark_method $N pocketfft
  benchmark_method $N fftw
}

function main() {
  echo "Method,N,Time" > benchmark.fftw.log
  echo "Method,N,Time" > benchmark.pocketfft.log
  for N in $(seq 100 100 1000);
  do
    benchmark_round $N
  done
}
main
