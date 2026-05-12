#include "Logger.h"
#include <QProcess>

Logger &Logger::instance()
{
  static Logger instance;
  return instance;
}

void Logger::setLogFile(const QString &filePath)
{
  QMutexLocker locker(&m_Mutex);

  if (m_LogFile.isOpen())
    m_LogFile.close();

  m_LogFile.setFileName(filePath);
  if (m_LogFile.open(QIODevice::WriteOnly | QIODevice::Append))
    m_LogStream.setDevice(&m_LogFile);
  else
    qCritical() << "Failed to open log file: " << filePath;
}

void Logger::setMaxSize(qint64 maxSize)
{
  QMutexLocker locker(&m_Mutex);
  m_MaxSize = maxSize;
}

void Logger::log(eLogLevel level, const QString &message, const QString &file, int line)
{
  if (level < m_LogLevel)
    return; //Проверка уровня логирования

  QMutexLocker locker(&m_Mutex);

  // Проверяем размер файла и выполняем ротацию, если необходимо
  if (m_LogFile.isOpen() && m_LogFile.size() >= m_MaxSize) {
    rotateLog();
  }

  QString logMessage = QString("[%1] [%2] %3").arg(QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss")).arg(levelToString(level)).arg(message);

  if (!file.isEmpty() && line != -1) {
    logMessage += QString(" (File: %1, Line: %2)").arg(file).arg(line);
  }

  // Выводим сообщение
  m_LogStream << logMessage << Qt::endl;
  consoleOutput(level, logMessage);
}

Logger::Logger(QObject *parent)
    : QObject{parent}
    , m_MaxSize(1024 * 1024)
    , m_LogLevel(Error)
{}

Logger::~Logger()
{
  if (m_LogFile.isOpen())
    m_LogFile.close();
}

void Logger::consoleOutput(eLogLevel level, const QString &message)
{
  switch (level) {
  case Info:
    qInfo() << message;
    break;
  case Warning:
    qWarning() << message;
    break;
  case Error:
    qCritical() << message;
    break;
  case Debug:
    qDebug() << message;
    break;
  default:
    return;
  }
}

void Logger::rotateLog()
{
  if (!m_LogFile.isOpen()) {
    return;
  }

  // Закрываем текущий файл
  m_LogFile.close();

  // Переименовываем текущий файл
  QFileInfo fileInfo(m_LogFile.fileName());
  QString timestamp = QDateTime::currentDateTime().toString("yyyyMMdd_HHmmss");
  QString archiveFileName = fileInfo.path() + "/" + fileInfo.baseName() + "_" + timestamp + "." + fileInfo.completeSuffix();

  if (QFile::rename(m_LogFile.fileName(), archiveFileName)) {
    // Архивируем старый файл
    QProcess gzipProcess;
    gzipProcess.start("gzip", QStringList() << archiveFileName);
    gzipProcess.waitForFinished();
  }

  // Открываем новый файл для записи
  if (!m_LogFile.open(QIODevice::WriteOnly | QIODevice::Append))
    qCritical() << "Failed to open log file after rotation:" << m_LogFile.fileName();

  m_LogStream.setDevice(&m_LogFile);
}

QString Logger::levelToString(eLogLevel level)
{
  switch (level) {
  case Info:
    return "INFO";
  case Warning:
    return "WARNING";
  case Error:
    return "ERROR";
  case Debug:
    return "DEBUG";
  default:
    return "UNKNOWN";
  }
}

qint64 Logger::MaxSize() const
{
  return m_MaxSize;
}

qint8 Logger::LogLevel() const
{
  return m_LogLevel;
}

void Logger::setLogLevel(qint8 logLevel)
{
  m_LogLevel = logLevel;
}