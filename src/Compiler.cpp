#include <QFileInfo>
#include <QObject>
#include <QProcess>
#include <QSettings>
#include <QString>
#include <QStringList>

#include "Compiler.h"

Compiler::Compiler(QObject *parent)
  : QObject(parent),
    process_(new QProcess(this))
{
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

bool Compiler::works() const {
  QProcess pawncc;
  pawncc.setProcessChannelMode(QProcess::SeparateChannels);
  pawncc.start(path_);
  pawncc.waitForFinished();
  return pawncc.error() != QProcess::FailedToStart;
}

void Compiler::run(const QString &inputFile) {
  process_->start(getCommandLine(inputFile), QProcess::ReadOnly);
}

QString Compiler::getCommandLine(const QString &inputFile) const {
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
  path_ = settings.value("Compiler/Path", "pawncc").toString();
  options_ = settings.value("Compiler/Options", "-;+ -(+").toString().split("\\s*");
}

void Compiler::saveSettings() {
  QSettings settings;
  settings.setValue("Compiler/Path", path_);
  settings.setValue("Compiler/Options", options_.join(" "));
}
