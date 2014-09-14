#include "cglwidget.h"
#include "cparallelepiped.h"
#include <cmath>
#include <QMouseEvent>
#include <QWheelEvent>
#include <limits>

CGLWidget::CGLWidget(QWidget *parent, const QGLWidget* shareWidget, Qt::WindowFlags f) :
    QGLWidget(parent, shareWidget, f)
{
    menu = new QMenu(this);
    QMenu* rotate(new QMenu(menu));
    rotate->setTitle("Выровнять относительносительно");
    sidesMenu << new QAction("Плоскости XY", rotate)
              << new QAction("Плоскости XZ", rotate)
              << new QAction("Плоскости YZ", rotate);
    connect(rotate, SIGNAL(hovered(QAction*)), SLOT(menuHandling(QAction *)));
    rotate->addActions(sidesMenu);
    menu->addMenu(rotate);

    transformIdentity();
}

void CGLWidget::transformIdentity()
{
    rotateMatrix.setToIdentity();

    rotate.first = rotate.second = translate.first = translate.second = 0.0f;

    zoom = 0.5f;

    safelyUpdate();
}

void CGLWidget::initializeGL() {
    glEnable (GL_BLEND);
    //glBlendFunc (GL_SRC_ALPHA, GL_ONE_MaxUS_SRC_ALPHA);
    glEnable(GL_DEPTH_TEST);
    //glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_TRUE);
    //glShadeModel(GL_FLAT);
    glPointSize(12);

    //glDepthFunc(GL_LEQUAL);

    glLineWidth(2);
    initializeCGL();
}
void CGLWidget::resizeGL(int width, int height) {
    edge = width < height ? width : height;
    glPointSize(edge / 100 + 3);
    resizeCGL(width, height);
}

