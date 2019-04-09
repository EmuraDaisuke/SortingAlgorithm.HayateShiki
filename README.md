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
Microsoft(R) C/C++ Optimizing Compiler Version 19.16.27027.1 for x64  
~~~
cl Main.cpp -std:c++14 -DNDEBUG -Ox -EHsc -Fe:TestMsvc.exe
TestMsvc.exe
~~~

## **clang++**
clang version 8.0.0 (tags/RELEASE_800/final)  
Target: x86_64-w64-windows-gnu  
~~~
clang++ Main.cpp -std=c++14 -DNDEBUG -O3 -o TestClang++.exe
TestClang++.exe
~~~

## **g++**
gcc version 8.3.0 (Rev2, Built by MSYS2 project)  
Target: x86_64-w64-mingw32  
~~~
g++ Main.cpp -std=c++14 -DNDEBUG -O3 -o TestG++.exe
TestG++.exe
~~~

<br>

# Random number benchmark
Sorts float values ​​generated from the same seed.  
The unit is seconds, the lower the number, the faster.  

## **Msvc**
|Array|std::sort|std::stable_sort|gfx::timsort|Hayate-Shiki|
|-:|-:|-:|-:|-:|
|10,000|0.00045246|0.00038873|0.00062176|**0.00037623**|
|1,000,000|0.06813277|0.06093867|0.08863368|**0.05777066**|
|100,000,000|8.94022903|8.47948857|11.70084008|**7.79592353**|

Msvc is slower on std::sort and faster on std::stable_sort compared to other compilers.  
Although the unexpected result came out from the beginning, I was able to win because of this characteristic.  

## **clang++**
|Array|std::sort|std::stable_sort|gfx::timsort|Hayate-Shiki|
|-:|-:|-:|-:|-:|
|10,000|0.00039759|0.00044011|0.00058801|**0.00039381**|
|1,000,000|**0.05758739**|0.06616213|0.08201547|0.06179281|
|100,000,000|**7.59832462**|8.98149368|10.78802763|8.35315418|

A disappointing result clang++.  
Compared to other compilers, the optimization logic resulted in questionable results.  

## **g++**
|Array|std::sort|std::stable_sort|gfx::timsort|Hayate-Shiki|
|-:|-:|-:|-:|-:|
|10,000|0.00039976|0.00043879|0.00063106|**0.00037259**|
|1,000,000|**0.05787407**|0.06543038|0.08824662|0.05804457|
|100,000,000|**7.63117388**|8.87959867|11.41298208|7.81652397|

Good fight was g ++.  
Even [1,000,000] has become close.  
The difference in [100,000,000] seems to be the effect of caching efficiency due to in-place or out-place.  

<br>

# Limited condition benchmark
The following all sorted the array [100,000,000] of float value.  
The unit is seconds, the lower the number, the faster.  

## Ascending completed
||std::sort|std::stable_sort|gfx::timsort|Hayate-Shiki|
|-:|-:|-:|-:|-:|
|Msvc|1.05179160|1.27148740|**0.04668568**|0.19135180|
|clang++|1.03415544|1.46316208|**0.03579544**|0.17728592|
|g++|1.43500044|1.28193182|**0.04063568**|0.18892132|

## Descending completed
||std::sort|std::stable_sort|gfx::timsort|Hayate-Shiki|
|-:|-:|-:|-:|-:|
|Msvc|1.24744338|1.63704790|**0.05956712**|0.28730910|
|clang++|0.74045104|1.51935572|**0.07884614**|0.25792324|
|g++|1.03767822|1.51203144|**0.07834288**|0.24093602|

## Constant
||std::sort|std::stable_sort|gfx::timsort|Hayate-Shiki|
|-:|-:|-:|-:|-:|
|Msvc|0.06624690|1.26874262|**0.04670814**|0.18949800|
|clang++|0.96820870|1.46377342|**0.03569006**|0.17826696|
|g++|1.25858640|1.27864388|**0.04058178**|0.18731668|

<br>

# Finally
How was it?  

Hayate-Shiki won all over std::stable_sort, but it turns out that it is not easy to win over std::sort.  
However, depending on the environment and input, it may have the potential to surpass std::sort.  

Does it come the day when merge sort wins quick sort?  

The sort algorithm is still romantic.  

<br>

---
It may be more enjoyable if you read the following together.  
* [Setsuna-Shiki](https://github.com/EmuraDaisuke/SortingAlgorithm.SetsunaShiki)
* [Homura-Shiki](https://github.com/EmuraDaisuke/SortingAlgorithm.HomuraShiki)
