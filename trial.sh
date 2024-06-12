for file in $(find ~/Documents/rexim/immagini-p2/ -type f); do
  echo -e "Trying $file"
  ./builddir/main.exe -C -v -a "pocketfft:30:29" -i $file -o /tmp/dct.output.png
done
