#ifndef __DATA_H__
#define __DATA_H__

#include "utils.h"

/* Structure to store a set of Data for the Machine */
struct _Data {
	/* Array which contains all the letters we read from the Data file */
	Letter * data;
	/* Array which contains left-side extra data (extra_data[X] stands for data[-X]) */
	Letter * extra_data;

	/* Store arrays length too */

	/* Number of Letter in the data we read */
	int data_length;
	/* Number of extra left-side data */
	int extra_data_length;
};

/* Instantiate a new Data set */
Data * newData();

/* Free memory, leaks are evil */
void freeData(Data *);

#endif /* __DATA_H__ */
