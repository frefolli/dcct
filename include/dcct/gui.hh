#ifndef DCCT_GUI_HH
#define DCCT_GUI_HH
#include <QMainWindow>
#include <QLabel>
#include <QPushButton>

namespace dcct {
  class Tester : public QMainWindow {
      Q_OBJECT

      int32_t mButtonPressedAmt;

      QLabel* mpLabel;
      QPushButton* mpPushButton;

      void updateLabelText();

  public:
      Tester(QWidget* parent = nullptr);
      ~Tester() = default;

  signals:
      void sigLabelTextUpdated(std::string_view);
  };
}
#endif//DCCT_GUI_HH