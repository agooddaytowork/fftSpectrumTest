#include "fftvisualizer.h"

fftVisualizer::fftVisualizer(QObject *parent) : QObject(parent),m_fftCalculator(new FFTCalc()), m_audioProbe(new QAudioProbe()), m_recorder(new QAudioRecorder()), m_beatDetectoKick(new beatDetector()),m_beatDetectorSnare(new beatDetector()),m_beatEstimation(new beatEstimation())
{
    QAudioEncoderSettings audioSettings;
    audioSettings.setCodec("audio/pcm");
    audioSettings.setQuality(QMultimedia::HighQuality);
    audioSettings.setChannelCount(2);
    audioSettings.setSampleRate(44100);

    m_recorder->setEncodingSettings(audioSettings);
    m_recorder->setAudioSettings(audioSettings);


    m_beatDetectoKick->setRange(1,3);
    m_beatDetectorSnare->setRange(8,14);

    if(m_audioProbe->setSource(m_recorder))
    {
        QObject::connect(m_audioProbe,&QAudioProbe::audioBufferProbed,this,&fftVisualizer::processAudioBuffer);
//        QObject::connect(m_audioProbe,&QAudioProbe::audioBufferProbed,m_fftCalculator,&FFTCalc::);

        QObject::connect(m_fftCalculator,&FFTCalc::calculatedSpectrum,this,&fftVisualizer::spectrumCalculated);

        QObject::connect(m_beatDetectoKick,&beatDetector::beatFound,[=](){

            qDebug() << "Drum Found";
            emit beatDetected(1);

        });

        QObject::connect(m_beatDetectoKick,&beatDetector::beatFound,m_beatEstimation,&beatEstimation::receiveBeat);

        QObject::connect(m_fftCalculator,&FFTCalc::calculatedSpectrum,this,&fftVisualizer::spectrumCalculated);

        QObject::connect(m_beatDetectorSnare,&beatDetector::beatFound,[=](){

//            qDebug() << "Snare Found";
            emit beatDetected(2);
        });


    }



}

void fftVisualizer::start()
{
    m_recorder->record();
}

void fftVisualizer::stop()
{
    m_recorder->stop();
}

double fftVisualizer::getSubandInstantEnergy(const int &subBandNo)
{
    if(subBandNo >= m_spectrum.count()) return -1.0;

    return m_spectrum[subBandNo];
}
void fftVisualizer::spectrumCalculated(QVector<double> spectrum)
{
   qDebug() << ".";
   emit clearBeatDetect();


    static bool lowDetected = false;
    m_spectrum = spectrum;

    m_beatDetectoKick->detectBeat(m_spectrum);
    m_beatDetectorSnare->detectBeat(m_spectrum);

//    bool beatFound=false;
//    float specLow=spectrum[0]+spectrum[1]+spectrum[2];
//    if(specLow<1){
//        lowDetected=true;
//    }
//    else if(specLow>2){
//        beatFound=true;
//    }

//    if(lowDetected && beatFound){

////        qDebug() << "Beat";
//            lowDetected=false;

//            emit beatDetected();
//    }


    qDebug() << "BPM: " << m_beatEstimation->estimateBeatPerSecond();
    emit newInstantEnergyAvailable();
}


