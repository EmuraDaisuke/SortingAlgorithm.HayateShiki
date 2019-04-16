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
|10,000|0.00045542|0.00038963|0.00063259|**0.00038294**|
|1,000,000|0.06900306|0.06157810|0.08987316|**0.05963387**|
|100,000,000|8.95517375|8.46306964|11.66389608|**7.92667359**|

Msvc is slower on std::sort and faster on std::stable_sort compared to other compilers.  
Although the unexpected result came out from the beginning, I was able to win because of this characteristic.  

## **clang++**
|Array|std::sort|std::stable_sort|gfx::timsort|Hayate-Shiki|
|-:|-:|-:|-:|-:|
|10,000|**0.00039482**|0.00044310|0.00058579|0.00040413|
|1,000,000|**0.05872628**|0.06865944|0.08372736|0.06585446|
|100,000,000|**7.77238754**|9.34188338|11.05923570|8.91061443|

A disappointing result clang++.  
Compared to other compilers, the optimization logic resulted in questionable results.  

## **g++**
|Array|std::sort|std::stable_sort|gfx::timsort|Hayate-Shiki|
|-:|-:|-:|-:|-:|
|10,000|0.00039890|0.00043748|0.00063354|**0.00037713**|
|1,000,000|**0.05793422**|0.06557857|0.08855738|0.05840274|
|100,000,000|**7.61435214**|8.86337450|11.39435302|7.83662079|

Good fight was g ++.  
Even [1,000,000] has become close.  
The difference in [100,000,000] seems to be the effect of caching efficiency due to in-place or out-place.  

<br>

# Limited condition benchmark
The following all sorted the array [100,000,000] of float value.  
The unit is seconds, the lower the number, the faster.  

## Constant
||std::sort|std::stable_sort|gfx::timsort|Hayate-Shiki|
|-:|-:|-:|-:|-:|
|Msvc|0.06713918|1.24926460|**0.03552456**|0.04001278|
|clang++|0.96321854|1.38927788|0.04675406|**0.03971196**|
|g++|1.24455174|1.27419866|0.04730418|**0.03958826**|

## Ascending order
||std::sort|std::stable_sort|gfx::timsort|Hayate-Shiki|
|-:|-:|-:|-:|-:|
|Msvc|1.04917134|1.24956610|**0.03797098**|0.04027310|
|clang++|1.06030176|1.38933688|0.04619012|**0.03979394**|
|g++|1.42411580|1.26989158|0.04694066|**0.03960894**|

## Descending order
||std::sort|std::stable_sort|gfx::timsort|Hayate-Shiki|
|-:|-:|-:|-:|-:|
|Msvc|1.24351168|1.62816732|**0.05992322**|0.06486218|
|clang++|0.76173164|1.49226176|**0.07894234**|0.08284760|
|g++|1.04372210|1.50443338|**0.07921356**|0.08537936|

<br>

# Finally
How was it?  

Beat std::stable_sort!  
gfx::timsort won by random numbers, but under certain conditions it was close.  
And it turns out that it is not easy to win std::sort.  
However, depending on the environment and input, it may have the potential to surpass std::sort.  

Does it come the day when merge sort wins quick sort?  

The sort algorithm is still romantic.  

<br>

---
It may be more enjoyable if you read the following together.  
* [Setsuna-Shiki](https://github.com/EmuraDaisuke/SortingAlgorithm.SetsunaShiki)
* [Homura-Shiki](https://github.com/EmuraDaisuke/SortingAlgorithm.HomuraShiki)
