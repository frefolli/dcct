#include <dcct/gui.hh>
#include <dcct/logging.hh>
#include <dcct/compression.hh>
#include <QBoxLayout>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QIcon>
#include <QString>
#include <QPixmap>
#include <filesystem>

QString ToQString(std::string str) {
  QString string;
  string.assign(str);
  return string;
}

inline void CraftSlider(QSlider*& slider, QLabel*& label,
                        QBoxLayout* whereToAdd, std::string label_prefix,
                        uint32_t value, uint32_t min_value, uint32_t max_value) {
  slider = new QSlider();
  slider->setValue(value);
  slider->setMinimum(min_value);
  slider->setMaximum(max_value);
  slider->setOrientation(Qt::Orientation::Horizontal);
  label = new QLabel();
  label->setText(ToQString(label_prefix + ": " + std::to_string(value)));
  label->setAlignment(Qt::AlignmentFlag::AlignCenter);
  auto cell = new QVBoxLayout();
  cell->addWidget(label);
  cell->addWidget(slider);
  whereToAdd->addLayout(cell);
}

inline void CraftLineEdit(QLineEdit*& line_edit, QLabel*& label,
                          QBoxLayout* whereToAdd, std::string title,
                          std::string value) {
  line_edit = new QLineEdit();
  line_edit->setText(ToQString(value));
  label = new QLabel();
  label->setText(ToQString(title));
  label->setAlignment(Qt::AlignmentFlag::AlignCenter);
  auto cell = new QVBoxLayout();
  cell->addWidget(label);
  cell->addWidget(line_edit);
  whereToAdd->addLayout(cell);
}

inline void CraftButton(QPushButton*& button, QBoxLayout* whereToAdd, std::string title) {
  button = new QPushButton();
  button->setText(ToQString(title));
  whereToAdd->addWidget(button);
}

inline void CraftProgressbar(QProgressBar*& progressbar, QBoxLayout* whereToAdd) {
  progressbar = new QProgressBar();
  progressbar->hide();
  whereToAdd->addWidget(progressbar);
}

inline void CraftImage(QLabel*& image_label, QBoxLayout* whereToAdd, std::string title) {
  image_label = new QLabel();
  QLabel* title_label = new QLabel();
  title_label->setText(ToQString(title));
  title_label->setAlignment(Qt::AlignmentFlag::AlignCenter);
  auto cell = new QVBoxLayout();
  cell->addWidget(title_label);
  cell->addWidget(image_label);
  whereToAdd->addLayout(cell);
}

void dcct::GUI::buildFloatingWindows() {
  this->open_image_dialog = new QFileDialog();
  this->open_image_dialog->setWindowTitle(ToQString("Choose an Image"));
  this->open_image_dialog->setWindowIcon(QIcon("resources/images/icon.png"));
  this->open_image_dialog->setNameFilter(ToQString("Images (*.bmp *.png *.jpg)"));
  this->alert_dialog = new QMessageBox();
  this->alert_dialog->setWindowIcon(QIcon("resources/images/icon.png"));
}

void dcct::GUI::buildToolbar() {
  auto toolbar = new QHBoxLayout();
  CraftSlider(block_size_slider, block_size_label,
              toolbar, "Block size",
              initial_actuator.blockSize, 1, 100);
  CraftSlider(quality_slider, quality_label,
              toolbar, "Quality",
              initial_actuator.quality, 1, 2 * initial_actuator.blockSize - 2);
  CraftLineEdit(image_line_edit, image_label,
                toolbar, "Input image path",
                initial_filepath);
  CraftButton(open_image_button, toolbar, "choose image");
  CraftButton(compress_image_button, toolbar, "compress");
  this->layout->addLayout(toolbar);
}

void dcct::GUI::buildProgressbar() {
  CraftProgressbar(progressbar, this->layout);
}

void dcct::GUI::buildGraphicalSpace() {
  auto graphical_space = new QHBoxLayout();
  CraftImage(original_image, graphical_space, "Original Image");
  CraftImage(compressed_image, graphical_space, "Compressed Image");
  this->layout->addLayout(graphical_space);
}

void dcct::GUI::addConnections() {
  QObject::connect(this->open_image_button, &QPushButton::clicked, this, [this] {
    this->userWantsToOpenImage();
  });
  QObject::connect(this->compress_image_button, &QPushButton::clicked, this, [this] {
    this->userWantsToCompressImage();
  });
  QObject::connect(this->block_size_slider, &QSlider::valueChanged, this, [this] {
    this->userChangedBlockSize();
  });
  QObject::connect(this->quality_slider, &QSlider::valueChanged, this, [this] {
    this->userChangedQuality();
  });
}

void dcct::GUI::userWantsToOpenImage() {
  if (this->open_image_dialog->exec()) {
    this->image_line_edit->setText(this->open_image_dialog->selectedFiles()[0]);
  }
}

void dcct::GUI::userWantsToCompressImage() {
  std::string input_filepath = this->image_line_edit->text().toStdString();
  std::string output_filepath = std::string(std::filesystem::temp_directory_path()) + "/dcct.output.png";

  dcct::ActuatorSpecifier specifier = {
    .type = dcct::ActuatorSpecifier::Type::FFTW,
    .blockSize = (uint32_t) this->block_size_slider->value(),
    .quality = (uint32_t) this->quality_slider->value()
  };

  auto set_progressbar_length = [this](uint32_t length) {
    this->progressbar->setMaximum(length);
    this->progressbar->setValue(0);
  };
  auto increment_progressbar = [this]() {
    this->progressbar->setValue(this->progressbar->value() + 1);
  };
  this->progressbar->show();
  dcct::CompressionEvent result = dcct::CompressImage(input_filepath,
                                                      output_filepath,
                                                      specifier,
                                                      set_progressbar_length,
                                                      increment_progressbar);
  if (result != dcct::CompressionEvent::OK) {
    dcct::RaiseFatalError(dcct::ToString(result));
  }

  this->original_image->setPixmap(QPixmap(ToQString(input_filepath)).scaledToWidth(512));
  this->compressed_image->setPixmap(QPixmap(ToQString(output_filepath)).scaledToWidth(512));
  this->progressbar->hide();
}

void dcct::GUI::userChangedBlockSize() {
  uint32_t block_size = block_size_slider->value();
  quality_slider->setMaximum(block_size * 2 - 1);
  block_size_label->setText(ToQString("Block size: " + std::to_string(block_size)));
}

void dcct::GUI::userChangedQuality() {
  uint32_t quality = quality_slider->value();
  quality_label->setText(ToQString("Quality: " + std::to_string(quality)));
}

dcct::GUI::GUI(dcct::ActuatorSpecifier& initial_actuator,
               std::string initial_filepath) :
               initial_actuator(initial_actuator),
               initial_filepath(initial_filepath) {
  this->setWindowTitle("DCCT / Qt6");
  this->setWindowIcon(QIcon("resources/images/icon.png"));

  this->layout = new QVBoxLayout();

  this->buildFloatingWindows();
  this->buildToolbar();
  this->buildProgressbar();
  this->buildGraphicalSpace();
  this->addConnections();
  
  this->setLayout(this->layout);
}