void CGLWidget::drawGradienQuad(const QRgb c1,const QRgb c2,const QRgb c3,const QRgb c4) {
    glViewport(0,0,this->width(), this->height());
    glBegin(GL_QUADS);
        const float z(1.0f - std::numeric_limits<float>::epsilon());
        glColor4ubv(static_cast<const unsigned char*>(static_cast<const void*>(&c1)));
        glVertex3f(-1.0f, 1.0f, z);
        glColor4ubv(static_cast<const unsigned char*>(static_cast<const void*>(&c2)));
        glVertex3f( 1.0f, 1.0f, z);
        glColor4ubv(static_cast<const unsigned char*>(static_cast<const void*>(&c3)));
        glVertex3f( 1.0f,-1.0f, z);
        glColor4ubv(static_cast<const unsigned char*>(static_cast<const void*>(&c4)));
        glVertex3f(-1.0f,-1.0f, z);
    glEnd();
}
void CGLWidget::clear()
{
    const QBrush& b(this->palette().background());
    glClearColor(b.color().redF(),b.color().greenF(),b.color().blueF(), 1);
    glClear(GL_COLOR_BUFFER_BIT);
    glClear(GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    drawIdantityQuad();
}

void CGLWidget::loadRotate() {
    glMatrixMode(GL_MODELVIEW);
    glRotatef(rotate.first,1.,0.,0.);
    glRotatef(rotate.second,0.,1.,0.);
#if QT_VERSION >= 0x050000
    glMultMatrixf(rotateMatrix.data());
#else
    glMultMatrixd(rotateMatrix.data());
#endif
}

void CGLWidget::paintGL()
{
    clear();

    GLfloat r (sceneSize.size() * zoom);

    glMatrixMode(GL_MODELVIEW);
    glTranslatef(translate.first * 2 * r,translate.second * 2 * r,0);



    glMatrixMode(GL_PROJECTION);
    if (this->width() > this->height()) {
        float k(static_cast<float>(this->width())/this->height());
        glOrtho(-r * k, r * k, -r, r, r * 50.0f, -r * 50.0f);
    } else {
        float k(static_cast<float>(this->height())/this->width());
        glOrtho(-r, r, -r * k, r * k, r * 50.0f, -r * 50.0f);
    }

    loadRotate();

    const QVector3D center(sceneSize.center());
    glTranslatef(-center.x(), -center.y(), -center.z());


    glViewport( 0, 0, this->width(), this->height());

    glColor3ub(0x00,0x00,0x00);
    paintCGL();
}

void CGLWidget::debugSpace(const CParallelepiped& s) {
    glBegin(GL_LINES); {
        glColor3ub(0,0,255);
        glVertex3d(s.xMin(), s.yMin(), s.zMin());
        glVertex3d(s.xMax(), s.yMin(), s.zMin());
        glVertex3d(s.xMin(), s.yMin(), s.zMin());
        glVertex3d(s.xMin(), s.yMax(), s.zMin());
        glVertex3d(s.xMin(), s.yMin(), s.zMin());
        glVertex3d(s.xMin(), s.yMin(), s.zMax());

        glColor3ub(255,255,0);
        glVertex3d(s.xMax(), s.yMin(), s.zMin());
        glVertex3d(s.xMax(), s.yMax(), s.zMin());
        glVertex3d(s.xMax(), s.yMin(), s.zMax());
        glVertex3d(s.xMax(), s.yMin(), s.zMin());

        glColor3ub(0,255,255);
        glVertex3d(s.xMin(), s.yMax(), s.zMin());
        glVertex3d(s.xMax(), s.yMax(), s.zMin());
        glVertex3d(s.xMin(), s.yMax(), s.zMin());
        glVertex3d(s.xMin(), s.yMax(), s.zMax());

        glColor3ub(255,0,255);
        glVertex3d(s.xMin(), s.yMin(), s.zMax());
        glVertex3d(s.xMax(), s.yMin(), s.zMax());
        glVertex3d(s.xMin(), s.yMin(), s.zMax());
        glVertex3d(s.xMin(), s.yMax(), s.zMax());

        glColor3ub(255,0,0);
        glVertex3d(s.xMax(), s.yMax(), s.zMax());
        glVertex3d(s.xMin(), s.yMax(), s.zMax());
        glVertex3d(s.xMax(), s.yMax(), s.zMax());
        glVertex3d(s.xMax(), s.yMin(), s.zMax());
        glVertex3d(s.xMax(), s.yMax(), s.zMax());
        glVertex3d(s.xMax(), s.yMax(), s.zMin());
    } glEnd();
    renderText(s.xMin(), s.yMin(), s.zMin(), "{" + QString::number(s.xMin()) + ", " + QString::number(s.yMin()) + ", " + QString::number(s.zMin()) + "}");
    renderText(s.xMax(), s.yMax(), s.zMax(), "{" + QString::number(s.xMax()) + ", " + QString::number(s.yMax()) + ", " + QString::number(s.zMax()) + "}");
}

void CGLWidget::mousePressEvent(QMouseEvent* e)
{
    if (e->buttons() & Qt::RightButton) {
        menu->move(e->globalPos());
        menu->show();
    } else if (e->buttons() & Qt::LeftButton) {
        mPPosition = e->pos();
    }
}

void CGLWidget::mouseMoveEvent(QMouseEvent* e)
{
    if (e->buttons() & Qt::LeftButton) {
        if (!(e->modifiers() & Qt::ALT)) {
            rotate.first -= 180.0f * static_cast<GLfloat>(e->y() - mPPosition.y()) / edge;
        }
        if (!(e->modifiers() & Qt::CTRL)) {
            rotate.second -= 180.0f * static_cast<GLfloat>(e->x() - mPPosition.x()) / edge;
        }

        safelyUpdate();

        mPPosition = e->pos();
    } else if (e->buttons() & Qt::MidButton) {
        if (!(e->modifiers() & Qt::ALT)) {
            translate.second -= static_cast<GLfloat>(e->y() - mPPosition.y()) / edge;
        }
        if (!(e->modifiers() & Qt::CTRL)) {
            translate.first += static_cast<GLfloat>(e->x() - mPPosition.x()) / edge;
        }

        safelyUpdate();

        mPPosition = e->pos();
    }
}

void CGLWidget::menuHandling(QAction *a) {
    rotateBuf = rotateMatrix;
    rotateMatrix.setToIdentity();
    int i(0);
    while (sidesMenu.size() < i && sidesMenu.at(i) != a) ++i;

    rotate.first = rotate.second = 0.0f;
    switch (i) {
    case 1:
        rotateMatrix.rotate(0,0,0,0);
        break;
    case 2:
        rotate.first = M_PI / 2;
        break;
    case 3:
        rotate.second = M_PI / 2;
        break;
    case 4:
        rotate.first = M_PI / 2;
        rotate.second = M_PI / 2;
        break;
    default:
        rotateMatrix = rotateBuf;
    }
    //loadRotate();
    //glGetFloatv(GL_MODELVIEW_MATRIX, rotateMatrix.data());
    //rotate.first = rotate.second = 0.0f;
    qDebug() << rotateMatrix;
    repaint();
    qDebug() << "Hangling";
}
void CGLWidget::menuCatcher() {
    rotateBuf = rotateMatrix;
    qDebug() << "set";
}

void CGLWidget::wheelEvent(QWheelEvent* e) {
    if (e->delta() > 0) {
        zoom *= 0.9f;
    } else {
        zoom /= 0.9f;
    }
    safelyUpdate();
}

void CGLWidget::mouseReleaseEvent(QMouseEvent*)
{
    if (rotate.first && rotate.second) {
        this->makeCurrent();
        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();
        loadRotate();
#if QT_VERSION >= 0x050000
        glGetFloatv(GL_MODELVIEW_MATRIX, rotateMatrix.data());
#else
        glGetDoublev(GL_MODELVIEW_MATRIX, rotateMatrix.data());
#endif
        rotate.first = rotate.second = 0.0f;
        safelyUpdate();
    }
}
