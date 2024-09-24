cc = g++ -O3 -std=c++20 -fexceptions -I../../.. \
  -Wall -Werror -Wno-unused-but-set-variable -Wno-unused-local-typedefs \
  -Wno-unused-variable

cl = cl /O2 /std:c++20 /EHsc /utf-8 /bigobj /I../../.. \
  /W4 /wd4100 /wd4127 /wd4189 /wd4244 /wd4267 /wd4459 /wd4702 /wd4996 /wd4324 \
  /WX

cl_libs = user32.lib gdi32.lib winmm.lib shlwapi.lib shell32.lib
cc_libs = -lwinmm -lShLwApi -lShell32

vc:
	$(cl) /Fea.exe main.cpp $(cl_libs)
	a
gcc:
	$(cc) -o a.exe main.cpp $(cc_libs)
	a

all0: a0.exe a1.exe
all:
	$(MAKE) -j2 all0

a0.exe: main.cpp
	copy main.cpp main_0.cpp
	$(cl) /Fea0.exe main_0.cpp $(cl_libs)
	a0
a1.exe: main.cpp
	copy main.cpp main_1.cpp
	$(cc) -o a1.exe main_1.cpp $(cc_libs)
	a1
a2.exe: main.cpp
	copy main.cpp main_2.cpp
	$(cl) /Fea2.exe main_2.cpp -DRE_NOEXCEPT $(cl_libs)
	a2
a3.exe: main.cpp
	copy main.cpp main_3.cpp
	$(cc) -o a3.exe main_3.cpp -DRE_NOEXCEPT $(cc_libs)
	a3

clean:
	del a.exe
	del a0.exe
	del a1.exe
	del a2.exe
	del a3.exe
	del main_0.cpp
	del main_1.cpp
	del main_2.cpp
	del main_3.cpp
	del main.obj
	del main_0.obj
	del main_2.obj
