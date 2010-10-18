#ifndef __DATA_H__
#define __DATA_H__

#include "utils.h"

struct _Data {
	Letter * data;
	Letter * extra_data; /* Left side data in the reverse order */
	/* Store arrays length too */
	int data_length;
	int extra_data_length;
};

Data * newData();
void freeData(Data *);

#endif /* __DATA_H__ */
