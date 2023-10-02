# CMakeList.txt : CMake project for my_math, include source and define
# project specific logic here.

set(HEADERS 
	sourses/headers.h 
	sourses/matrix.h
	sourses/bigint.h 
	sourses/complex.h 
	sourses/polynomial.h 
	sourses/numerical_methods.h 
)

set(SOURSES
		sourses/main.cpp
		sourses/bigint.cpp
)

add_executable (${PROJECT_NAME} ${SOURSES} ${HEADERS})

