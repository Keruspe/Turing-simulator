#ifndef __DATA_H__
#define __DATA_H__

#include "types.h"

struct _Data {
	Letter * data;
	// Store arrays length too
	int data_length;
};

Data * newData();
void freeData(Data *);

#endif /* __DATA_H__ */
