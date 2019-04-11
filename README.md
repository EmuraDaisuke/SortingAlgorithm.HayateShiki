# 颯式(Hayate-Shiki)
Hayate-Shiki is an improved merge sort algorithm with the goal of "faster than quick sort".  

It has the following features.  
* Comparison sort
* Stable sort
* External area: N
* Best time: O (N)
* Average time: O (N log N)
* Worst time: O (N log N)
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
|10,000|0.00045104|0.00038737|0.00062055|**0.00037726**|
|1,000,000|0.06895459|0.06194708|0.09007592|**0.05960105**|
|100,000,000|8.94273760|8.48790507|11.70993918|**7.90364017**|

Msvc is slower on std::sort and faster on std::stable_sort compared to other compilers.  
Although the unexpected result came out from the beginning, I was able to win because of this characteristic.  

## **clang++**
|Array|std::sort|std::stable_sort|gfx::timsort|Hayate-Shiki|
|-:|-:|-:|-:|-:|
|10,000|0.00039748|0.00044216|0.00058833|**0.00039542**|
|1,000,000|**0.05794733**|0.06703828|0.08304218|0.06467940|
|100,000,000|**7.59819467**|9.02512841|10.90226948|8.69520123|

A disappointing result clang++.  
Compared to other compilers, the optimization logic resulted in questionable results.  

## **g++**
|Array|std::sort|std::stable_sort|gfx::timsort|Hayate-Shiki|
|-:|-:|-:|-:|-:|
|10,000|0.00039915|0.00044045|0.00062139|**0.00037733**|
|1,000,000|**0.05791501**|0.06553794|0.08642513|0.05851454|
|100,000,000|**7.62242797**|8.88416647|11.16218393|7.86212201|

Good fight was g ++.  
Even [1,000,000] has become close.  
The difference in [100,000,000] seems to be the effect of caching efficiency due to in-place or out-place.  

<br>

# Limited condition benchmark
The following all sorted the array [100,000,000] of float value.  
The unit is seconds, the lower the number, the faster.  

## Ascending order
||std::sort|std::stable_sort|gfx::timsort|Hayate-Shiki|
|-:|-:|-:|-:|-:|
|Msvc|1.05042422|1.26930574|0.04714074|**0.04002944**|
|clang++|1.05335930|1.44680144|**0.03551652**|0.03996312|
|g++|1.43373992|1.27849542|0.04640786|**0.03997012**|

## Descending order
||std::sort|std::stable_sort|gfx::timsort|Hayate-Shiki|
|-:|-:|-:|-:|-:|
|Msvc|1.24230476|1.63490322|**0.05923096**|0.27479994|
|clang++|0.76989306|1.53627088|**0.07876710**|0.22927154|
|g++|1.04074260|1.53410250|**0.07918274**|0.23090848|

## Constant
||std::sort|std::stable_sort|gfx::timsort|Hayate-Shiki|
|-:|-:|-:|-:|-:|
|Msvc|0.06637970|1.26441570|0.04730072|**0.04029890**|
|clang++|1.14472912|1.44540720|**0.03527028**|0.03984316|
|g++|1.26266952|1.29177762|0.04832892|**0.04242476**|

<br>

# Finally
How was it?  

Beat std::stable_sort!  
Almost defeated gfx::timsort but lost in descending order.  
And it turns out that it is not easy to win std::sort.  
However, depending on the environment and input, it may have the potential to surpass std::sort.  

Does it come the day when merge sort wins quick sort?  

The sort algorithm is still romantic.  

<br>

---
It may be more enjoyable if you read the following together.  
* [Setsuna-Shiki](https://github.com/EmuraDaisuke/SortingAlgorithm.SetsunaShiki)
* [Homura-Shiki](https://github.com/EmuraDaisuke/SortingAlgorithm.HomuraShiki)
