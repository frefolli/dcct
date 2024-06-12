function convert_image() {
  image=$1
  convert $image.bmp $image.png
}

function compress_image() {
  format=$1
  t=$2
  F=$3
  d=$4
  ./builddir/main.exe -C -v -a "fftw:$F:$d" -i report/images/compression-$t-original.$format -o report/images/compression-$t-F$F-d$d.$format
  if [ "$format" = "bmp" ]; then
    convert_image report/images/compression-$t-F$F-d$d
  fi
}

function compress_and_convert_images() {
  format=$1
  t=$2
  if [ "$format" = "bmp" ]; then
    convert_image report/images/compression-$t-original
  fi
  compress_image $format $t 10 16
  compress_image $format $t 10 4
  compress_image $format $t 30 50
  compress_image $format $t 30 10
}

function main() {
  compress_and_convert_images bmp gs
  compress_and_convert_images png cl
}
main
