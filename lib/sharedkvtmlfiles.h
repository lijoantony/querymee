/***************************************************************************
*   this file is from kdeedu project. Filename: sharedkvtmlfiles.h
***************************************************************************/

/***************************************************************************
                     scan a group of KVTML documents to get information from them
    -----------------------------------------------------------------------
    copyright      : (C) 2007 Jeremy Whiting <jpwhiting@kde.org>

    Copyright 2010 Reto Zingg <g.d0b3rm4n@gmail.com>

 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#ifndef SHAREDKVTMLFILES_H
#define SHAREDKVTMLFILES_H

#include "libqtvtvocdocument_export.h"

#include <QtCore/QStringList>

typedef void (*KdeCleanUpFunction)();

class KCleanUpGlobalStatic
{
        public:
            KdeCleanUpFunction func;

            inline ~KCleanUpGlobalStatic() { func(); }
};


#ifdef Q_CC_MSVC
# define K_GLOBAL_STATIC_STRUCT_NAME(NAME) _k_##NAME##__LINE__
#else
# define K_GLOBAL_STATIC_STRUCT_NAME(NAME)
#endif

#define K_GLOBAL_STATIC(TYPE, NAME) K_GLOBAL_STATIC_WITH_ARGS(TYPE, NAME, ())

#define K_GLOBAL_STATIC_WITH_ARGS(TYPE, NAME, ARGS)                            \
static QBasicAtomicPointer<TYPE > _k_static_##NAME = Q_BASIC_ATOMIC_INITIALIZER(0); \
static bool _k_static_##NAME##_destroyed;                                      \
static struct K_GLOBAL_STATIC_STRUCT_NAME(NAME)                                \
{                                                                              \
     bool isDestroyed()                                                         \
     {                                                                          \
         return _k_static_##NAME##_destroyed;                                   \
     }                                                                          \
     inline operator TYPE*()                                                    \
     {                                                                          \
         return operator->();                                                   \
     }                                                                          \
     inline TYPE *operator->()                                                  \
     {                                                                          \
         if (!_k_static_##NAME) {                                               \
             if (isDestroyed()) {                                               \
                 qFatal("Fatal Error: Accessed global static '%s *%s()' after destruction. " \
                        "Defined at %s:%d", #TYPE, #NAME, __FILE__, __LINE__);  \
             }                                                                  \
             TYPE *x = new TYPE ARGS;                                           \
             if (!_k_static_##NAME.testAndSetOrdered(0, x)                      \
                 && _k_static_##NAME != x ) {                                   \
                 delete x;                                                      \
             } else {                                                           \
                 static KCleanUpGlobalStatic cleanUpObject = { destroy };       \
             }                                                                  \
         }                                                                      \
         return _k_static_##NAME;                                               \
     }                                                                          \
     inline TYPE &operator*()                                                   \
     {                                                                          \
         return *operator->();                                                  \
     }                                                                          \
     static void destroy()                                                      \
     {                                                                          \
         _k_static_##NAME##_destroyed = true;                                   \
         TYPE *x = _k_static_##NAME;                                            \
         _k_static_##NAME = 0;                                                  \
         delete x;                                                              \
     }                                                                          \
} NAME;


/**
 *namespace and singleton class to scan groups of kvtml files
 *from shared kvtml location, and give information
 *about files found there
 *@author Jeremy Whiting
 */
namespace SharedKvtmlFiles
{
    /** get list of all languages found in any files */
    QTVTVOCDOCUMENT_EXPORT QStringList languages();

    /** get list of filenames found of given language
     *@param language language requested QString() for all languages
     *@return a list of filenames with words in language
     */
    QTVTVOCDOCUMENT_EXPORT QStringList fileNames( const QString &language = QString() );

    /** get the list of document titles found of a given language
     *@param language requested language QString() for all titles
     *@return a list of document titles with words in language
     */
    QTVTVOCDOCUMENT_EXPORT QStringList titles( const QString &language = QString() );

    /** get the list of document remarts found of a given language
     *@param language requested language QString() for all comments
     *@return a list of document remarks with words in language
     */
    QTVTVOCDOCUMENT_EXPORT QStringList comments( const QString &language = QString() );

    /** rescan the shared kvtml locations */
    QTVTVOCDOCUMENT_EXPORT void rescan();

    /** sort files downloaded to kvtml top-level dir into locale sub-folders */
    QTVTVOCDOCUMENT_EXPORT void sortDownloadedFiles();
}

#endif

