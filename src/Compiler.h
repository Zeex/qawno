#ifndef COMPILER_H
#define COMPILER_H

#include <QString>
#include <QStringList>

class Compiler: public QObject {
 Q_OBJECT

 public:
  explicit Compiler(QObject *parent = 0);
  ~Compiler() override;

  QString path() const;
  void setPath(const QString &path);

  QStringList options() const;
  void setOptions(const QString &options);
  void setOptions(const QStringList &options);

  QString output() const;

  QString command() const;
  QString commandFor(const QString &inputFile) const;

  void run(const QString &inputFile);

 private:
  QString path_;
  QStringList options_;
  QString output_;
};

#endif // COMPILER_H
