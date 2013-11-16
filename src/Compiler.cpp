#include <QFileInfo>
#include <QProcess>
#include <QSettings>

#include "Compiler.h"

Compiler::Compiler(QObject *parent)
  : QObject(parent)
{
  QSettings settings;
  path_ = settings.value("Compiler/Path", "pawncc").toString();
  options_ = settings.value("Compiler/Options", "-;+ -(+").toString().split("\\s*");
}

Compiler::~Compiler() {
  QSettings settings;
  settings.setValue("Compiler/Path", path_);
  settings.setValue("Compiler/Options", options_.join(" "));
}

QString Compiler::path() const {
  return path_;
}

void Compiler::setPath(const QString &path) {
  path_ = path;
}

QStringList Compiler::options() const {
  return options_;
}

void Compiler::setOptions(const QString &options) {
  options_ = options.split("\\s*");
}

void Compiler::setOptions(const QStringList &options) {
  options_ = options;
}

QString Compiler::output() const {
  return output_;
}

QString Compiler::command() const {
  return QString("%1 %2").arg(path_).arg(options_.join(" "));
}

QString Compiler::commandFor(const QString &inputFile) const {
  QFileInfo in(inputFile);
  QFileInfo out(in.absolutePath() + "/" + in.baseName() + ".amx");
  return QString("%1 \"%2\" -o\"%3\"").arg(command()).arg(in.filePath())
                                                     .arg(out.filePath());
}

void Compiler::run(const QString &inputFile) {
  QProcess process;
  process.setProcessChannelMode(QProcess::MergedChannels);

  QString command = commandFor(inputFile);
  process.start(command, QProcess::ReadOnly);

  if (process.waitForFinished()) {
    output_ = process.readAll();
  } else {
    output_ = process.errorString();
  }
}
