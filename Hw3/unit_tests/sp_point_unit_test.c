#include "../SPPoint.h"
#include "unit_test_util.h"
//#include "sp_point_unit_test.h"
#include <stdbool.h>
#include <stdlib.h>

//TODO: need to check more things and stuff...
//TODO use valgrind

//Checks if copy Works
bool pointBasicCopyTest() {
	double data[2] = { 1.0, 1.0 };
	int dim = 2;
	int index = 1;
	SPPoint p = spPointCreate(data, dim, index);
	SPPoint q = spPointCopy(p);
	ASSERT_TRUE(spPointGetIndex(p) == spPointGetIndex(q));
	ASSERT_TRUE(spPointGetDimension(p) == spPointGetDimension(q));
	int i;
	for (i = 0; i < spPointGetDimension(p); i++) {
		ASSERT_TRUE(spPointGetAxisCoor(p, i) == spPointGetAxisCoor(q, i));
	}
	spPointDestroy(p);
	double data2[3] = {2.0,2.0,2.0};
	p = spPointCreate(data2,3,2);
	ASSERT_TRUE(spPointGetIndex(q)==1);
	ASSERT_TRUE(spPointGetDimension(q)==2);
	ASSERT_TRUE(spPointGetAxisCoor(q,1)==1.0);
	ASSERT_FALSE(spPointGetIndex(p) == spPointGetIndex(q) ||
			spPointGetDimension(p) == spPointGetDimension(q));
	spPointDestroy(p);
	spPointDestroy(q);
	return true;
}
bool pointBasicL2Distance() {
	double data1[2] = { 1.0, 2.0 };
	double data2[2] = { 1.0, 0.0 };
	int dim1 = 2;
	int dim2 = 2;
	int index1 = 1;
	int index2 = 1;
	SPPoint p = spPointCreate((double *)data1, dim1, index1);
	SPPoint q = spPointCreate((double *)data2, dim2, index2);
	ASSERT_TRUE(spPointL2SquaredDistance(p,p) == 0.0);
	ASSERT_TRUE(spPointL2SquaredDistance(q,q) == 0.0);
	ASSERT_TRUE(spPointL2SquaredDistance(p,q) == 4.0);
	ASSERT_TRUE(spPointL2SquaredDistance(q,p) == 4.0);
	spPointDestroy(p);
	spPointDestroy(q);
	return true;
}

//int main() {
//	RUN_TEST(pointBasicCopyTest);
//	RUN_TEST(pointBasicL2Distance);
//	return 0;
//}
