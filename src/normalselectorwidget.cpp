#include "src/normalselectorwidget.h"
#include <math.h>
#include <QOpenGLFramebufferObject>
#include <QOpenGLVersionProfile>
#include <QOpenGLVertexArrayObject>
#include <QOpenGLFunctions>
#include <QMouseEvent>
#include <QThread>


#define PI 3.14159265359


void NormalSelectorWidget::renderSphere()
{
  sphereVAO.bind();
  glDrawElements(GL_TRIANGLE_STRIP, indexCount, GL_UNSIGNED_INT, 0);
}

NormalSelectorWidget::NormalSelectorWidget(QWidget *parent)
{

}

void NormalSelectorWidget::initializeGL(){
  initializeOpenGLFunctions();
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  glEnable(GL_BLEND);
  glClearColor(0.5, 0.5, 1.0, 1.0);

  glEnable(GL_DEPTH_TEST);

  setUpdateBehavior(QOpenGLWidget::PartialUpdate);


  sphere.link();
  sphere.addShaderFromSourceFile(QOpenGLShader::Vertex,
                                 ":/shaders/shaders/sphere_vertex.glsl");
  sphere.addShaderFromSourceFile(QOpenGLShader::Fragment,
                                 ":/shaders/shaders/sphere_fragment.glsl");

  /* Prepare sphere */

  plane.link();
  plane.addShaderFromSourceFile(QOpenGLShader::Vertex,
                                ":/shaders/shaders/plane_vertex.glsl");
  plane.addShaderFromSourceFile(QOpenGLShader::Fragment,
                                ":/shaders/shaders/plane_fragment.glsl");
  if (!sphereVAO.isCreated())
  {
    sphereVAO.create();

    unsigned int vbo, ebo;
    glGenBuffers(1, &vbo);
    glGenBuffers(1, &ebo);

    QVector<QVector3D> positions;
    QVector<QVector2D> uv;
    QVector<QVector3D> normals;
    std::vector<unsigned int> indices;

    const unsigned int X_SEGMENTS = 64;
    const unsigned int Y_SEGMENTS = 64;
    for (unsigned int y = 0; y <= Y_SEGMENTS; ++y)
    {
      for (unsigned int x = 0; x <= X_SEGMENTS; ++x)
      {
        float xSegment = (float)x / (float)X_SEGMENTS;
        float ySegment = (float)y / (float)Y_SEGMENTS;
        float xPos = cos(xSegment * 2.0f * PI) * sin(ySegment * PI);
        float yPos = cos(ySegment * PI);
        float zPos = sin(xSegment * 2.0f * PI) * sin(ySegment * PI);

        positions.push_back(QVector3D(xPos, yPos, zPos));
        uv.push_back(QVector2D(xSegment, ySegment));
        normals.push_back(QVector3D(xPos, yPos, zPos));
      }
    }

    bool oddRow = false;
    for (int y = 0; y < Y_SEGMENTS; ++y)
    {
      if (!oddRow) // even rows: y == 0, y == 2; and so on
      {
        for (int x = 0; x <= X_SEGMENTS; ++x)
        {
          indices.push_back(y       * (X_SEGMENTS + 1) + x);
          indices.push_back((y + 1) * (X_SEGMENTS + 1) + x);
        }
      }
      else
      {
        for (int x = X_SEGMENTS; x >= 0; --x)
        {
          indices.push_back((y + 1) * (X_SEGMENTS + 1) + x);
          indices.push_back(y       * (X_SEGMENTS + 1) + x);
        }
      }
      oddRow = !oddRow;
    }
    indexCount = indices.size();

    std::vector<float> data;
    for (int i = 0; i < positions.size(); ++i)
    {
      data.push_back(positions[i].x());
      data.push_back(positions[i].y());
      data.push_back(positions[i].z());
      if (uv.size() > 0)
      {
        data.push_back(uv[i].x());
        data.push_back(uv[i].y());
      }
      if (normals.size() > 0)
      {
        data.push_back(normals[i].x());
        data.push_back(normals[i].y());
        data.push_back(normals[i].z());
      }
    }

    sphereVAO.bind();
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, data.size() * sizeof(float), &data[0], GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);
    float stride = (3 + 2 + 3) * sizeof(float);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride, (void*)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, stride, (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, stride, (void*)(5 * sizeof(float)));
  }

  /* Prepare Plante */

  planeVAO.create();
  plane.link();

  float vertices[] = {
    -1.0f, -1.0f, 0.0f, 0.0f, 1.0f, // bot left
    1.0f,  -1.0f, 0.0f, 1.0f, 1.0f, // bot right
    1.0f,  1.0f,  0.0f, 1.0f, 0.0f, // top right
    -1.0f, 1.0f,  0.0f, 0.0f, 0.0f  // top left
  };

  QOpenGLVertexArrayObject VAO;
  QOpenGLBuffer VBO;

  planeVAO.bind();
  VBO.create();
  VBO.bind();

  VBO.allocate(vertices, sizeof(vertices));

  int vertexLocation = plane.attributeLocation("aPos");
  glVertexAttribPointer(static_cast<GLuint>(vertexLocation), 3, GL_FLOAT,
                        GL_FALSE, 5 * sizeof(float), nullptr);
  glEnableVertexAttribArray(static_cast<GLuint>(vertexLocation));

  int texCoordLocation = plane.attributeLocation("aTexCoord");
  glVertexAttribPointer(static_cast<GLuint>(texCoordLocation), 2, GL_FLOAT,
                        GL_FALSE, 5 * sizeof(float),
                        (void *)(3 * sizeof(float)));
  glEnableVertexAttribArray(static_cast<GLuint>(texCoordLocation));

  planeVAO.release();
  VBO.release();

}

