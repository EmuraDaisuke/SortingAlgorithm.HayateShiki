# qtq.Merge
qtq.Mergeは、「クイックソートより速い（Quicker than QuickSort）」を目指している、マージソートの改良アルゴリズムです。

以下の特徴があります。  
* 比較ソート
* 安定ソート
* 外部ソート：N
* 平均時間：O(N log N)
* 最悪時間：O(N log N)
* 昇順ソート済み：O(N)
* 降順ソート済み：O(2N)
* 再帰無し

<br>

# ビルド＆テスト
## **Msvc**
cl Main.cpp -Ox -EHsc -Fe:TestMsvc.exe  
./TestMsvc.exe  

## **clang++**
clang++ Main.cpp -Ofast -o TestClang.exe  
./TestClang.exe  

## **g++**
g++ Main.cpp -Ofast -o TestGcc.exe  
./TestGcc.exe  

<br>

# 評価サンプル
同じ種から生成した、ランダムなfloat値  
単位は秒（値が低い程、速い）

## **Msvc**
Microsoft(R) C/C++ Optimizing Compiler Version 19.15.26732.1 for x64  
Microsoft (R) Incremental Linker Version 14.15.26732.1  

|数|std::sort|std::stable_sort|qtq.Merge.TypeV|
|-:|-:|-:|-:|
|10,000|0.00083858|*0.00069774*|**0.00060311**|
|1,000,000|0.07069080|*0.06401268*|**0.06192394**|
|100,000,000|8.96780856|*8.58470773*|**8.15964417**|

## **clang**
clang version 7.0.0 (tags/RELEASE_700/final)  
Target: x86_64-w64-windows-gnu  

|数|std::sort|std::stable_sort|qtq.Merge.TypeV|
|-:|-:|-:|-:|
|10,000|*0.00041480*|0.00045297|**0.00040581**|
|1,000,000|**0.06034432**|0.06799446|*0.06345384*|
|100,000,000|**7.61524030**|9.22656715|*8.38774283*|

## **gcc**
gcc version 8.2.0 (Rev3, Built by MSYS2 project)  
Target: x86_64-w64-mingw32  

|数|std::sort|std::stable_sort|qtq.Merge.TypeV|
|-:|-:|-:|-:|
|10,000|0.00041768|0.00045425|**0.00038624**|
|1,000,000|**0.06043607**|0.06795885|*0.06112900*|
|100,000,000|**7.82155688**|9.08399603|*7.91397587*|

## **動作環境**
Windows 10 Pro 64bit  
Core i7-8700 3.20GHz  
