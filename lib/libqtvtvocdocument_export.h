/***************************************************************************
*   this file is from kdeedu project. Filename: libkeduvocdocument_export.h
***************************************************************************/

/*  This file is part of the KDE project
    Copyright (C) 2007 David Faure <faure@kde.org>
    Copyright (C) 2010 Reto Zingg <g.d0b3rm4n@gmail.com>

    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Library General Public
    License as published by the Free Software Foundation; either
    version 2 of the License, or (at your option) any later version.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Library General Public License for more details.

    You should have received a copy of the GNU Library General Public License
    along with this library; see the file COPYING.LIB.  If not, write to
    the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
    Boston, MA 02110-1301, USA.
*/

#ifndef LIBQTVTVOCDOCUMENT_EXPORT_H
#define LIBQTVTVOCDOCUMENT_EXPORT_H

/* needed for KDE_EXPORT and KDE_IMPORT macros */
// #include <kdemacros.h>

#define KDE_EXPORT __attribute__ ((visibility("default")))
#define KDE_IMPORT __attribute__ ((visibility("default")))

#ifdef __cplusplus
# include <QtCore/qglobal.h>
# ifndef KDE_DEPRECATED
#  ifdef KDE_DEPRECATED_WARNINGS
#   define KDE_DEPRECATED Q_DECL_DEPRECATED
#  else
#   define KDE_DEPRECATED
#  endif
# endif
#endif



#ifndef KEDUVOCDOCUMENT_EXPORT
# if defined(MAKE_KEDUVOCDOCUMENT_LIB)
/* We are building this library */
#  define QTVTVOCDOCUMENT_EXPORT KDE_EXPORT
# else
/* We are using this library */
#  define QTVTVOCDOCUMENT_EXPORT KDE_IMPORT
# endif
#endif

# ifndef KEDUVOCDOCUMENT_EXPORT_DEPRECATED
#  define KEDUVOCDOCUMENT_EXPORT_DEPRECATED KDE_DEPRECATED QTVTVOCDOCUMENT_EXPORT
# endif

#endif

