vc:
	cl /O2 /std:c++20 /EHsc /utf-8 /bigobj \
  /W4 /wd4100 /wd4127 /wd4189 /wd4244 /wd4267 /wd4459 /wd4702 /wd4996 /wd4324 \
  -Femain.exe main.cpp
	main.exe

gcc:
	g++ -O3 -std=c++20 -fexceptions \
  -Wall -Wno-unused-but-set-variable -Wno-unused-local-typedefs \
  -Wno-unused-variable \
  -o main.exe main.cpp
	main.exe

clean:
	del main.obj
	del main.exe
