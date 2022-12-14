/****************************************************************
Copyright (C) 2009 Aleksey Myasnikov
This program is free software: you can redistribute it and/or
modify it under the terms of the GNU Lesser GPL as publishedby
the Free Software Foundation, either version 3 of the License,
or (at your option) any later version.
This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU Lesser GPL for more details. You should have received a
copy of the GNU Lesser GPL in file LICENSE along with this
program.  If not, see <http://www.gnu.org/licenses/lgpl.txt>
I draw your attention to the fact that the library InsularGenetica
is registered Russian Agency for Patents and Trademarks (ROSPATENT),
which issued the "Certificate of official registration of computer
programs for N FIXME FIXME FIXME FIXME". A copy of the certificate
of registration is presented in the file CERTIFICATE in the root
of the project. It does not impose on the end developers/users, no
additional restrictions other than those prescribed by GNU Lesser
GPL, while maintaining information about developer this library.
****************************************************************/
/**
 * @file    export.h
 * @brief   Macros definitions
 * @author  Aleksey Myasnikov
 * @date    18/02/2009
 * @version 3.3
**/
#ifndef EXPORT_H_INCLUDED
#define EXPORT_H_INCLUDED
#include <qglobal.h>
#if QT_VERSION < 0x040000
    #include <qgplugin.h>
    #ifdef Q_OS_WIN
        #define Q_DECL_EXPORT __declspec(dllexport)
    #elif defined(QT_VISIBILITY_AVAILABLE)
        #define Q_DECL_EXPORT __attribute__((visibility("default")))
    #endif
#else
    #include <QtCore/qplugin.h>
#endif
#ifndef Q_DECL_EXPORT
    #define Q_DECL_EXPORT
#endif
#define DLLEXPORT Q_EXTERN_C Q_DECL_EXPORT
#endif // EXPORT_H_INCLUDE
