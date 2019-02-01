#include "beatestimation.h"

beatEstimation::beatEstimation(QObject *parent) : QObject(parent)
{

}

void beatEstimation::receiveBeat()
{
    beatTimeHistory.append(QDateTime::currentMSecsSinceEpoch());
}


double beatEstimation::estimateBeatPerSecond()
{
    qint64 thisMoment = QDateTime::currentMSecsSinceEpoch();

    QVector<qint64> tempoList;


    for(int i = 0; i < beatTimeHistory.count(); i++)
    {
//        qDebug() << "Delta: " << thisMoment - beatTimeHistory[i] << "beat History count: "  << beatTimeHistory.count();
        if(thisMoment - beatTimeHistory[i] < 1000)
        {

            tempoList.append(beatTimeHistory[i]);
        }
    }
    beatTimeHistory = tempoList;

    if(tempoList.count() >= 2)
    {
      double msPerBeat = (beatTimeHistory.last()- beatTimeHistory.first()) / static_cast<double>(beatTimeHistory.count() );

//      qDebug() << 60000.0 / msPerBeat;
      return 60000.0 / (msPerBeat);
    }
    else {
        return 0;
    }

}
