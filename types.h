/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*- */
/**
 * Turing-simulator
 * Copyright (C) Marc-Antoine Perennou 2010 <Marc-Antoine@Perennou.com>
 */

#ifndef __TYPES_H__
#define __TYPES_H__

/**
 * Define a few custom types for readability purpose
 */

/* Base types, Elements, names are obvious */
typedef char Move;
typedef char* State;
typedef char* Letter;
typedef char* Element; /* Can either be a State or a Letter */

/* More handy name for structs */
typedef struct _Transition Transition;
typedef struct _Machine Machine;
typedef struct _Data Data;

/* Collections */
typedef State* StatesCollection;
typedef Letter* LettersCollection;
typedef Element* ElementsCollection;
typedef Transition* TransitionsCollection;

/* More readable */
typedef char* String;

#endif /* __TYPES_H__ */
