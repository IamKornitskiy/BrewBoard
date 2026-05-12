#ifndef LOGGER_H
#define LOGGER_H

#include <QDateTime>
#include <QDir>
#include <QFile>
#include <QList>
#include <QMutex>
#include <QObject>
#include <QString>
#include <QTextStream>

class Logger : public QObject
{
    Q_OBJECT
public:
  enum eLogLevel { Error, Warning, Info, Debug };

  static Logger &instance();
  void setLogFile(const QString &filePath);
  void setMaxSize(qint64 maxSize);
  void log(eLogLevel level, const QString &message, const QString &file = "", int line = -1);

  void setLogLevel(qint8 newLogLevel);

  qint8 LogLevel() const;

  qint64 MaxSize() const;

private:
  Logger(QObject *parent = nullptr);
  ~Logger();

  void consoleOutput(eLogLevel level, const QString &message);
  void rotateLog();
  QString levelToString(eLogLevel level);

  QFile m_LogFile;
  QTextStream m_LogStream;
  QMutex m_Mutex;
  qint64 m_MaxSize;
  qint8 m_LogLevel; //Уровень логирования

signals:
  void newLogMessage(eLogLevel level, const QString &message);
};

#endif // LOGGER_H