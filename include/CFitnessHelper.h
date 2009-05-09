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
 * @file    CFitnessHelper.h
 * @brief   Файл содержит реализацию вспомогательных методов,
 *          необходимых при расчете целевой функции
 * @date    20/02/2009
 * @version 1.15
**/
#ifndef C_FITNESS_HELPER_HEADER
#define C_FITNESS_HELPER_HEADER
#include "../idl/IFitness.h"
#include "export.h"
namespace InsularGenetica
{
    class CChromosome;
    struct Q_DECL_EXPORT CFitnessHelper : virtual public IFitness
    {
        /**
         * @brief   Базовый конструктор
        **/
        CFitnessHelper(unsigned int count);
        /**
         * @brief   Деструктор
        **/
        ~CFitnessHelper();
        /**
         * @brief   Метод получения количества рассчитанных целевых функций
         * @return  Количество рассчитанных целевых функций
        **/
        unsigned int count();
    protected:
        unsigned int    m_count;  ///<! Количество аргументов функции
        unsigned int    m_counter;///<! Счетчик обращений к функции
        /** @brief  Метод для преобразования хромосомы (части хромосомы)
                    к типу double. Перед использованием этого метода
                    переменная m_count обязательно должна быть
                    инициализирована ненулевым положительным значеним.
            @param  chr     - хромосома,
            @param  index   - номер переменной,
            @return значение в диапазоне от 0 до 1, соответствующее
                    участку хромосомы
        **/
        double decode(const CChromosome& chr,
                      unsigned int       index) const;
    };
};
#endif // C_FITNESS_
