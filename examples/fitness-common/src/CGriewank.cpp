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
** Обращаю Ваше внимание на то, что библиотека InsularGenetica
** зарегистрирована Российским агенством по патентам и товарным знакам
** (РОСПАТЕНТ), о чем выдано "Свидетельство об официальной регистрации
** программы для ЭВМ" за № FIXME от FIXME FIXME FIXME года. Копия
** свидетельства о регистрации представлена в файле CERTIFICATE
** в корне проекта.
** Это не накладывает на конечных разработчиков/пользователей никаких
** дополнительных ограничений, кроме предусмотренных GNU Lesser GPL,
** ПРИ СОХРАНЕНИИ ИНФОРМАЦИИ О РАЗРАБОТЧИКЕ ЭТОЙ БИБЛИОТЕКИ.
****************************************************************************/
/**
 * @file    CGriewank.cpp
 * @brief   Файл содержит класс CGriewank,
 *          который реализует тестовую целевую функцию Griewank.
 * @date    10/02/2009
**/

#include "../../include/CGriewank.h"
#include <math.h>
#include <qglobal.h>
#if QT_VERSION < 0x040000
    #include <qstring.h>
    #include <qobject.h>
#else
    #include <QtCore/QString>
    #include <QtCore/QObject>
#endif

/**
 * @brief   Базовый конструктор
**/
CGriewank::CGriewank() : CFitnessHelper(10) {};
/**
 * @brief   Конструктор
 * @param   count - количество переменных целевой функции
**/
CGriewank::CGriewank(unsigned int count, bool invert) :
    CFitnessHelper(count),
    m_invert(invert)
{
};
/**
 * @brief   Деструктор
**/
CGriewank::~CGriewank(){};
/**
 * @brief   Метод вычисления значения целевой функции
 * @return  значение функции
**/
double CGriewank::calc(const InsularGenetica::CChromosome& chr)
{
    double result = 0.;
    double multiplicate = 1.;
    double summ         = 0.;
    for(unsigned long i = 0; i < m_count; i++)
    {
        double x      = (decode(chr, i)-0.5)*40;
        summ         += x*x/4000;
        multiplicate *= cos(x/sqrt(double(i+1)));
    }
    result=(m_invert?-1.:1.)*(multiplicate-summ-1.);
    m_counter++;
    return result;
};

/**
 * @brief   Метод получения наименования функции
 * @return  наименование функции
**/
const QString CGriewank::name()
{
    return QObject::trUtf8("Griewank [%1%2перем.]")
           .arg(m_invert ? QObject::trUtf8("инверт.,") : QString::null)
           .arg(m_count);
};
