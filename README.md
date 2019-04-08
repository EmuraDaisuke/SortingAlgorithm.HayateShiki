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
|10,000|0.00045311|0.00038742|0.00062179|**0.00037810**|
|1,000,000|0.06826439|0.06116140|0.08895712|**0.05941663**|
|100,000,000|8.99102733|8.54398624|11.82235680|**8.03245842**|

Msvc is slower on std::sort and faster on std::stable_sort compared to other compilers.  
Although the unexpected result came out from the beginning, I was able to win because of this characteristic.  

## **clang++**
|Array|std::sort|std::stable_sort|gfx::timsort|Hayate-Shiki|
|-:|-:|-:|-:|-:|
|10,000|0.00039968|0.00044068|0.00058889|**0.00039942**|
|1,000,000|**0.05761442**|0.06629626|0.08213553|0.06319817|
|100,000,000|**7.65998188**|9.02074948|10.78835585|8.59167794|

A disappointing result clang++.  
Compared to other compilers, the optimization logic resulted in questionable results.  

## **g++**
|Array|std::sort|std::stable_sort|gfx::timsort|Hayate-Shiki|
|-:|-:|-:|-:|-:|
|10,000|0.00039925|0.00043802|0.00063139|**0.00037335**|
|1,000,000|**0.05770632**|0.06557444|0.08941936|0.05796241|
|100,000,000|**7.65302426**|8.93940243|11.55346838|7.82881867|

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
|Msvc|1.05746516|1.26163644|0.04702674|**0.03562770**|
|clang++|1.03520472|1.46043022|**0.03555822**|0.05057494|
|g++|1.42474874|1.27932342|0.04135430|**0.03995162**|

## Descending completed
||std::sort|std::stable_sort|gfx::timsort|Hayate-Shiki|
|-:|-:|-:|-:|-:|
|Msvc|1.24606222|1.63625282|**0.05934068**|0.24938872|
|clang++|0.74098110|1.53675010|**0.07881390**|0.22430168|
|g++|1.04665430|1.51179186|**0.07844204**|0.23485550|

## Constant
||std::sort|std::stable_sort|gfx::timsort|Hayate-Shiki|
|-:|-:|-:|-:|-:|
|Msvc|0.06700882|1.26402688|0.04671364|**0.03510426**|
|clang++|0.96671638|1.46089826|**0.03567136**|0.05064882|
|g++|1.25442060|1.28246388|0.04092334|**0.03981956**|

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