void NormalSelectorWidget::paintGL(){
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glClearColor(0.5, 0.5, 1.0, 1.0);

  glEnable(GL_DEPTH_TEST);


  qreal aspect = qreal(width()) / qreal(height()? height() : 1);

  QMatrix4x4 projection;
  projection.setToIdentity();
  projection.perspective(45, aspect, -1, 1);

  QMatrix4x4 view;
  view.setToIdentity();
  view.translate(0,0,-2.8);

  QMatrix4x4 transform;
  transform.setToIdentity();

  sphere.bind();
  sphere.setUniformValue("transform", projection*view*transform);
  renderSphere();

  rendered = grabFramebuffer();

  glDisable(GL_DEPTH_TEST);
  planeVAO.bind();
  plane.bind();

  QVector3D normal(2*c.redF()-1,2*c.greenF()-1,c.blueF());

  QVector3D pos = view.inverted()*projection.inverted()*QVector3D(p.x(),p.y(),0.8);

  transform.translate(pos);

  QVector3D rotation_axis = normal.crossProduct(QVector3D(0,0,1),normal);
  float rotation_angle = acos(normal.dotProduct(QVector3D(0,0,1),normal));

  transform.rotate(rotation_angle*180/PI,rotation_axis);

  transform.scale(0.1);
  plane.setUniformValue("transform",projection*view*transform);
  plane.setUniformValue("color",c);
  glDrawArrays(GL_QUADS, 0, 4);

  QVector2D n(2*c.redF()-1,2*c.greenF()-1);

  if (normal.z() != 1){
    glBegin(GL_LINES);
    glVertex3f(0,0,0);
    glVertex3f(0,0,5);
    glEnd();
  }

  glBegin(GL_LINE_LOOP);
  for(int i =0; i <= 300; i++){
    double angle = 2 * PI * i / 300;
    double x = cos(angle)*0.2;
    double y = sin(angle)*0.2;
    glVertex2d(x,y);
  }
  glEnd();
}

void NormalSelectorWidget::resizeGL(int w, int h) {
  update();
}

void NormalSelectorWidget::mousePressEvent(QMouseEvent *event) {
  p = QVector2D(event->localPos().x()/width()*2-1,-event->localPos().y()/height()*2+1);
  c = rendered.pixelColor(event->localPos().x(),event->localPos().y());
  update();
  this->normal_changed(c);
}

void NormalSelectorWidget::mouseMoveEvent(QMouseEvent *event) {
  p = QVector2D(event->localPos().x()/width()*2-1,-event->localPos().y()/height()*2+1);
  if (p.x()<=-1 || p.x() >= 1 || p.y() <= -1 || p.y() >= 1) return;
  c = rendered.pixelColor(event->localPos().x(),event->localPos().y());
  update();
  this->normal_changed(c);
}

QColor NormalSelectorWidget::get_normal(){
  return c;
}