void fftVisualizer::processAudioBuffer(QAudioBuffer buffer)
{

//    qDebug() << ".";
//    qreal peakValue;
//    if(buffer.format().channelCount() != 2)
//      return;

//    sample.resize(buffer.frameCount());
//    B.resize(buffer.frameCount());
//    E.resize(32);

//    // audio is signed int
//    if(buffer.format().sampleType() == QAudioFormat::SignedInt){

//      QAudioBuffer::S16S *data = buffer.data<QAudioBuffer::S16S>();
//      // peak value changes according to sample size.
//      if (buffer.format().sampleSize() == 32)
//        peakValue=INT_MAX;
//      else if (buffer.format().sampleSize() == 16)
//        peakValue=SHRT_MAX;
//      else
//        peakValue=CHAR_MAX;

//      for(int i=0; i<buffer.frameCount(); i++){
//          B[i] = (data[i].left)*(data[i].left/peakValue) + (data[i].right)*(data[i].right/peakValue);
//      }

//      for(int i = 0; i < 32; i++)
//      {
//          double Bsum = 0.0;
//         for(int k = 0; k < 32*(i+1); k++)
//         {
//             Bsum = Bsum + B[k];
//         }
//         E[i] = 32.0/static_cast<double>(buffer.frameCount()) * Bsum;
//      }

////      QDebug deb = qDebug();


////      for(int i = 0; i < E.count(); i++)
////      {

////          deb << E[i];
////      }
//      if(H0.size() <= 43)
//      {
//          H0.append(E[1]);
//      }
//      else {

//              H0.removeFirst();
//              H0.append(E[1]);

//          double AverageEnergy = 0.0;
//          double H0Sum = 0.0;
//          for(int i = 0; i < 43; i++)
//          {
//                H0Sum += H0[i];
//          }

//          AverageEnergy = 1.0/43.0 * H0Sum;

//          if(E[1] > AverageEnergy*1.5)
//          {
////              qDebug() << "Beat found";
//          }
//      }

//    }

//    emit newInstantEnergyAvailable();

    qreal peakValue;
    int duration;

    if(buffer.frameCount() < SPECSIZE)
      return;

    // return left and right audio mean levels
    m_LevelLeft = m_LevelRight = 0;
    // It only knows how to process stereo audio frames
    // mono frames = :P
    if(buffer.format().channelCount() != 2)
      return;

    sample.resize(buffer.frameCount());

  //  qDebug() << " alo = " << buffer.format().sampleSize() << endl;

    // audio is signed int
    if(buffer.format().sampleType() == QAudioFormat::SignedInt){
      QAudioBuffer::S16S *data = buffer.data<QAudioBuffer::S16S>();
      // peak value changes according to sample size.
      if (buffer.format().sampleSize() == 32)
        peakValue=INT_MAX;
      else if (buffer.format().sampleSize() == 16)
        peakValue=SHRT_MAX;
      else
        peakValue=CHAR_MAX;


      //peakValue = 32767;
      //qDebug() << " Pico Signed: ------    " << peakValue << endl;
      // scale everything to [0,1]
      for(int i=0; i<buffer.frameCount(); i++){
        // for visualization purposes, we only need one of the
        // left/right channels
        sample[i] = data[i].left/peakValue;
        m_LevelLeft+= abs(data[i].left)/peakValue;
        m_LevelRight+= abs(data[i].right)/peakValue;
      }
    }

    // audio is unsigned int
    else if(buffer.format().sampleType() == QAudioFormat::UnSignedInt){
      QAudioBuffer::S16U *data = buffer.data<QAudioBuffer::S16U>();
      if (buffer.format().sampleSize() == 32)
        peakValue=UINT_MAX;
      else if (buffer.format().sampleSize() == 16)
        peakValue=USHRT_MAX;
      else
        peakValue=UCHAR_MAX;

      //peakValue = 32767;
      //qDebug() << " Pico Unsigned: ------    " << peakValue << endl;

      for(int i=0; i<buffer.frameCount(); i++){
        sample[i] = data[i].left/peakValue;
        m_LevelLeft+= abs(data[i].left)/peakValue;
        m_LevelRight+= abs(data[i].right)/peakValue;
      }
    }

    // audio is float type
    else if(buffer.format().sampleType() == QAudioFormat::Float){
      QAudioBuffer::S32F *data = buffer.data<QAudioBuffer::S32F>();
      peakValue = 1.00003;
      for(int i=0; i<buffer.frameCount(); i++){
        sample[i] = data[i].left/peakValue;
        // test if sample[i] is infinity (it works)
        // some tests produced infinity values :p
        if(sample[i] != sample[i]){
          sample[i] = 0;
        }
        else{
          m_LevelLeft+= abs(data[i].left)/peakValue;
          m_LevelRight+= abs(data[i].right)/peakValue;
        }
      }
    }
    //qDebug() << peakValue<<endl;
    // if the probe is listening to the audio
    // do fft calculations
    // when it is done, calculator will tell us
    if(m_audioProbe->isActive()){
        //qDebug() << "buffer" << sample[4] << endl;

      duration = buffer.format().durationForBytes(buffer.frameCount())/1000;
      m_fftCalculator->calc(sample, duration);
    }
    // tells anyone interested about left and right mean levels
//    emit levels(levelLeft/buffer.frameCount(),levelRight/buffer.frameCount());

}
