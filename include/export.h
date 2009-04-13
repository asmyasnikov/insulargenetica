/****************************************************************************
** Copyright (C) 2009 Мясников Алексей Сергеевич.
** Contact: AlekseyMyasnikov@yandex.ru
**          amyasnikov@npomis.ru
**          AlekseyMyasnikov@mail.ru
**          MyasnikovAleksey@mail.ru
** Этот файл является частью реализации библиотеки островного генетического
** алгоритма с динамическим распределением вероятностей выбора генетических
** операторов в каждой группе операторов
** Данная библиотека является свободным программным обеспечением. Вы вправе
** распространять её и/или модифицировать в соответствии с условиями версии 3
** либо по вашему выбору с условиями более поздней версии Стандартной
** Общественной Лицензии Ограниченного Применения GNU, опубликованной
** Free Software Foundation.
** Я распространяю эту библиотеку в надежде на то, что она будет вам
** полезной, однако НЕ ПРЕДОСТАВЛЯЮ НА НЕЕ НИКАКИХ ГАРАНТИЙ, в том числе
** ГАРАНТИИ ТОВАРНОГО СОСТОЯНИЯ ПРИ ПРОДАЖЕ и ПРИГОДНОСТИ ДЛЯ ИСПОЛЬЗОВАНИЯ
** В КОНКРЕТНЫХ ЦЕЛЯХ. Для получения более подробной информации ознакомьтесь
** со Стандартной Общественной Лицензией Ограниченного Применений GNU в
** файле LICENSE в корне исходных текстов проекта или по адресу:
** http://www.gnu.org/copyleft/lgpl.html.
****************************************************************************/
/**
 * @file    export.h
 * @brief   Файл содержит объявление макроса экпортирования
 *          внешних функций библиотеки
 * @date    18/02/2009
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

#endif // EXPORT_H_INCLUDED
