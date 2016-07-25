/*
 * sp_bpqueue_unit_test.c
 *
 *  Created on: 27 במאי 2016
 *      Author: Yair
 */



#include "unit_test_util.h"
#include "../SPListElement.h"
#include "../SPList.h"
#include "../SPBPriorityQueue.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include <assert.h>

static SPBPQueue quickQueue(int size, ...) {
	va_list items;
	SPBPQueue Q = spBPQueueCreate(size);
	va_start(items, size);
	int i;
	for (i = 0; i < size; i++) {
		spBPQueueEnqueue(Q, va_arg(items, SPListElement));
	}
	va_end(items);
	return Q;
}

bool testBPQueueCreate(){
	SPBPQueue q = spBPQueueCreate(3);
	ASSERT_TRUE(q!=NULL);
	spBPQueueDestroy(q);
	return true;
}

bool testBPQueueCopy(){
	ASSERT_TRUE(spListCopy(NULL) == NULL);
	SPBPQueue Q0 = spBPQueueCreate(3);
	SPBPQueue copyQ0 = spBPQueueCopy(Q0);
	ASSERT_TRUE(copyQ0!=NULL);
	ASSERT_TRUE(spBPQueueSize(copyQ0)==0);
	ASSERT_TRUE(spBPQueueGetMaxSize(copyQ0)==3);
	SPListElement e1 = spListElementCreate(1, 1.0);
	SPListElement e2 = spListElementCreate(2, 2.0);
	SPListElement e3 = spListElementCreate(3, 3.0);
	SPListElement e4 = spListElementCreate(4, 4.0);
	SPBPQueue Q = quickQueue(4,e1,e2,e3,e4);
	SPBPQueue copyQ = spBPQueueCopy(Q);
	int i;
	int n= spBPQueueSize(Q);
	// checks if the copy is the same as the original
	for(i=0;i<n;i++){
		SPListElement firstQ = spBPQueuePeek(Q);
		SPListElement firstCopyQ = spBPQueuePeek(copyQ);
		ASSERT_TRUE(spListElementCompare(firstQ,firstCopyQ)==0);
		spBPQueueDequeue(Q);
		spBPQueueDequeue(copyQ);
		spListElementDestroy(firstQ);
		spListElementDestroy(firstCopyQ);
	}
	//checks if when we change the original we don't change the copy
	spBPQueueDestroy(Q);
	spBPQueueDestroy(copyQ);
	Q = quickQueue(2,e1,e2);
	copyQ = spBPQueueCopy(Q);
	spBPQueueDestroy(Q);
	Q = quickQueue(2,e3,e4);
	ASSERT_FALSE(spBPQueueMinValue(Q)==spBPQueueMinValue(copyQ));
	ASSERT_FALSE(spBPQueueMaxValue(Q)==spBPQueueMaxValue(copyQ));
	spBPQueueDestroy(Q0);
	spBPQueueDestroy(copyQ0);
	spListElementDestroy(e1);
	spListElementDestroy(e2);
	spListElementDestroy(e3);
	spListElementDestroy(e4);
	spBPQueueDestroy(Q);
	spBPQueueDestroy(copyQ);
	return true;

}

bool testBPQueueDestroy(){
	spBPQueueDestroy(NULL);
	return true;
}

bool testBPQueueClear(){
	spBPQueueClear(NULL);
	SPListElement e1 = spListElementCreate(1, 1.0);
	SPListElement e2 = spListElementCreate(2, 2.0);
	SPListElement e3 = spListElementCreate(3, 3.0);
	SPBPQueue Q = quickQueue(3,e1,e2,e3);
	ASSERT_TRUE(spBPQueueSize(Q)==3);
	spBPQueueClear(Q);
	ASSERT_TRUE(spBPQueueSize(Q)==0);
	spListElementDestroy(e1);
	spListElementDestroy(e2);
	spListElementDestroy(e3);
	spBPQueueDestroy(Q);
	return true;
}

bool testBPQueueSize(){
	ASSERT_TRUE(spBPQueueSize(NULL)==-1);
	SPListElement e1 = spListElementCreate(1, 1.0);
	SPListElement e2 = spListElementCreate(2, 2.0);
	SPListElement e3 = spListElementCreate(3, 3.0);
	SPListElement e4 = spListElementCreate(4, 4.0);
	SPBPQueue Q = quickQueue(4,e1,e2,e3,e4);
	ASSERT_TRUE(spBPQueueSize(Q)==4);
	ASSERT_FALSE(spBPQueueEnqueue(Q,e1)!=SP_BPQUEUE_SUCCESS);
	ASSERT_TRUE(spBPQueueSize(Q)==4);
	ASSERT_FALSE(spBPQueueDequeue(Q)!=SP_BPQUEUE_SUCCESS);
	ASSERT_TRUE(spBPQueueSize(Q)==3);
	spListElementDestroy(e1);
	spListElementDestroy(e2);
	spListElementDestroy(e3);
	spListElementDestroy(e4);
	spBPQueueDestroy(Q);
	return true;
}

