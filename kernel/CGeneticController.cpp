/****************************************************************
Copyright (C) 2009 Aleksey Myasnikov
This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.
This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.
You should have received a copy of the GNU General Public License
in file LICENSE along with this program.  If not, see
<http://www.gnu.org/licenses/>
I draw your attention to the fact that the library InsularGenetica
is registered Russian Agency for Patents and Trademarks (ROSPATENT),
which issued the "Certificate of official registration of computer
programs for № FIXME FIXME FIXME FIXME". A copy of the certificate
of registration is presented in the file CERTIFICATE in the root
of the project. It does not impose on the end developers / users,
no additional restrictions other than those prescribed by GNU Lesser
GPL, while maintaining information about developer this library.
****************************************************************/
/**
 * @file    CGeneticController.cpp
 * @brief   Файл содержит реализацию класса CGeneticController, который
 *          отвечает за бизнес-логику островной модели ГА
 * @date    23/03/2009
 * @version 1.18
**/
#include <qglobal.h>
#if QT_VERSION < 0x040000
    #include <qdatetime.h>
    #include <qstringlist.h>
    #include <qobject.h>
    #include <qstring.h>
    #ifndef qMin
        #define qMin(a,b) QMIN((a),(b))
    #endif
    #ifndef qMax
        #define qMax(a,b) QMAX((a),(b))
    #endif
#else
    #include <QtCore/QTime>
    #include <QtCore/QString>
    #include <QtCore/QStringList>
    #include <QtCore/QObject>
#endif
#include "../include/CGeneticController.h"
#include "../operators/selection/CElitarSelection.h"
#include "../operators/selection/CRandomSelection.h"
#include "../operators/selection/CRouletteSelection.h"
#include "../operators/grouping/CAllWithAll.h"
#include "../operators/grouping/CBestWithAll.h"
#include "../operators/grouping/CBestWithBest.h"
#include "../operators/grouping/CInBreeding.h"
#include "../operators/grouping/COutBreeding.h"
#include "../operators/grouping/CRandomGrouping.h"
#include "../operators/reproduction/COneDotCrossover.h"
#include "../operators/reproduction/CTwoDotCrossover.h"
#include "../operators/reproduction/CUnifiedCrossover.h"
#include "../operators/mutation/CFullMutation.h"
#include "../operators/mutation/COneDotMutation.h"
#include "../operators/mutation/CRandomMutation.h"
#include "../operators/mutation/CTwoDotMutation.h"
#include "../operators/accepting/CAnyChromosome.h"
#include "../operators/accepting/CBestFitness.h"
#include "../operators/accepting/CBetterAverageFitness.h"
#include "CGeneticAlgorithm.h"
#include "CFitnessSafeThreadFunction.h"
#ifdef Q_OS_UNIX
    #if QT_VERSION < 0x040000
        #include <qfile.h>
        #include <qtextstream.h>
    #else
        #include <QtCore/QFile>
        #include <QtCore/QTextStream>
    #endif
#endif
#ifdef Q_OS_WIN
    #include <windows.h>
