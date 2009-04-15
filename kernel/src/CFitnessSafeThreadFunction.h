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
** ПРИ СОХРАНЕНИИ ИНФОРМАЦИИ О РАЗРАБОТЧИКАХ ЭТОЙ БИБЛИОТЕКИ.
****************************************************************************/
/**
 * @file    CFitnessSafeThreadFunction.h
 * @brief   Класс CFitnessSafeThreadFunction позволяет обернуть
 *          функцию здоровья для её потокобезопасноного использования
 * @date    23/03/2009
**/
#ifndef C_FITNESS_SAFE_THREAD_FUNCTION_H_INCLUDED
#define C_FITNESS_SAFE_THREAD_FUNCTION_H_INCLUDED
#include <qglobal.h>
#if QT_VERSION < 0x040000
    #include <qmutex.h>
#else
    #include <QtCore/QMutex>
#endif
#include "../../idl/IFitness.h"
namespace GeneticAlgorithm
{
    /**
     * @brief Класс CFitnessSafeThreadFunction позволяет обернуть
     *        функцию здоровья для потокобезопасноного использования
    **/
    struct CFitnessSafeThreadFunction : public IFitness
    {
        /**
         * @brief Конструктор
         * @param fitness - основная функция здоровья
        **/
        CFitnessSafeThreadFunction(IFitness*fitness)
        {
            m_fitness = fitness;
        };
        ~CFitnessSafeThreadFunction()
        {
            m_fitness = NULL;
        };
        /**
         * @brief   Метод вычисления значения целевой функции
         * @return  значение функции
        **/
        double calc(const CChromosome& chr)
        {
            Q_ASSERT(m_fitness);
            QMutexLocker locker(&m_mutex);
            return m_fitness->calc(chr);
        };
        /**
         * @brief   Метод получения наименования функции
         * @return  наименование функции
        **/
        const QString name()
        {
            QMutexLocker locker(&m_mutex);
            return m_fitness->name();
        };
        /**
         * @brief   Метод получения количества рассчитанных целевых функций
         * @return  Количество рассчитанных целевых функций
        **/
        unsigned int count()
        {
            QMutexLocker locker(&m_mutex);
            return m_fitness->count();
        };
    private:
        IFitness*m_fitness;
        QMutex   m_mutex;
    };
};
#endif // C_FITNESS
