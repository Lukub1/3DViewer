#ifndef GLVIEW_H
#define GLVIEW_H

#include <QFile>
#include <QFileDialog>
#include <QMessageBox>
#include <QOpenGLFunctions>
#include <QOpenGLWidget>
#include <QtOpenGL>

extern "C" {
#include "../parse.h"
}

class GLView : public QOpenGLWidget, protected QOpenGLFunctions {
  Q_OBJECT

 public:
  GLView(QWidget* parent = nullptr);
  void setStr(const QString& s);
  double dx = 0, dy = 0, dz = 0, wx = 0, wy = 0, wz = 0, kx = 0;
  int n_vertexes, n_facets;
  int line_width = 1, point_width = 1, point_style = 0;
  float red = 0, green = 0, blue = 0;
  float line_red = 1, line_green = 1, line_blue = 1;
  float point_red = 1, point_green = 1, point_blue = 1;
  bool projection = true;
  unsigned int line_style = 0xffff;
  void screenshot(QString filename);

 protected:
  void initializeGL();
  void resizeGL(int w, int h);
  void paintGL();

 private:
  QString str;
};

#endif  // GLVIEW_H
