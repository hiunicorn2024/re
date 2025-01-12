cl = cl /std:c++20 /EHsc /utf-8 /I.. \
  /W4 /wd4100 /wd4127 /wd4189 /wd4244 /wd4267 /wd4459 /wd4702 /wd4996 /wd4324 \
  /WX

vc: xvvsim.obj main.cpp
	$(cl) -Femain.exe main.cpp xvvsim.obj
	main.exe
xvvsim.obj: xvvsim.cpp
	$(cl) /O2 /c xvvsim.cpp

gcc:
	g++ -O3 -std=c++20 -fexceptions -I.. \
  -Wall -Wno-unused-but-set-variable -Wno-unused-local-typedefs \
  -Wno-unused-variable \
  -o main.exe main.cpp xvvsim.cpp
	main.exe

clean:
	del main.obj
	del xvvsim.obj
	del main.exe
