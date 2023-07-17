#include "mainwindow.h"

#include <QDebug>

#include "glview.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow) {
  ui->setupUi(this);
  ui->widget->setUpdatesEnabled(false);
  settings = new QSettings("", "3D Viewer 1.0", this);
  LoadSettings();
  recordTimer = new QTimer(0);
  ui->horizontalSlider_dx->setValue(ui->widget->dx);
  ui->horizontalSlider_dy->setValue(ui->widget->dy);
  ui->horizontalSlider_dz->setValue(ui->widget->dz);
  ui->doubleSpinBox_x->setValue(ui->widget->wx);
  ui->doubleSpinBox_y->setValue(ui->widget->wy);
  ui->doubleSpinBox_z->setValue(ui->widget->wz);
  ui->horizontalSlider_scale->setValue(100);
  ui->horizontalSlider_width->setValue(ui->widget->line_width);
  ui->horizontalSlider_pWidth->setValue(ui->widget->point_width);
  ui->radioButton_ortho->setChecked(ui->widget->projection);
  ui->radioButton_frustum->setChecked(!ui->widget->projection);
  if (ui->widget->line_style == 0xffff)
    ui->radioButton_linetype_1->setChecked(true);
  if (ui->widget->line_style == 0xf000)
    ui->radioButton_linetype_2->setChecked(true);
  if (ui->widget->point_style == 0)
    ui->radioButton_point_none->setChecked(true);
  else if (ui->widget->point_style == 1)
    ui->radioButton_point_circle->setChecked(true);
  else if (ui->widget->point_style == 2)
    ui->radioButton_point_square->setChecked(true);
  connect(ui->Button_Gif, SIGNAL(clicked()), this,
          SLOT(on_Button_Gif_clicked()));
  connect(recordTimer, SIGNAL(timeout()), this, SLOT(recordTimerAlarm()));
}

MainWindow::~MainWindow() {
  SaveSettings();
  delete ui;
  delete recordTimer;
}

void MainWindow::on_Button_file_clicked() {
  line_x = ui->horizontalSlider_scale->value();
  ui->widget->kx = line_x / 500;
  str = QFileDialog::getOpenFileName(this, "Open a file", "/",
                                     "Object file (*.obj)");
  ui->label_name_f->setText(str);
  ui->widget->setStr(str);
  ui->widget->setUpdatesEnabled(true);
  on_Button_rotate_clicked();
  //    ui->widget->update();
}

void MainWindow::on_Button_creator_clicked() {
  if (str.isEmpty() == false) {
    int f = ui->widget->n_facets;
    int v = ui->widget->n_vertexes;
    ui->label_facets->setText(QString::number(f, 'g', 7));
    ui->label_vertexes->setText(QString::number(v, 'g', 7));
  }
}

void MainWindow::on_Button_Color_clicked() {
  QColor color_bgd = QColorDialog::getColor(Qt::white, this, "Выбор цвета");
  if (color_bgd.isValid()) {
    ui->widget->red = color_bgd.redF();
    ui->widget->green = color_bgd.greenF();
    ui->widget->blue = color_bgd.blueF();
  }
  ui->widget->update();
}

void MainWindow::on_horizontalSlider_dx_valueChanged(int value) {
  ui->widget->dx = value;
  ui->widget->update();
}

void MainWindow::on_horizontalSlider_dy_valueChanged(int value) {
  ui->widget->dy = value;
  ui->widget->update();
}

void MainWindow::on_horizontalSlider_dz_valueChanged(int value) {
  ui->widget->dz = value;
  ui->widget->update();
}

void MainWindow::on_horizontalSlider_scale_valueChanged(int value) {
  line_x = (double)value;
  ui->widget->kx = line_x / 500;
  ui->widget->update();
}

void MainWindow::on_Button_rotate_clicked() {
  wx = (ui->doubleSpinBox_x->value());
  wy = (ui->doubleSpinBox_y->value());
  wz = (ui->doubleSpinBox_z->value());
  ui->widget->wx = (wx == 0) ? 360 : wx;
  ui->widget->wy = (wy == 0) ? 360 : wy;
  ui->widget->wz = (wz == 0) ? 360 : wz;
  ui->widget->update();
}

void MainWindow::on_radioButton_ortho_clicked() {
  ui->widget->projection = true;
  ui->widget->update();
}

void MainWindow::on_radioButton_frustum_clicked() {
  ui->widget->projection = false;
  ui->widget->update();
}

void MainWindow::on_Button_Color_line_clicked() {
  QColor color_line = QColorDialog::getColor(Qt::white, this, "Выбор цвета");
  if (color_line.isValid()) {
    ui->widget->line_red = color_line.redF();
    ui->widget->line_green = color_line.greenF();
    ui->widget->line_blue = color_line.blueF();
  }
  ui->widget->update();
}

void MainWindow::on_horizontalSlider_width_valueChanged(int value) {
  ui->widget->line_width = value;
  ui->widget->update();
}

