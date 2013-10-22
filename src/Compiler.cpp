#include <QFileInfo>
#include <QObject>
#include <QProcess>
#include <QSettings>
#include <QString>
#include <QStringList>

#include "Compiler.h"

Compiler::Compiler(QObject *parent)
  : QObject(parent)
{
  process_ = new QProcess(this);
  process_->setProcessChannelMode(QProcess::MergedChannels);
  connect(process_, SIGNAL(finished(int)), SIGNAL(finished(int)));
}

Compiler::~Compiler() {
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

bool Compiler::test() const {
  QProcess pawncc;
  pawncc.setProcessChannelMode(QProcess::SeparateChannels);
  pawncc.start(path_);
  pawncc.waitForFinished();
  return pawncc.error() != QProcess::FailedToStart;
}

void Compiler::run(const QString &inputFile) {
  process_->start(commandLineFor(inputFile), QProcess::ReadOnly);
}

QString Compiler::commandLineFor(const QString &inputFile) const {
  QFileInfo in(inputFile);
  QFileInfo out(in.absolutePath() + "/" + in.baseName() + ".amx");
  return QString("%1 %2 \"%3\" -o\"%4\"").arg(path_)
                                         .arg(options_.join(" "))
                                         .arg(inputFile)
                                         .arg(out.filePath());
}

QString Compiler::output() const {
  return process_->readAll();
}

void Compiler::loadSettings() {
  QSettings settings;
  settings.beginGroup("Compiler");
    path_ = settings.value("Path").toString();
    if (path_.isEmpty()) {
      path_ = "pawncc"; // Assume compiler is in PATH
    }
    options_ = settings.value("Options").toString().split("\\s*");
  settings.endGroup();
}

void Compiler::saveSettings() {
  QSettings settings;
  settings.beginGroup("Compiler");
    settings.setValue("Path", path_);
    settings.setValue("Options", options_.join(" "));
  settings.endGroup();
}
