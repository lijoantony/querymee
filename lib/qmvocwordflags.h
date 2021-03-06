/***************************************************************************
*   this file is from kdeedu project. Filename: keduvocwordflags.h
***************************************************************************/

/***************************************************************************
    Copyright 2008 David Capel <wot.narg@gmail.com>
    Copyright (C) 2010 Reto Zingg <g.d0b3rm4n@gmail.com>
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#ifndef QMVOCWORDFLAGS_H
#define QMVOCWORDFLAGS_H

#include <QFlags>

class QmVocWordFlag
{
public:
    enum Flags
    {
        // This is used for both empty flags and to denote no flags of the correct type were set.
        NoInformation = 0x0,

        // Gender
        Masculine = 0x1,
        Feminine = 0x2,
        Neuter = 0x4,

        // Plurality
        Singular = 0x10,
        Dual = 0x20,
        Plural = 0x40,

        // Part of Speech
        Verb = 0x100,
        Noun = 0x200,
        Pronoun = 0x400,
        Adjective = 0x800,
        Adverb = 0x1000,
        Article = 0x2000,
        Conjunction = 0x4000,

        // Person
        First = 0x10000,
        Second = 0x20000,
        Third = 0x40000,

        // Declension Case
        Nominative = 0x80000,
        Genitive = 0x100000,
        Dative = 0x200000,
        Accusative = 0x400000,
        Ablative = 0x800000,
        Locative = 0x1000000,
        Vocative = 0x2000000,

        // Other assorted flags
        Definite = 0x4000000, // The article is definite
        Indefinite = 0x8000000, // The article is indefinite
        Regular = 0x10000000,
        Irregular = 0x20000000
    };

    static const Flags genders = (Flags)(Masculine | Feminine | Neuter);
    static const Flags partsOfSpeech =(Flags)(Noun | Verb | Article | Pronoun | Adjective | Adverb | Conjunction);
    static const Flags numbers = (Flags)(Singular | Plural | Dual);
    static const Flags cases = (Flags)(Nominative|Genitive|Dative|Accusative|Ablative|Locative|Vocative);
    static const Flags persons = (Flags)(First | Second | Third);

};

Q_DECLARE_FLAGS(QmVocWordFlags, QmVocWordFlag::Flags)
Q_DECLARE_OPERATORS_FOR_FLAGS(QmVocWordFlags)

#endif
