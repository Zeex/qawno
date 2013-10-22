#ifndef COMPILER_H
#define COMPILER_H

#include <QList>
#include <QObject>
#include <QString>
#include <QStringList>

class QProcess;

class Compiler: public QObject {
 Q_OBJECT

 public:
  explicit Compiler(QObject *parent = 0);
  ~Compiler();

  QString path() const;
  void setPath(const QString &path);

  QStringList options() const;
  void setOptions(const QString &options);
  void setOptions(const QStringList &options);

  QString commandLineFor(const QString &inputFile) const;

  bool test() const;
  void run(const QString &inputFile);

  QString output() const;

 public slots:
  void loadSettings();
  void saveSettings();

 signals:
  void finished(int);

 private:
  QString path_;
  QStringList options_;
  QProcess *process_;
};

#endif // COMPILER_H