void MainWindow::on_radioButton_linetype_1_clicked() {
  ui->widget->line_style = 0xffff;
  ui->widget->update();
}

void MainWindow::on_radioButton_linetype_2_clicked() {
  ui->widget->line_style = 0xf000;
  ui->widget->update();
}

void MainWindow::on_Button_Color_point_clicked() {
  QColor color_point = QColorDialog::getColor(Qt::white, this, "Выбор цвета");
  if (color_point.isValid()) {
    ui->widget->point_red = color_point.redF();
    ui->widget->point_green = color_point.greenF();
    ui->widget->point_blue = color_point.blueF();
  }
  ui->widget->update();
}

void MainWindow::on_horizontalSlider_pWidth_valueChanged(int value) {
  ui->widget->point_width = value;
  ui->widget->update();
}

void MainWindow::on_radioButton_point_none_clicked() {
  ui->widget->point_style = 0;
  ui->widget->update();
}

void MainWindow::on_radioButton_point_circle_clicked() {
  ui->widget->point_style = 1;
  ui->widget->update();
}

void MainWindow::on_radioButton_point_square_clicked() {
  ui->widget->point_style = 2;
  ui->widget->update();
}

void MainWindow::on_Button_screenshot_clicked() {
  std::string ext;
  std::string filter = "Image file (*" + ext + ")";
  QString filename;
  if (ui->comboBox_screenshot->currentIndex() == 0) {
    filename = QFileDialog::getSaveFileName(this, "Save as...", "name.png",
                                            "PNG (*.png)");
  }
  if (ui->comboBox_screenshot->currentIndex() == 1) {
    filename = QFileDialog::getSaveFileName(this, "Save as...", "name.bmp",
                                            "BMP (*.bmp)");
  }
  if (ui->comboBox_screenshot->currentIndex() == 2) {
    filename = QFileDialog::getSaveFileName(this, "Save as...", "name.jpg",
                                            "JPG (*.jpg)");
  }

  if (!filename.isEmpty()) ui->widget->grab().save(filename);
}

void MainWindow::on_Button_Gif_clicked() {
  qDebug() << "rdy!";
  ui->Button_Gif->setText("Запись");
  recordTimer->start(100);

  recordTimerAlarm();
  cout = 0.0;
}

void MainWindow::recordTimerAlarm() {
  if (cout <= 5.0) {
    mas_image.push_back(ui->widget->grab().toImage());
    cout += 0.1;
  } else {
    MAKEgif();
    recordTimer->stop();
    ui->Button_Gif->setText("Записать Gif");
  }
}

void MainWindow::MAKEgif() {
  QString gif_name =
      QFileDialog::getSaveFileName(this, "Save gif", "", "*.gif");
  QGifImage gif_image(QSize(640, 480));
  gif_image.setDefaultDelay(100);

  for (QVector<QImage>::Iterator screen = mas_image.begin();
       screen != mas_image.end(); ++screen) {
    gif_image.addFrame(*screen);
  }
  gif_image.save(gif_name);
  mas_image.clear();
  cout = 0.0;
}

void MainWindow::SaveSettings() {
  settings->setValue("projection", ui->widget->projection);
  settings->setValue("line_style", ui->widget->line_style);
  settings->setValue("line_width", ui->widget->line_width);
  settings->setValue("point_style", ui->widget->point_style);
  settings->setValue("point_width", ui->widget->point_width);
  settings->setValue("background_color_red", ui->widget->red);
  settings->setValue("background_color_green", ui->widget->green);
  settings->setValue("background_color_blue", ui->widget->blue);
  settings->setValue("line_color_red", ui->widget->line_red);
  settings->setValue("line_color_green", ui->widget->line_green);
  settings->setValue("line_color_blue", ui->widget->line_blue);
  settings->setValue("point_color_red", ui->widget->point_red);
  settings->setValue("point_color_green", ui->widget->point_green);
  settings->setValue("point_color_blue", ui->widget->point_blue);
}

void MainWindow::LoadSettings() {
  ui->widget->projection = settings->value("projection").toBool();
  ui->widget->line_style = settings->value("line_style").toUInt();
  ui->widget->line_width = settings->value("line_width").toInt();
  ui->widget->point_style = settings->value("point_style").toInt();
  ui->widget->point_width = settings->value("point_width").toInt();
  ui->widget->red = settings->value("background_color_red").toFloat();
  ui->widget->green = settings->value("background_color_green").toFloat();
  ui->widget->blue = settings->value("background_color_blue").toFloat();
  ui->widget->line_red = settings->value("line_color_red").toFloat();
  ui->widget->line_green = settings->value("line_color_green").toFloat();
  ui->widget->line_blue = settings->value("line_color_blue").toFloat();
  ui->widget->point_red = settings->value("point_color_red").toFloat();
  ui->widget->point_green = settings->value("point_color_green").toFloat();
  ui->widget->point_blue = settings->value("point_color_blue").toFloat();
}
