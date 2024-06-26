BUILDDIR=./builddir
LIB=./builddir/dcct.so
INCLUDE=./include/dcct.hh ./include/dcct/*.hh
SRC=./src/main.cc ./src/dcct/*.cc
MESON_CONF=meson.build
BUILD_TYPE=release

@all: ${LIB}

${BUILDDIR}: ${MESON_CONF}
	meson setup --buildtype=${BUILD_TYPE} ${BUILDDIR}
	cp -r $$(pwd)/resources ${BUILDDIR}/resources

${LIB}: ${BUILDDIR} ${SRC} ${INCLUDE}
	ninja -C ${BUILDDIR}

clean:
	rm -rf ${BUILDDIR}

test:
	meson test -C ${BUILDDIR}

install:
	mkdir -p ${DESTDIR}/usr/local/lib/
	mkdir -p ${DESTDIR}/usr/local/include/
	mkdir -p ${DESTDIR}/usr/local/share/pkgconfig
	mv builddir/libdcct.so ${DESTDIR}/usr/local/lib/
	cp -r include/* ${DESTDIR}/usr/local/include/
	cp -r dcct.pc ${DESTDIR}/usr/local/share/pkgconfig

fftw:
	./builddir/main.exe -I -a fftw
	./builddir/main.exe -R -i output.Y.bin

pocketfft:
	./builddir/main.exe -I -a pocketfft
	./builddir/main.exe -R -i output.Y.bin

docs:
	make -C ./doc

report/main.pdf: report/main.tex report/images/*
	make -C report
	cp report/main.pdf ~/Desktop/report-dcct.pdf

slides/main.pdf: slides/main.tex slides/images/*
	make -C slides
	cp slides/main.pdf ~/Desktop/slides-dcct.pdf

edit:
	lvim slides/main.tex
