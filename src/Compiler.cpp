#include <QFileInfo>
#include <QProcess>
#include <QSettings>

#include "Compiler.h"

Compiler::Compiler() {
  QSettings settings;
  path_ = settings.value("CompilerPath", "pawncc").toString();
  options_ = settings.value("CompilerOptions", "-;+ -(+").toString().split("\\s*");
}

Compiler::~Compiler() {
  QSettings settings;
  settings.setValue("CompilerPath", path_);
  settings.setValue("CompilerOptions", options_.join(" "));
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
  QString fileName = QFileInfo(inputFile).fileName();
  return QString("%1 -c \"%2\"").arg(command(), fileName);
}

void Compiler::run(const QString &inputFile) {
  QProcess process;
  process.setProcessChannelMode(QProcess::MergedChannels);
  process.setWorkingDirectory(QFileInfo(inputFile).absolutePath());

  QString command = commandFor(inputFile);
  process.start(command, QProcess::ReadOnly);

  if (process.waitForFinished()) {
    output_ = process.readAll();
  } else {
    output_ = process.errorString();
  }
}
