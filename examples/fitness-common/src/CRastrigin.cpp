/****************************************************************************
** Copyleft (C) 2009 Мясников Алексей Сергеевич.
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
 * @file    CRastrigin.cpp
 * @brief   Файл содержит класс CRastrigin,
 *          который реализует тестовую целевую функцию Растригина.
 * @date    10/02/2009
**/

#include "../../include/CRastrigin.h"
#include <math.h>
#include <qglobal.h>
#if QT_VERSION < 0x040000
    #include <qstring.h>
    #include <qobject.h>
    #ifndef qMin
        #define qMin(a,b) QMIN((a),(b))
    #endif
    #ifndef qMax
        #define qMax(a,b) QMAX((a),(b))
    #endif
#else
    #include <QtCore/QString>
    #include <QtCore/QObject>
#endif

/**
 * @brief   Базовый конструктор
**/
CRastrigin::CRastrigin() : CFitnessHelper(10) {};
/**
 * @brief   Конструктор
 * @param   count - количество переменных целевой функции
 * @param   filter - Фильтрация хромосом по доле единичных битов [0..1]
**/
CRastrigin::CRastrigin(unsigned int count, bool invert, double filter) :
    CFitnessHelper(count),
    m_invert(invert),
    m_filter(filter)
{
};
/**
 * @brief   Деструктор
**/
CRastrigin::~CRastrigin(){};
/**
 * @brief   Метод вычисления значения целевой функции
 * @return  значение функции
**/
double CRastrigin::calc(const CChromosome& chr)
{
    double result = 0.;
    for(unsigned long i = 0; i < m_count; i++)
    {
        double x = (decode(chr, i)-0.5)*10.24;
        result  += (10.*(cos(2.*M_PI*x)-1) - x*x);
    }
    if(m_filter > 0. && m_filter <= 1.)
    {
        unsigned int count = 0;
        for(uint i = 0; i < CChromosome::size(); i++)
        {
            if(chr.getGene(i)) count++;
        }
        result *= exp(qMax(0.,double(count)-m_filter*double(CChromosome::size())));
    }
    m_counter++;
    return ((m_invert)?(-1.):(1.))*result;
};

/**
 * @brief   Метод получения наименования функции
 * @return  наименование функции
**/
const QString CRastrigin::name()
{
    return QObject::trUtf8("Растригина [%1%2перем.%3]")
           .arg(m_invert ? QObject::trUtf8("инверт.") : QString::null)
           .arg(m_count)
           .arg(m_filter>0.?QObject::trUtf8(",огран.%1%бит").arg(int(m_filter*100.)):QString::null);
};
