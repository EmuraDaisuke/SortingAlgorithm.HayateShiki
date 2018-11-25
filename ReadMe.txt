


Build & Test
	Msvc
		cl main.cpp -Ox -EHsc -Fe:test_msvc.exe
		./test_msvc.exe
	
	clang++
		clang++ main.cpp -Ofast -o test_clang.exe
		./test_clang.exe
	
	g++
		g++ main.cpp -Ofast -o test_gnu.exe
		./test_gnu.exe
