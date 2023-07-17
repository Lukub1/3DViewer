#include "glview.h"

#include <QDebug>
#define GL_SILENCE_DEPRECATION

GLView::GLView(QWidget* parent) : QOpenGLWidget(parent) {}

void GLView::setStr(const QString& s) { str = s; }

void GLView::initializeGL() {
  initializeOpenGLFunctions();
  glViewport(0, 0, width(), height());
}

void GLView::resizeGL(int w, int h) { glViewport(0, 0, w, h); }

void GLView::paintGL() {
  glClearColor(red, green, blue, 1.0);
  glClear(GL_COLOR_BUFFER_BIT);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  if (projection) {
    glOrtho(-15, 15, -20, 20, -100, 100);
  } else {
    glFrustum(-1.5, 1.5, -2.0, 2.0, 3, 50);
    glTranslatef(0.0, 0.0, -3.0);
  }

  // преобразование в с-строку:
  qDebug() << str;
  QByteArray buf = str.toLocal8Bit();
  const char* f_name = buf.data();

  // запись в структуру
  data_t data;
  int error = readFile(f_name, &data);

  callocStruct(&data);
  scanFile(f_name, &data);
  n_vertexes = data.numVertex;
  n_facets = data.numEdges;
  qDebug() << n_vertexes << n_facets;
  if (!error) {
    data_t output;
    dup(&data, &output);
    if (wx) rotation(&output, &output, Ax, wx);
    if (wy) rotation(&output, &output, Ay, wy);
    if (wz) rotation(&output, &output, Az, wz + 180);
    shifting(&output, dx, dy, dz);
    qDebug() << "dx =" << dx << "dy =" << dy << "dz =" << dz;

    // перебирает грани в структуре data
    int facet = 0;
    int wrong_vertex_flag = 0;
    glScaled(kx, kx, kx);
    glColor3d(line_red, line_green, line_blue);
    glLineWidth(line_width);
    glLineStipple(1, line_style);
    glVertexPointer(3, GL_FLOAT, 0, data.vertexes);
    glEnable(GL_LINE_STIPPLE);
    for (int j = 0; j < data.allSides; j++) {
      // проверяет номера вершин на валидность
      for (int i = 0; i < output.numSides[j]; i++) {
        if (output.edges[j][i] > output.numVertex) wrong_vertex_flag = 1;
      }
      if (wrong_vertex_flag) {
        wrong_vertex_flag = 0;
        continue;
      }
      glBegin(GL_LINE_LOOP);
      // отрисовывает линии, из которых состоит грань
      for (int i = 0; i < output.numSides[j] - 1; i++) {
        facet = output.edges[j][i];
        glVertex3f(output.vertexes[facet - 1][0], output.vertexes[facet - 1][1],
                   output.vertexes[facet - 1][2]);
        facet = output.edges[j][i + 1];
        glVertex3f(output.vertexes[facet - 1][0], output.vertexes[facet - 1][1],
                   output.vertexes[facet - 1][2]);
      }
      // отрисовывает последнюю линию
      facet = output.edges[j][output.numSides[j] - 1];
      glVertex3f(output.vertexes[facet - 1][0], output.vertexes[facet - 1][1],
                 output.vertexes[facet - 1][2]);
      facet = output.edges[j][0];
      glVertex3f(output.vertexes[facet - 1][0], output.vertexes[facet - 1][1],
                 output.vertexes[facet - 1][2]);
      glEnd();
    }
    glDisable(GL_LINE_STIPPLE);
    glColor3d(point_red, point_green, point_blue);
    glPointSize(point_width);
    if (point_style) {
      if (point_style == 1) glEnable(GL_POINT_SMOOTH);
      glBegin(GL_POINTS);
      for (int j = 1; j <= data.numVertex - 1; j++) {
        glVertex3d(output.vertexes[j][0], output.vertexes[j][1],
                   output.vertexes[j][2]);
      }
      glEnd();
      if (point_style == 1) glDisable(GL_POINT_SMOOTH);
    }
    structFree(&output);
  }
  structFree(&data);
}

void GLView::screenshot(QString filename) {
  this->grabFramebuffer().save(filename);
}
