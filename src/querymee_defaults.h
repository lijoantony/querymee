/***************************************************************************

    Copyright 2010, 2011 Reto Zingg <g.d0b3rm4n@gmail.com>

 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#ifndef QUERYMEE_DEFAULTS_H
#define QUERYMEE_DEFAULTS_H

#define MaxGrade0 5
#define NumberOfLeitnerBoxes 7

// max entries which can be in a leitner box until the box needs to be repeated
#define MAXENTRIES int maxEntries[7] = {0,15,30,60,120,240,480};

// Constants
#ifdef Q_WS_MAEMO_5
    #define DICTIONARY_PATH "/MyDocs/Querymee/"
#else
    #define DICTIONARY_PATH "/.querymee/"
#endif

#endif // QUERYMEE_DEFAULTS_H
