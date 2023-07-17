#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QColorDialog>
#include <QFileDialog>
#include <QGridLayout>
#include <QImage>
#include <QMainWindow>
#include <QMessageBox>
#include <QPainter>
#include <QPixmap>
#include <QSettings>
#include <QSize>
#include <QTimer>
#include <QValidator>
#include <QVector>

#include "../QtGifImage-master/src/gifimage/qgifimage.h"

extern "C" {
#include "../parse.h"
}

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
  Q_OBJECT

 public:
  MainWindow(QWidget *parent = nullptr);
  ~MainWindow();
  QString str;
  double dx, dy, dz, wx, wy, wz, line_x;
  int v, f;
  float cout = 0;
  QTimer *recordTimer;
  QVector<QImage> mas_image;

 private slots:
  void on_Button_file_clicked();
  void on_Button_creator_clicked();
  void on_Button_Color_clicked();
  void on_horizontalSlider_dx_valueChanged(int value);
  void on_horizontalSlider_dy_valueChanged(int value);
  void on_horizontalSlider_dz_valueChanged(int value);
  void on_horizontalSlider_scale_valueChanged(int value);
  void on_radioButton_ortho_clicked();
  void on_radioButton_frustum_clicked();
  void on_Button_Color_line_clicked();
  void on_horizontalSlider_width_valueChanged(int value);
  void on_radioButton_linetype_1_clicked();
  void on_radioButton_linetype_2_clicked();
  void on_Button_Color_point_clicked();
  void on_horizontalSlider_pWidth_valueChanged(int value);
  void on_radioButton_point_none_clicked();
  void on_radioButton_point_circle_clicked();
  void on_radioButton_point_square_clicked();
  void on_Button_screenshot_clicked();
  void on_Button_Gif_clicked();
  void recordTimerAlarm();
  void MAKEgif();
  void on_Button_rotate_clicked();
  void SaveSettings();
  void LoadSettings();

 private:
  Ui::MainWindow *ui;

  QSettings *settings = NULL;
  /*
      int frames;
      QString gifFileName;
      QVector<QImage> *img_buf;
      GifAnim *ganim;
      GifWriter *gwriter;*/
  bool record = false;
};
#endif  // MAINWINDOW_H
