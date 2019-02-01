#ifndef BEATESTIMATION_H
#define BEATESTIMATION_H

#include <QObject>
#include <QDateTime>
#include <QVector>
#include <QDebug>


class beatEstimation : public QObject
{
    Q_OBJECT

 QVector<qint64> beatTimeHistory;
 double m_currentBPM;
public:
    explicit beatEstimation(QObject *parent = nullptr);
    double estimateBeatPerSecond();
signals:

public slots:

    void receiveBeat();

};

#endif // BEATESTIMATION_H
