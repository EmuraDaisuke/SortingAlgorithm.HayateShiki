# 颯式(Hayate-Shiki)
Hayate-Shiki is an improved merge sort algorithm with the goal of "faster than quick sort".  

It has the following features.  
* Comparison sort
* Stable sort
* External area: N
* Average time: O (N log N)
* Worst time: O (N log N)
* Ascending completed: O (N)
* Descending completed: O (2N)
* Recursion: None

<br>

# Basic algorithm
* The external area is regarded as a 2N continuous band.
* The following rules apply when placing values ​​in the external area.
  * If (maximum <= value), place it above the ascending order column and update the maximum.
  * If (value < minimum), place it below the descending column and update the minimum.
  * If (minimum <= value < maximum), place new values ​​(maximum and minimum) in ascending order column, and let the value group arranged so far be Part.
* Merge parts.

<br>

## Examples
~~~
The external area is regarded as a 2N continuous band.

4 5 1 2 7 6 3 8|Input column
. . . . . . . .|External area
->Asc     Dsc<-|Actually

               |4 5 1 2 7 6 3 8|Input column
. . . . . . . . . . . . . . . .|External area
          Dsc<-|->Asc          |2N continuous band
~~~
~~~
Put new values ​​(maximum and minimum) in ascending order column.
               |. 5 1 2 7 6 3 8
. . . . . . . . 4 . . . . . . .
~~~
~~~
The next value is (maximum <= value), place it above the ascending order column and update the maximum.
               |. . 1 2 7 6 3 8
. . . . . . . . 4 5 . . . . . .
~~~
~~~
The next value is (value < minimum), place it below the descending column and update the minimum.
               |. . . 2 7 6 3 8
. . . . . . . 1 4 5 . . . . . .
~~~
~~~
The next value is (minimum <= value < maximum), place new values ​​(maximum and minimum) in ascending order column,
and let the value group arranged so far be Part.(Part: 145 completed)
               |. . . . 7 6 3 8
. . . . . . .|1 4 5|2 . . . . .
~~~
~~~
The next value is (maximum <= value), place it above the ascending order column and update the maximum.
               |. . . . . 6 3 8
. . . . . . .|1 4 5|2 7 . . . .
~~~
~~~
The next value is (minimum <= value < maximum), place new values ​​(maximum and minimum) in ascending order column,
and let the value group arranged so far be Part.(Part: 27 completed)
               |. . . . . . 3 8
. . . . . . .|1 4 5|2 7|6 . . .
~~~
~~~
The next value is (value < minimum), place it below the descending column and update the minimum.
               |. . . . . . . 8
. . . . . . 3|1 4 5|2 7|6 . . .
~~~
~~~
The next value is (maximum <= value), place it above the ascending order column and update the maximum.(Part: 368 completed)
               |. . . . . . . .
. . . . . .|3|1 4 5|2 7|6 8|. .
~~~
~~~
External area result.
4 5|2 7|6 8|. .  Ascending column arrangement
. . . . . .|3|1  Descending column arrangement
4 5|2 7|6 8|3|1  Actual arrangement
~~~
~~~
Merge generated Parts.
145  27  368
12457  368
12345678
Sort complete.
~~~

<br>

# Improvement
We will make additional improvements to the basic algorithm.  
* Insert sort is performed to secure the length of Part.
* Merge sequentially to avoid recursion.

<br>

# Build & Test
The following environment has been verified.  
* Windows 10 Pro 64bit
* Core i7-8700 3.20 GHz

<br>

## **Msvc**
Microsoft(R) C/C++ Optimizing Compiler Version 19.15.26732.1 for x64  
Microsoft (R) Incremental Linker Version 14.15.26732.1  
~~~
cl Main.cpp -Ox -EHsc -Fe:TestMsvc.exe
TestMsvc.exe
~~~

## **clang++**
clang version 7.0.0 (tags/RELEASE_700/final)  
Target: x86_64-w64-windows-gnu  
~~~
clang++ Main.cpp -O3 -o TestClang++.exe  
TestClang++.exe  
~~~

## **g++**
gcc version 8.2.0 (Rev3, Built by MSYS2 project)  
Target: x86_64-w64-mingw32  
~~~
g++ Main.cpp -O3 -o TestG++.exe
TestG++.exe
~~~

<br>

# Random number benchmark
Sorts float values ​​generated from the same seed.  
The unit is seconds, the lower the number, the faster.  

## **Msvc**
|件数|std::sort|std::stable_sort|Hayate-Shiki|
|-:|-:|-:|-:|
|10,000|0.00083858|*0.00069774*|**0.00060311**|
|1,000,000|0.07069080|*0.06401268*|**0.06192394**|
|100,000,000|8.96780856|*8.58470773*|**8.15964417**|

Msvc is slower on std :: sort and faster on std :: stable_sort compared to other compilers.  
Although the unexpected result came out from the beginning, I was able to win because of this characteristic.  

## **clang++**
|件数|std::sort|std::stable_sort|Hayate-Shiki|
|-:|-:|-:|-:|
|10,000|*0.00040838*|0.00044848|**0.00040581**|
|1,000,000|**0.05941431**|0.06861489|*0.06335664*|
|100,000,000|**7.61420540**|9.02416852|*8.35873630*|

A disappointing result clang++.  
The need for optimization at the source level has led to doubts about the compiler's optimization logic.  

## **g++**
|件数|std::sort|std::stable_sort|Hayate-Shiki|
|-:|-:|-:|-:|
|10,000|0.00041512|0.00045907|**0.00038881**|
|1,000,000|**0.05990257**|0.06931713|*0.06063945*|
|100,000,000|**7.63235313**|9.07213184|*7.94998035*|

Good fight was g ++.  
Even [1,000,000] has become close.  
The difference in [100,000,000] seems to be the effect of caching efficiency due to in-place or out-place.  

<br>

# Limited condition benchmark
The following all sorted the array [100,000,000] of float value.  
The unit is seconds, the lower the number, the faster.  

## Ascending completed
||std::sort|std::stable_sort|Hayate-Shiki|
|-:|-:|-:|-:|
|Msvc|**0.21892567**|1.25088036|*0.24510241*|
|clang++|*1.00117798*|1.44316028|**0.25334411**|
|g++|1.33518179|*1.30792267*|**0.22993043**|

## Descending completed
||std::sort|std::stable_sort|Hayate-Shiki|
|-:|-:|-:|-:|
|Msvc|*0.27433638*|1.50812926|**0.26809872**|
|clang++|*0.92900829*|1.64602567|**0.28246292**|
|g++|*1.21591655*|1.55615578|**0.25077546**|

## Constant
||std::sort|std::stable_sort|Hayate-Shiki|
|-:|-:|-:|-:|
|Msvc|**0.06657813**|1.25241026|*0.24396806*|
|clang++|*0.97352844*|1.42485664|**0.24551785**|
|g++|*1.22685844*|1.29003928|**0.22957563**|

<br>

# Finally
How was it?  

Hayate-Shiki won all over std::stable_sort, but it turns out that it is not easy to win over std::sort.  
However, depending on the environment and input, it may have the potential to surpass std::sort.  

Does it come the day when merge sort wins quick sort?  

The sort algorithm is still romantic.  
