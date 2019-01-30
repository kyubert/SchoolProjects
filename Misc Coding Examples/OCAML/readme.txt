On my honor I have neither given nor recieved aid on this exam.

readme.txt (this document)
beng-sde2.log (recorded tests of required functions)
randomgenerator.caml (generates random lists of vectors, not essential to the code but proof for testing)
cmeans.caml (the source code)

Function 3.1: distanceR2 (v1, v2)
-finds the distance between two vectors (float lists)
-Helper: square n
--squares the float n

Function 3.2: distanceSqAllMeans (v, vset)
-finds the distances between vector v and all vectors in vset (float list list)

Function 3.3: listMinPos (alist)
-returns the position of the smallest value in alist
-Helper: listCmp (blist)
--returns the smallest value in blist

Function 3.4: elsum (l1, l2)
-adds vectors l1 and l2

Function 3.5: scaleList (l1, scale)
-divides elements of vector l1 by integer scale

Function 3.6: zeroes (size)
-creates a vector of size size for which the values are only 0.0

Function 3.7: zeroMeansSet (c,d)
-creates a list of size c of vectors of size d for which all values are 0.0

Function 3.8: zeroVdiff (v1, v2)
-returns true if vectors v1 and v2 are exactly the same and returns false if otherwise

Function 3.9: zeroSetdiff (s1, s2)
-applies zeroVdiff to a list of vectors and only returns true if all vectors are equal

Function 3.10: zeroCounts (c)
-creates an integer list of size c in which all values are set to 0

Function 3.11: updateCounts (p, counts)
-adds 1 to the pth element of integer list counts

Function 4.1: updateMeansSum (v, x, means)
-adds v to the xth vector in vector list means

Function 4.2: formNewMeans (newmeanssum, newcounts)
-divides the vectors in vector list newmeanssum by the respective integers in integer list newcounts

Function 4.3: reclassify (h, currmeans, newmeanssum, newcounts)
-returns the average of the closest vectors in vector list h to each vector in vector list currmeans

Function 4.4: cmeans (c, h, mucurrent)
-runs reclassify on vector list h and vector list of length c mucurrent until two consecutive runs return the same result