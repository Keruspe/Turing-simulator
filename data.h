/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*- */
/*
 * Turing-simulator
 * Copyright (C) Marc-Antoine Perennou 2010 <Marc-Antoine@Perennou.com>
 */

#ifndef __DATA_H__
#define __DATA_H__

#include "types.h"
#include "utils.h"

/* Structure to store a set of Data for the Machine */
struct _Data {
	/* Array which contains all the letters we read from the Data file */
	LettersCollection data;
	/* Array which contains left-side extra data (ex: extra_data[3] stands for data[-4]) */
	LettersCollection extra_data;

	/*
	 * Store arrays length too
	 */

	/* Number of Letter in the data we read */
	int data_length;
	/* Number of extra left-side data */
	int extra_data_length;
};

/* Instantiate a new Data set */
Data * newData();

/* Free memory, leaks are evil */
void freeData(Data *);

/* Get Letter at Machine position */
Letter getLetter(Machine *);

/* Set Letter at index */
void setLetter(Data *, int index, Letter);

/* Print the Data */
void printData(Data*);

#endif /* __DATA_H__ */
