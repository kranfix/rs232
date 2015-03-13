CC=gcc
dir=rs232
PKG=.build/${dir}.o

OS=linux
TEST=.build/test
SAMPLE=.build/sample
EXT=c

build: ${PKG}

${PKG}: ${dir}_${OS}.${EXT} ${dir}.h .build
	${CC} -c $< -o $@

.build:
	mkdir .build

clean:
	rm .build/${dir}.o

test: ${TEST}
	./${TEST}

sample: ${SAMPLE}	
	./${SAMPLE}

${TEST}: test_${OS}.c ${PKG}
${SAMPLE}: sample_${OS}.c ${PKG}
	${CC} $^ -o $@

cleantest:
	rm ${TEST}

cleansample:
	rm ${SAMPLE}

cleanall:
	rm -rdf .build