bool testBPQueueGetMaxSize(){
	ASSERT_TRUE(spBPQueueGetMaxSize(NULL)==-1);
	SPListElement e1 = spListElementCreate(1, 1.0);
	SPListElement e2 = spListElementCreate(2, 2.0);
	SPBPQueue Q = quickQueue(2,e1,e2);
	ASSERT_TRUE(spBPQueueGetMaxSize(Q)==2);
	ASSERT_FALSE(spBPQueueDequeue(Q)!=SP_BPQUEUE_SUCCESS);
	ASSERT_TRUE(spBPQueueGetMaxSize(Q)==2);
	spListElementDestroy(e1);
	spListElementDestroy(e2);
	spBPQueueDestroy(Q);
	return true;
}

bool testBPQueueEnqueue(){
	SPListElement e1 = spListElementCreate(1, 5.0);
	ASSERT_FALSE(spBPQueueEnqueue(NULL,e1)!=SP_BPQUEUE_INVALID_ARGUMENT);
	SPBPQueue Q = spBPQueueCreate(2);
	ASSERT_FALSE(spBPQueueEnqueue(Q,NULL)!=SP_BPQUEUE_INVALID_ARGUMENT);
	SPListElement e2 = spListElementCreate(2, 2.0);
	SPListElement e3 = spListElementCreate(3, 2.0);
	SPListElement e4 = spListElementCreate(4, 4.0);
	ASSERT_FALSE(spBPQueueEnqueue(Q,e4)!=SP_BPQUEUE_SUCCESS);
	ASSERT_FALSE(spBPQueueEnqueue(Q,e3)!=SP_BPQUEUE_SUCCESS);
	ASSERT_TRUE(spBPQueueMinValue(Q)==2.0);
	ASSERT_TRUE(spBPQueueMaxValue(Q)==4.0);
	ASSERT_FALSE(spBPQueueEnqueue(Q,e2)!=SP_BPQUEUE_SUCCESS);
	ASSERT_FALSE(spBPQueueEnqueue(Q,e1)!=SP_BPQUEUE_FULL);
	SPListElement first = spBPQueuePeek(Q);
	ASSERT_TRUE(spListElementGetIndex(first)==2);
	SPListElement last = spBPQueuePeekLast(Q);
	ASSERT_TRUE(spListElementGetIndex(last)==3);
	ASSERT_TRUE(spBPQueueMinValue(Q)==spBPQueueMaxValue(Q));
	spListElementDestroy(e1);
	spListElementDestroy(e2);
	spListElementDestroy(e3);
	spListElementDestroy(e4);
	spListElementDestroy(first);
	spListElementDestroy(last);
	spBPQueueDestroy(Q);
	return true;
}

bool testBPQueueDequeue(){
	ASSERT_FALSE(spBPQueueDequeue(NULL)!=SP_BPQUEUE_INVALID_ARGUMENT);
	SPBPQueue Q = spBPQueueCreate(2);
	ASSERT_FALSE(spBPQueueDequeue(Q)!=SP_BPQUEUE_EMPTY);
	SPListElement e1 = spListElementCreate(1, 1.0);
	ASSERT_FALSE(spBPQueueEnqueue(Q,e1)!= SP_BPQUEUE_SUCCESS);
	ASSERT_TRUE(spBPQueueSize(Q)==1);
	ASSERT_FALSE(spBPQueueDequeue(Q)!=SP_BPQUEUE_SUCCESS);
	ASSERT_TRUE(spBPQueueSize(Q)==0);
	spListElementDestroy(e1);
	spBPQueueDestroy(Q);
	return true;
}

bool testBPQueuePeek(){
	ASSERT_FALSE(spBPQueuePeek(NULL)!=NULL);
	SPBPQueue Q = spBPQueueCreate(2);
	ASSERT_FALSE(spBPQueuePeek(Q)!=NULL);
	SPListElement e2 = spListElementCreate(2, 2.0);
	SPListElement e1 = spListElementCreate(1, 1.0);
	spBPQueueEnqueue(Q,e2);
	spBPQueueEnqueue(Q,e1);
	SPListElement peeky = spBPQueuePeek(Q);
	ASSERT_TRUE(spListElementGetValue(peeky)==spBPQueueMinValue(Q));
	spListElementDestroy(e2);
	spListElementDestroy(e1);
	spListElementDestroy(peeky);
	spBPQueueDestroy(Q);
	return true;
}

