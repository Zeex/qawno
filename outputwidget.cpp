#include <QSettings>

#include "outputwidget.h"

OutputWidget::OutputWidget(QWidget *parent) : QPlainTextEdit(parent)
{
	QSettings settings;
	settings.beginGroup("Font");
		QFont font;
		settings.beginGroup("Output");
			font.setFamily(settings.value("Family", "Courier New").toString());
			font.setPointSize(settings.value("PointSize", 10).toInt());
			font.setBold(settings.value("Bold", false).toBool());
			font.setItalic(settings.value("Italic", false).toBool());
			QPlainTextEdit::setFont(font);
		settings.endGroup();
	settings.endGroup();
}

void OutputWidget::setFont(const QFont &font)
{
	QSettings settings;
	settings.beginGroup("Font");
		settings.beginGroup("Output");
			settings.setValue("Family", font.family());
			settings.setValue("PointSize", font.pointSize());
			settings.setValue("Bold", font.bold());
			settings.setValue("Italic", font.italic());
		settings.endGroup();
	settings.endGroup();

	QPlainTextEdit::setFont(font);
}
