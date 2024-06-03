#ifndef DCCT_GUI_HH
#define DCCT_GUI_HH
#include <QWidget>
#include <QLabel>
#include <QSlider>
#include <QLineEdit>
#include <QPushButton>
#include <QProgressBar>
#include <QFileDialog>
#include <QMessageBox>
#include <QBoxLayout>
#include <dcct/specifiers.hh>

namespace dcct {
  class GUI : public QWidget {
    Q_OBJECT

    public:
      GUI(dcct::ActuatorSpecifier& initial_actuator,
          std::string initial_filepath);
      ~GUI() = default;

    private:
      void buildFloatingWindows();
      void buildToolbar();
      void buildProgressbar();
      void buildGraphicalSpace();
      void addConnections();

      dcct::ActuatorSpecifier& initial_actuator;
      std::string initial_filepath;

      QBoxLayout* layout = nullptr;
      QFileDialog* open_image_dialog = nullptr;
      QMessageBox* alert_dialog = nullptr;
      QProgressBar* progressbar = nullptr;

      QSlider* block_size_slider = nullptr;
      QLabel* block_size_label = nullptr;
      QSlider* quality_slider = nullptr;
      QLabel* quality_label = nullptr;
      QLineEdit* image_line_edit = nullptr;
      QLabel* image_label = nullptr;
      QPushButton* open_image_button = nullptr;
      QPushButton* compress_image_button = nullptr;
      QLabel* original_image = nullptr;
      QLabel* compressed_image = nullptr;

      void userWantsToOpenImage();
      void userWantsToCompressImage();
      void userChangedBlockSize();
      void userChangedQuality();
    signals:
  };
}
#endif//DCCT_GUI_HH
