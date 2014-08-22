#include <QStatusBar>

class StatusBar: public QStatusBar {
 Q_OBJECT

 public:
  StatusBar(QWidget *parent = 0);

  void setCursorPosition(int line, int column);
};
