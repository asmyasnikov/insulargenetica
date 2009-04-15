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
 * @file    CGeneticController.cpp
 * @brief   Файл содержит реализацию класса CGeneticController, который
 *          отвечает за бизнес-логику островной модели ГА
 * @date    23/03/2009
**/
#include <qglobal.h>
#include <qglobal.h>
#if QT_VERSION < 0x040000
    #include <qlibrary.h>
    #include <qdir.h>
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
    #include <QtCore/QLibrary>
    #include <QtCore/QDir>
    #include <QtCore/QTime>
    #include <QtCore/QString>
    #include <QtCore/QStringList>
    #include <QtCore/QObject>
#endif
#include "../../include/CGeneticController.h"
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
GeneticAlgorithm::
CGeneticController::
CGeneticController(IFitness*     fitness,
                   unsigned int  population,
                   unsigned int  island,
                   unsigned long minutes )
{
    m_function = new CFitnessSafeThreadFunction(fitness);
    CChromosome::setFitnessFunction(m_function);
    m_best_solutions_size = population;
    m_best_solutions = CPopulation(0);
    m_minutes = minutes;
    loadLibraries("selection"   );
    loadLibraries("grouping"    );
    loadLibraries("reproduction");
    loadLibraries("mutation"    );
    loadLibraries("accepting"   );
    loadLibraries("."   );
    for(unsigned int i = 0; i < island; i++)
    {
        CGeneticAlgorithm*alg = new CGeneticAlgorithm(m_operators,m_minutes);
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
GeneticAlgorithm::
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
    for(
#if QT_VERSION < 0x040000
        QValueList<QLibrary*>::iterator i = m_load_libs.begin();
#else
        QList<QLibrary*>::iterator i = m_load_libs.begin();
#endif
        i != m_load_libs.end(); i++)
    {
        (*i)->unload();
        delete (*i);
    };
    m_load_libs.clear();
#if QT_VERSION < 0x040000
    if(running())
#else
    if(isRunning())
#endif
    {
#if QT_VERSION >= 0x040000
        setTerminationEnabled();
        terminate();
#endif
        wait();
    }
    delete m_function;
};
/**
 * @brief Основной цикл потока
**/
void
GeneticAlgorithm::
CGeneticController::
run()
{
    QTime timer;
    timer.start();
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
            alg->start(QThread::LowestPriority);
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
GeneticAlgorithm::
CPopulation
GeneticAlgorithm::
CGeneticController::
calc(IFitness*    fitness,
     unsigned int chromosom,
     unsigned int population,
     unsigned int minutes,
     int          island)
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
    CGeneticController control(fitness, population, island, minutes);
    control.start(QThread::LowPriority);
    control.wait();
    return control.m_best_solutions;
};
/**
 * @brief Шаблон позволяет загружать библиотки из соответствующих
 *        поддирректорий
 * @param map   - словарь, в который следует поместить модули
 *                генетических операторов
 * @param name  - наименование поддирректории, в которой следует
 *                производить поиск
**/
void
GeneticAlgorithm::
CGeneticController::
loadLibraries(const QString& name)
{
    QDir dir(name);
    if(dir.exists())
    {
        dir.setFilter(QDir::Files);
#if QT_VERSION < 0x040000
        dir.setNameFilter("*.so *.dll");
#else
        QStringList filters;
        filters.append("*.so");
        filters.append("*.dll");
        dir.setNameFilters(filters);
#endif
        QStringList list = dir.entryList();
        for (int i = 0; i < list.size(); ++i)
        {
#if QT_VERSION < 0x040000
            if((*list.at(i)).contains("genetic-"))
#else
            if(list.at(i).contains("genetic-"))
#endif
            {
#if QT_VERSION < 0x040000
                QLibrary*lib = new QLibrary(dir.absFilePath(*list.at(i)));
#else
                QLibrary*lib = new QLibrary(dir.absoluteFilePath(list.at(i)));
#endif
                if(!lib->isLoaded() && !lib->load())
                {
                    qDebug("%s",
                           QObject::trUtf8("Библиотека \"%1\" "
                                           "не загружена : %2")
#if QT_VERSION < 0x040000
                           .arg(*list.at(i))
                           .arg(QString::null)
                           .local8Bit()
#else
                           .arg(list.at(i))
                           .arg(lib->errorString())
                           .toLocal8Bit()
#endif
                           .data());
                    continue;
                }
                typedef Interface*(*CreateModule)(void);
                CreateModule createModule =
                    (CreateModule)(lib->resolve("CreateModule"));
                if(!createModule)
                {
                    qDebug("%s",
                           QObject::trUtf8("Функция \'CreateModule\' не "
                                           "найдена в библиотеке\"%1\": %2")
#if QT_VERSION < 0x040000
                           .arg(*list.at(i))
                           .arg(QString::null)
                           .local8Bit()
#else
                           .arg(list.at(i))
                           .arg(lib->errorString())
                           .toLocal8Bit()
#endif
                           .data());
                    continue;
                }else{
                    Interface*iface = createModule();
                    if(iface)
                    {
                        IGeneticOperator* oper =
                            dynamic_cast<IGeneticOperator*>(iface);
                        if(oper)
                        {
                            m_operators.append(oper);
                            m_load_libs.append(lib);
                            continue;
                        }else{
                            delete iface;
                        }
                    }
                }
                lib->unload();
                delete lib;
            }
        }
    }
};
/**
 * @brief Отмена расчета алгоритма
**/
void
GeneticAlgorithm::
CGeneticController::
cancel()
{
    for(
#if QT_VERSION < 0x040000
        QValueList<CGeneticAlgorithm*>::iterator i = m_algorithms.begin();
#else
        QList<CGeneticAlgorithm*>::iterator i = m_algorithms.begin();
#endif
        i != m_algorithms.end(); i++)
    {
#if QT_VERSION < 0x040000
        if(!(*i)->running())
#else
        if(!(*i)->isRunning())
#endif
        {
            (*i)->cancel();
        }
    }
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
GeneticAlgorithm::
CGeneticController*
GeneticAlgorithm::
CGeneticController::
getCalculator(IFitness*    fitness,
              unsigned int chromosom,
              unsigned int population,
              unsigned int minutes,
              int          island )
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
                                                        minutes);
    return control;
};
/**
 * @brief Получить результаты расчета
**/
GeneticAlgorithm::
CPopulation
GeneticAlgorithm::
CGeneticController::
getBestSolutions(int size)
{
    start(QThread::LowPriority);
    wait();
    CPopulation result;
    for(int i = 0; i < m_best_solutions.size() && i < size; i++)
    {
        result.addChromosome(m_best_solutions.getChromosome(i));
    }
    return result;
};
