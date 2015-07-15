/**
 * @file 3dmouseinterface.h
 * @brief A mouse interface for qt3d scene3d
 * @author Lorenzo Lucignano
 * @version 1.0
 * @date 2015-07-02
 */



#ifndef MOUSEINTERFACE3D_H
#define MOUSEINTERFACE3D_H

#include <QQuickItem>
#include <Qt3DCore>
#include <Qt3DRenderer/QMesh>
#include <Qt3DCore/QComponentList>
#include <Qt3DCore/QCamera>
#include <QQmlListProperty>

class MouseInterface3D : public QQuickItem
{
    Q_OBJECT
    Q_DISABLE_COPY(MouseInterface3D)
    Q_PROPERTY(Qt3D::QEntity* scene3D WRITE setScene3D)
    Q_PROPERTY(Qt3D::QCamera* camera WRITE setCamera)
public:
    MouseInterface3D(QQuickItem *parent = 0);
    ~MouseInterface3D();
    void setScene3D(Qt3D::QEntity* scene3D);
    void setCamera(Qt3D::QCamera* camera);


public slots:
    void select(QVector2D mouseXYNormalized);
signals:
    void selectedItem(Qt3D::QEntity* item);

private:
    bool checkIntersectionRay_AABB(const Qt3D::QRay3D ray, const Qt3D::QAxisAlignedBoundingBox aabb);
    bool checkIntersectionRay_Triangle(const Qt3D::QRay3D ray, const QVector<QVector3D> triangle,qreal &tnear);
    Qt3D::QEntity* select_recursive_step(  Qt3D::QEntity* node ,const QMatrix4x4 parents_matrix, qreal &hitEntity_tnear, const Qt3D::QRay3D ray);
    Qt3D::QEntity* m_sceneroot;
    Qt3D::QCamera* m_camera;


};

#endif // MOUSEINTERFACE3D_H
