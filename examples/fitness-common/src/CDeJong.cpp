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
 * @file    CDeJong.cpp
 * @brief   Файл содержит класс CDeJong,
 *          который реализует тестовую целевую функцию DeJong'а.
 * @date    22/02/2009
**/

#include "../../include/CDeJong.h"
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
CDeJong::CDeJong() : CFitnessHelper(10) {};
/**
 * @brief   Конструктор
 * @param   count - количество переменных целевой функции
**/
CDeJong::CDeJong(unsigned int count, bool invert) :
    CFitnessHelper(count),
    m_invert(invert)
{
    Q_ASSERT((m_count == 2) || (m_count == 5));
};
/**
 * @brief   Деструктор
**/
CDeJong::~CDeJong(){};
/**
 * @brief   Метод вычисления значения целевой функции
 * @return  значение функции
**/
double CDeJong::calc(const InsularGenetica::CChromosome& chr)
{
    double result = (m_count == 2 ? 0.002 : 0.);
    if(m_count == 2)
    {
        double x1 = (decode(chr, 0)-0.5)*131.072;
        double x2 = (decode(chr, 1)-0.5)*131.072;
        for(int j = 1; j < 26; j++)
        {
            result += 1./(double(j)+pow(x1-16.*(double(j/5)-2.),6.)+
                                    pow(x2-16.*(double(j)-5.*double(j/5)-2.),6.));
        }
    }else if(m_count == 5){
        for(unsigned long i = 0; i < m_count; i++)
        {
            double x = (decode(chr, i)-0.5)*10.24;
            result += floor(x);
        }
    }
    m_counter++;
    return result;
};

/**
 * @brief   Метод получения наименования функции
 * @return  наименование функции
**/
const QString CDeJong::name()
{
    return QObject::trUtf8("DeJong [%1%2перем.]")
           .arg(m_invert ? QObject::trUtf8("инверт.,") : QString::null)
           .arg(m_count);
};