bool testBPQueuePeekLast(){
	ASSERT_FALSE(spBPQueuePeekLast(NULL)!=NULL);
	SPBPQueue Q = spBPQueueCreate(2);
	ASSERT_FALSE(spBPQueuePeekLast(Q)!=NULL);
	SPListElement e1 = spListElementCreate(1, 1.0);
	SPListElement e2 = spListElementCreate(2, 2.0);
	spBPQueueEnqueue(Q,e1);
	spBPQueueEnqueue(Q,e2);
	SPListElement lastPeeky = spBPQueuePeekLast(Q);
	ASSERT_TRUE(spListElementGetValue(lastPeeky)==spBPQueueMaxValue(Q));
	spListElementDestroy(e2);
	spListElementDestroy(e1);
	spListElementDestroy(lastPeeky);
	spBPQueueDestroy(Q);
	return true;
}

bool testBPQueueMinValue(){
	ASSERT_TRUE(spBPQueueMinValue(NULL)==-1.0);
	SPBPQueue Q = spBPQueueCreate(2);
	ASSERT_TRUE(spBPQueueMinValue(Q)==-1.0);
	SPListElement e1 = spListElementCreate(1, 1.0);
	SPListElement e2 = spListElementCreate(2, 2.0);
	spBPQueueEnqueue(Q,e1);
	spBPQueueEnqueue(Q,e2);
	ASSERT_TRUE(spBPQueueMinValue(Q)==1.0);
	spBPQueueDequeue(Q);
	ASSERT_TRUE(spBPQueueMinValue(Q)==2.0);
	spListElementDestroy(e1);
	spListElementDestroy(e2);
	spBPQueueDestroy(Q);
	return true;
}

bool testBPQueueMaxValue(){
	ASSERT_TRUE(spBPQueueMaxValue(NULL)==-1.0);
	SPBPQueue Q = spBPQueueCreate(2);
	ASSERT_TRUE(spBPQueueMaxValue(Q)==-1.0);
	SPListElement e1 = spListElementCreate(1, 1.0);
	SPListElement e2 = spListElementCreate(2, 2.0);
	spBPQueueEnqueue(Q,e1);
	ASSERT_TRUE(spBPQueueMaxValue(Q)==1.0);
	spBPQueueEnqueue(Q,e2);
	ASSERT_TRUE(spBPQueueMaxValue(Q)==2.0);
	ASSERT_FALSE(spBPQueueDequeue(Q)!=SP_BPQUEUE_SUCCESS);
	ASSERT_TRUE(spBPQueueMaxValue(Q)==2.0);
	ASSERT_FALSE(spBPQueueDequeue(Q)!=SP_BPQUEUE_SUCCESS);
	ASSERT_TRUE(spBPQueueMaxValue(Q)==-1.0);
	spListElementDestroy(e1);
	spListElementDestroy(e2);
	spBPQueueDestroy(Q);
	return true;
}

bool testBPQueueIsEmpty(){
	ASSERT_FALSE(spBPQueueIsEmpty(NULL));
	SPBPQueue Q = spBPQueueCreate(2);
	ASSERT_TRUE(spBPQueueIsEmpty(Q));
	SPListElement e1 = spListElementCreate(1, 1.0);
	ASSERT_FALSE(spBPQueueEnqueue(Q,e1)!=SP_BPQUEUE_SUCCESS);
	ASSERT_FALSE(spBPQueueIsEmpty(Q));
	spListElementDestroy(e1);
	spBPQueueDestroy(Q);
	return true;
}

bool testBPQueueIsFull(){
	ASSERT_FALSE(spBPQueueIsFull(NULL));
	SPBPQueue Q = spBPQueueCreate(2);
	ASSERT_FALSE(spBPQueueIsFull(Q));
	SPListElement e1 = spListElementCreate(1, 1.0);
	SPListElement e2 = spListElementCreate(2, 2.0);
	spBPQueueEnqueue(Q,e1);
	spBPQueueEnqueue(Q,e2);
	ASSERT_TRUE(spBPQueueIsFull(Q));
	ASSERT_FALSE(spBPQueueDequeue(Q)!=SP_BPQUEUE_SUCCESS);
	ASSERT_FALSE(spBPQueueIsFull(Q));
	spListElementDestroy(e1);
	spListElementDestroy(e2);
	spBPQueueDestroy(Q);
	return true;
}

//int main(){
//	RUN_TEST(testBPQueueCreate);
//	RUN_TEST(testBPQueueCopy);
//	RUN_TEST(testBPQueueDestroy);
//	RUN_TEST(testBPQueueClear);
//	RUN_TEST(testBPQueueSize);
//	RUN_TEST(testBPQueueGetMaxSize);
//	RUN_TEST(testBPQueueEnqueue);
//	RUN_TEST(testBPQueueDequeue);
//	RUN_TEST(testBPQueuePeek);
//	RUN_TEST(testBPQueuePeekLast);
//	RUN_TEST(testBPQueueMinValue);
//	RUN_TEST(testBPQueueMaxValue);
//	RUN_TEST(testBPQueueIsEmpty);
//	RUN_TEST(testBPQueueIsFull);
//	return 0;
//}