#endif
/**
 * @brief Конструктор
 * @param island - количество островов
**/
InsularGenetica::
CGeneticController::
CGeneticController(IFitness*     fitness,
                   unsigned int  population,
                   unsigned int  island,
                   unsigned long minutes,
                   ICancelService* cancel_service) :
    m_cancel_service(cancel_service),
    m_is_calculate(false),
    m_mutex(
#if QT_VERSION < 0x040000
            true
#else
            QMutex::Recursive
#endif
           )
{
    m_function = new CFitnessSafeThreadFunction(fitness);
    CChromosome::setFitnessFunction(m_function);
    m_best_solutions_size = population;
    m_best_solutions = CPopulation(0);
    m_minutes = minutes;
    m_operators.append(new CElitarSelection(0.1));
    m_operators.append(new CElitarSelection(0.2));
    m_operators.append(new CElitarSelection(0.3));
    m_operators.append(new CElitarSelection(0.4));
    m_operators.append(new CElitarSelection(0.5));
    m_operators.append(new CElitarSelection(0.6));
    m_operators.append(new CRandomSelection);
    m_operators.append(new CRouletteSelection);
    m_operators.append(new COutBreeding);
    m_operators.append(new CAllWithAll);
    m_operators.append(new CBestWithAll);
    m_operators.append(new CBestWithBest);
    m_operators.append(new CInBreeding);
    m_operators.append(new CRandomGrouping);
    m_operators.append(new COneDotCrossover);
    m_operators.append(new CTwoDotCrossover);
    m_operators.append(new CUnifiedCrossover);
    m_operators.append(new COneDotMutation);
    m_operators.append(new CTwoDotMutation);
    m_operators.append(new CFullMutation);
    m_operators.append(new CRandomMutation(0.25));
    m_operators.append(new CRandomMutation(0.50));
    m_operators.append(new CRandomMutation(0.75));
    m_operators.append(new CAnyChromosome);
    m_operators.append(new CBestFitness);
    m_operators.append(new CBetterAverageFitness);
    for(unsigned int i = 0; i < island; i++)
    {
        CGeneticAlgorithm*alg = new CGeneticAlgorithm(this, m_operators,m_minutes);
        m_algorithms.append(alg);
    };
    CGeneticAlgorithm*prev = NULL;
    for(
#if QT_VERSION < 0x040000
        QValueList<CGeneticAlgorithm*>::iterator i = m_algorithms.begin();
#else
        QList<CGeneticAlgorithm*>::iterator i = m_algorithms.begin();
#endif
        i != m_algorithms.end(); i++
       )
    {
        if(prev)
        {
            prev->setNeighbourAlgorithm(*i);
        }
        prev = (*i);
    }
    if(prev && (m_algorithms.size()>1))
    {
        prev->setNeighbourAlgorithm(*m_algorithms.begin());
    }
};
/**
 * @brief Деструктор
**/
InsularGenetica::
CGeneticController::
~CGeneticController(void)
{
    for(
#if QT_VERSION < 0x040000
        QValueList<CGeneticAlgorithm*>::iterator i = m_algorithms.begin();
#else
        QList<CGeneticAlgorithm*>::iterator i = m_algorithms.begin();
#endif
        i != m_algorithms.end(); i++)
    {
        delete (*i);
    }
    m_algorithms.clear();
    for(
#if QT_VERSION < 0x040000
        QValueList<IGeneticOperator*>::iterator i = m_operators.begin();
#else
        QList<IGeneticOperator*>::iterator i = m_operators.begin();
#endif
        i != m_operators.end(); i++
       )
    {
        delete (*i);
    }
    m_operators.clear();
    delete m_function;
};
/**
 * @brief Основной цикл потока
**/
void
InsularGenetica::
CGeneticController::
calculate()
{
    if(!m_is_calculate)
    {
#if QT_VERSION < 0x040000
        for(QValueList<CGeneticAlgorithm*>::iterator i = m_algorithms.begin();
            i != m_algorithms.end(); i++)
        {
            CGeneticAlgorithm*alg = *i;
            if(!alg->running())
#else
        foreach(CGeneticAlgorithm*alg, m_algorithms)
        {
            if(!alg->isRunning())
#endif
            {
                alg->start();
            }
        }
#if QT_VERSION < 0x040000
        for(QValueList<CGeneticAlgorithm*>::iterator i = m_algorithms.begin();
            i != m_algorithms.end(); i++)
        {
            CGeneticAlgorithm*alg = *i;
            if(alg->running())
#else
        foreach(CGeneticAlgorithm*alg, m_algorithms)
        {
            if(alg->isRunning())
#endif
            {
                alg->wait();
            }
            CPopulation pop = alg->
                              getBestChromosomes(qMax(int(m_best_solutions_size)/
                                                      m_algorithms.size(),1));
            for(int j = 0; j < pop.size(); j++)
            {
                if(m_best_solutions.size() < int(m_best_solutions_size))
                {
                    m_best_solutions.addChromosome(pop.getChromosome(j));
                }else{
                    m_best_solutions.replaceChromosome(pop.getChromosome(j));
                }
            }
        }
        m_is_calculate = true;
    }
};
/**
 * @brief Статический метод поиска решения
 * @param fitness - целевая функция (функция здоровья),
 * @param chromosom - размер хромосомы,
 * @param population - размер популяции,
 * @param minutes - максимальное количество минут для расчета,
 * @param island - количество островов алгоритма. При (island < 1)
 *                 количество островов определяется автоматически
 *                 (в2 раза больше числа установленных процессоров) .
 *  `              По определению, надежность решения повышается при
 *                 увеличении числа островов. Однако время поиска
 *                 увеличивается за счет потребления ресурсов при
 *                 передаче управления "островам".
**/
InsularGenetica::
CPopulation
InsularGenetica::
CGeneticController::
calc(IFitness*       fitness,
     unsigned int    chromosom,
     unsigned int    population,
     unsigned int    minutes,
     int             island,
     ICancelService* cancel_service)
{
    Q_ASSERT(population);
    Q_ASSERT(chromosom);
    Q_ASSERT(fitness);
    minutes = qMax(minutes,(unsigned int)(1));
    if(island < 1)
    {
#ifdef Q_OS_WIN
        SYSTEM_INFO sysInfo;
        GetSystemInfo(&sysInfo);
        island = sysInfo.dwNumberOfProcessors;
#endif
#ifdef Q_OS_UNIX
        QFile file("/proc/cpuinfo");
        if ( file.open(
#if QT_VERSION < 0x040000
                       IO_ReadOnly
#else
                       QIODevice::ReadOnly
#endif
                      ) )
        {
            island = 0;
            QTextStream textStream(&file);
            do{
                if ( textStream.readLine().startsWith("processor") )
                     island++;
            }while(!textStream.atEnd());
            file.close();
        }
#endif
        island *= 2;
    };
    CGeneticAlgorithm::setPopulationSize(population);
    CChromosome::setSize(chromosom);
    CGeneticController control(fitness, population, island, minutes, cancel_service);
    control.calculate();
    return control.m_best_solutions;
};
/**
 * @brief Статический метод конструирования потока "калькулятора"
 * @param fitness - целевая функция (функция здоровья),
 * @param chromosom - размер хромосомы,
 * @param population - размер популяции,
 * @param minutes - максимальное количество минут для расчета,
 * @param island - количество островов алгоритма. При (island = -1)
 *                 количество островов определяется автоматически
 *                 (в2 раза больше числа установленных процессоров) .
 *  `              По определению, надежность решения повышается при
 *                 увеличении числа островов. Однако время поиска
 *                 увеличивается за счет потребления ресурсов при
 *                 передаче управления "островам".
**/
InsularGenetica::
CGeneticController*
InsularGenetica::
CGeneticController::
getCalculator(IFitness*       fitness,
              unsigned int    chromosom,
              unsigned int    population,
              unsigned int    minutes,
              int             island,
              ICancelService* cancel_service )
{
    Q_ASSERT(population);
    Q_ASSERT(chromosom);
    Q_ASSERT(fitness);
    minutes = qMax(minutes,(unsigned int)(1));
    if(island < 1)
    {
#ifdef Q_OS_WIN
        SYSTEM_INFO sysInfo;
        GetSystemInfo(&sysInfo);
        island = sysInfo.dwNumberOfProcessors;
#endif
#ifdef Q_OS_UNIX
        QFile file("/proc/cpuinfo");
        if ( file.open(
#if QT_VERSION < 0x040000
                       IO_ReadOnly
#else
                       QIODevice::ReadOnly
#endif
                      ) )
        {
            island = 0;
            QTextStream textStream(&file);
            do{
                if ( textStream.readLine().startsWith("processor") )
                     island++;
            }while(!textStream.atEnd());
            file.close();
        }
#endif
        island *= 2;
    };
    CGeneticAlgorithm::setPopulationSize(population);
    CChromosome::setSize(chromosom);
    CGeneticController*control = new CGeneticController(fitness,
                                                        population,
                                                        island,
                                                        minutes,
                                                        cancel_service);
    return control;
};
/**
 * @brief Получить результаты расчета
**/
InsularGenetica::
CPopulation
InsularGenetica::
CGeneticController::
getBestSolutions(int size)
{
    if(!m_is_calculate) calculate();
    CPopulation result;
    for(int i = 0; i < m_best_solutions.size() && i < size; i++)
    {
        result.addChromosome(m_best_solutions.getChromosome(i));
    }
    return result;
};
/**
 * @brief   This method provides canceling evaluations.
 *          The method may be not safe-thread.
 * @return  cancel status
**/
bool
InsularGenetica::
CGeneticController::
isCanceled()
{
    QMutexLocker locker(&m_mutex);
    if(m_cancel_service) return m_cancel_service->isCanceled();
    else                 return false;
};
