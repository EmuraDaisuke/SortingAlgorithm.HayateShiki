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
cl Main.cpp -Ox -EHsc -Fe:TestMsvc.exe
TestMsvc.exe
~~~

## **clang++**
clang version 8.0.0 (tags/RELEASE_800/final)  
Target: x86_64-w64-windows-gnu  
~~~
clang++ Main.cpp -O3 -o TestClang++.exe
TestClang++.exe
~~~

## **g++**
gcc version 8.3.0 (Rev2, Built by MSYS2 project)  
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
|Array|std::sort|std::stable_sort|Hayate-Shiki|
|-:|-:|-:|-:|
|10,000|0.00045536|*0.00038941*|**0.00037852**|
|1,000,000|0.06819912|*0.06096062*|**0.05916414**|
|100,000,000|8.96064778|*8.46867757*|**7.98070745**|

Msvc is slower on std::sort and faster on std::stable_sort compared to other compilers.  
Although the unexpected result came out from the beginning, I was able to win because of this characteristic.  

## **clang++**
|Array|std::sort|std::stable_sort|Hayate-Shiki|
|-:|-:|-:|-:|
|10,000|**0.00039862**|0.00044817|*0.00040551*|
|1,000,000|**0.05683699**|0.06662549|*0.06324214*|
|100,000,000|**7.49816192**|9.04147304|*8.59871994*|

A disappointing result clang++.  
Compared to other compilers, the optimization logic resulted in questionable results.  

## **g++**
|Array|std::sort|std::stable_sort|Hayate-Shiki|
|-:|-:|-:|-:|
|10,000|*0.00039960*|0.00044316|**0.00037901**|
|1,000,000|**0.05772615**|0.06574962|*0.05814465*|
|100,000,000|**7.59918096**|8.88065410|*7.81997089*|

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
|Msvc|*0.26107338*|1.25014420|**0.03538870**|
|clang++|*0.99078740*|1.40538212|**0.05113556**|
|g++|1.28421660|*1.27630620*|**0.05116902**|

## Descending completed
||std::sort|std::stable_sort|Hayate-Shiki|
|-:|-:|-:|-:|
|Msvc|*0.31468252*|1.49977530|**0.25392970**|
|clang++|*0.92231022*|1.60681828|**0.28883200**|
|g++|*1.19012066*|1.50984774|**0.27091510**|

## Constant
||std::sort|std::stable_sort|Hayate-Shiki|
|-:|-:|-:|-:|
|Msvc|*0.06598216*|1.24120276|**0.03532978**|
|clang++|*0.96483422*|1.39459960|**0.05167836**|
|g++|*1.23251518*|1.26547230|**0.05145242**|

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
