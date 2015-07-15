/**
 * @file 3dmouseinterface.cpp
 * @brief A mouse interface for qt3d scene3d
 * @author Lorenzo Lucignano
 * @version 1.0
 * @date 2015-07-02
 */

#include "3dmouseinterface.h"


/*The class assume that the property is scene3D is a Scene3D element holding inside the root entity of the 3D scene graph.
* The rendered objects are assumed to be ONLY childen of those entities having a QMesh component, or children of such objects.
*/

bool pair_comparator(QPair<Qt3D::QEntity*,qreal> x, QPair<Qt3D::QEntity*,qreal> y){return (x.second<y.second);}

/*Necessary due to a bug in QAxisAlignedBoundingBox transform:
* https://bugreports.qt.io/browse/QTBUG-46998*/
QPair<QVector3D,QVector3D> transform_aabb(Qt3D::QAxisAlignedBoundingBox aabb,QMatrix4x4 m){
    QVector3D min=aabb.minPoint();
    QVector3D max=aabb.maxPoint();
    QVector3D m0(m.column(0));
    QVector3D m1(m.column(1));
    QVector3D m2(m.column(2));

    QVector3D xa = m0 * min.x();
    QVector3D xb = m0 * max.x();

    QVector3D ya = m1 * min.y();
    QVector3D yb = m1 * max.y();

    QVector3D za = m2 * min.z();
    QVector3D zb = m2 * max.z();


    QVector3D min_c1,min_c2,min_c3;
    QVector3D max_c1,max_c2,max_c3;


    if(xa.x()<xb.x()){
        min_c1.setX(xa.x());
        max_c1.setX(xb.x());
    }
    else{
        max_c1.setX(xa.x());
        min_c1.setX(xb.x());
    }

    if(xa.y()<xb.y()){
        min_c1.setY(xa.y());
        max_c1.setY(xb.y());
    }
    else{
        max_c1.setY(xa.y());
        min_c1.setY(xb.y());
    }

    if(xa.z()<xb.z()){
        min_c1.setZ(xa.z());
        max_c1.setZ(xb.z());
    }
    else{
        max_c1.setZ(xa.z());
        min_c1.setZ(xb.z());    //QSet<Qt3D::QEntity*> m_selectedItems;

    }

    if(ya.x()<yb.x()){
        min_c2.setX(ya.x());
        max_c2.setX(yb.x());
    }
    else{
        max_c2.setX(ya.x());
        min_c2.setX(yb.x());
    }

    if(ya.y()<yb.y()){
        min_c2.setY(ya.y());
        max_c2.setY(yb.y());
    }
    else{
        max_c2.setY(ya.y());
        min_c2.setY(yb.y());
    }

    if(ya.z()<yb.z()){
        min_c2.setZ(ya.z());
        max_c2.setZ(yb.z());
    }
    else{
        max_c2.setZ(ya.z());
        min_c2.setZ(yb.z());
    }


    if(za.x()<zb.x()){
        min_c3.setX(za.x());
        max_c3.setX(zb.x());
    }
    else{
        max_c3.setX(za.x());
        min_c3.setX(zb.x());
    }

    if(za.y()<zb.y()){
        min_c3.setY(za.y());
        max_c3.setY(zb.y());
    }
    else{
        max_c3.setY(za.y());
        min_c3.setY(zb.y());
    }

    if(za.z()<zb.z()){
        min_c3.setZ(za.z());
        max_c3.setZ(zb.z());
    }
    else{
        max_c3.setZ(za.z());
        min_c3.setZ(zb.z());
    }


    QVector3D t(m.column(3));
    QVector3D newMin=min_c1+min_c2+min_c3+t;
    QVector3D newMax=max_c1+max_c2+max_c3+t;

    QPair<QVector3D,QVector3D> new_aabb(newMin,newMax);
    return new_aabb;
}

MouseInterface3D::MouseInterface3D(QQuickItem *parent):
    QQuickItem(parent)
{

}

MouseInterface3D::~MouseInterface3D()
{
}

void MouseInterface3D::setScene3D(Qt3D::QEntity* scene3D){
    if(scene3D){
        m_sceneroot=scene3D;
    }
}
void MouseInterface3D::setCamera(Qt3D::QCamera *camera){
    if(camera){
        m_camera=camera;
    }
}
/*QQmlListProperty<Qt3D::QEntity> MouseInterface3D::selectedItems(){
    return QQmlListProperty<Qt3D::QEntity>(this,m_selectedItems);
}*/


void MouseInterface3D::select(QVector2D mouseXYNormalized){
    if(!m_sceneroot) return;
    if(!m_camera) return;

    QVector4D ray_clip(mouseXYNormalized.x(),mouseXYNormalized.y(),-1,1);
    QVector4D ray_eye = m_camera->projectionMatrix().inverted().map(ray_clip);
    ray_eye.setZ(-1);ray_eye.setW(0);
    QVector4D ray_wor_4D = m_camera->matrix().inverted().map(ray_eye);
    QVector3D ray_wor(ray_wor_4D.x(),ray_wor_4D.y(),ray_wor_4D.z());
    ray_wor.normalize();

    Qt3D::QRay3D ray;
    ray.setOrigin(m_camera->position());
    ray.setDirection(ray_wor);

/*    QVector3D ray_dir_inv(1.0f/ray_wor.x(),1.0f/ray_wor.y(),1.0f/ray_wor.z());
    int sign[3];
    sign[0] = (ray_dir_inv.x() < 0);
    sign[1] = (ray_dir_inv.y() < 0);
    sign[2] = (ray_dir_inv.z() < 0);
*/
    qreal hitEntity_tnear;


    Qt3D::QEntity* hitEntity=select_recursive_step(m_sceneroot,QMatrix4x4(),hitEntity_tnear,ray);

    if(hitEntity)
        emit selectedItem(hitEntity);

}

Qt3D::QEntity* MouseInterface3D::select_recursive_step( Qt3D::QEntity* node ,const QMatrix4x4 parents_matrix, qreal &hitEntity_tnear, const Qt3D::QRay3D ray){

    Qt3D::QAbstractMesh* entity_mesh=NULL;
    Qt3D::QTransform* entity_transform=NULL;
    Qt3D::QComponentList components= node->components();
    qreal local_hitEntity_tnear, child_hitEntity_tnear;
    Qt3D::QEntity* hitEntity=NULL;

    for(Qt3D::QComponent* component: components){
        if(component->isEnabled()){
            if(component->inherits("Qt3D::QAbstractMesh")){
                entity_mesh=qobject_cast<Qt3D::QAbstractMesh*>(component);
            }
            else if(component->inherits("Qt3D::QTransform")){
                entity_transform=qobject_cast<Qt3D::QTransform*>(component);
            }
        }
    }

    if(entity_mesh!=NULL && entity_transform!=NULL){
        QMatrix4x4 transform_matrix=parents_matrix*entity_transform->matrix();
        QMatrix4x4 transform_matrix_inv=transform_matrix.inverted();
        Qt3D::QAbstractMeshFunctorPtr mesh_functor=entity_mesh->meshFunctor();
        Qt3D::QMeshDataPtr data_ptr=mesh_functor.data()->operator ()();
        Qt3D::QMeshData* mesh_data=data_ptr.data();

        /*qDebug()<<node->objectName();
        qDebug()<<mesh_data->attributeByName("vertexPosition")->asVector3D();

        qDebug()<<(uint16_t) mesh_data->indexAttribute()->buffer().data()->data().size();
        uint16_t* shorts = reinterpret_cast<uint16_t*>(mesh_data->indexAttribute()->buffer().data()->data().data());
        for (int i = 0; i <  mesh_data->indexAttribute()->buffer().data()->data().size()/2; ++i)
            qDebug()<<shorts[i];
        */
        //Qt3D::QAxisAlignedBoundingBox aabb= mesh_data->boundingBox();
        //QPair<QVector3D,QVector3D> aabb_mod=transform_aabb(aabb,transform_matrix);
        Qt3D::QRay3D ray_transform(transform_matrix_inv*ray.origin(),transform_matrix_inv.mapVector(ray.direction()));
        if(checkIntersectionRay_AABB(ray_transform,mesh_data->boundingBox())){
            if(mesh_data->primitiveType() == Qt3D::QMeshData::Triangles){
                QVector<QVector3D> vertexPosition=mesh_data->attributeByName("vertexPosition")->asVector3D();
                if (mesh_data->indexAttribute()->type()==GL_UNSIGNED_SHORT) {
                    const GLushort* indexes = reinterpret_cast<const GLushort*>(mesh_data->indexAttribute()->buffer().data()->data().constData());
                    for(uint i=0;i<mesh_data->indexAttribute()->count();i=i+3){
                        QVector<QVector3D> triangle;
                        triangle.append(vertexPosition[static_cast<int>(indexes[i])]);
                        triangle.append(vertexPosition[static_cast<int>(indexes[i+1])]);
                        triangle.append(vertexPosition[static_cast<int>(indexes[i+2])]);
                        if(checkIntersectionRay_Triangle(ray_transform,triangle,local_hitEntity_tnear))
                            hitEntity=node;
                    }
                }
              }
            }
        }


    QList<Qt3D::QEntity*> entities= node->findChildren<Qt3D::QEntity*>(QString(),Qt::FindDirectChildrenOnly);
    QMatrix4x4 transform_matrix;
    if(entity_transform!=NULL)
        transform_matrix=parents_matrix*entity_transform->matrix();
    else
        transform_matrix=parents_matrix;

    for (Qt3D::QEntity* entity : entities){
        Qt3D::QEntity* hitChild=select_recursive_step(entity,transform_matrix,child_hitEntity_tnear,ray);
        if(hitChild!=NULL){
            if(hitEntity==NULL || child_hitEntity_tnear<local_hitEntity_tnear){
                hitEntity=hitChild;
                local_hitEntity_tnear=child_hitEntity_tnear;
            }
        }
    }

    hitEntity_tnear=local_hitEntity_tnear;
    return hitEntity;

}




/*Code from
 * http://www.scratchapixel.com/lessons/3d-basic-rendering/minimal-ray-tracer-rendering-simple-shapes/ray-box-intersection*/
bool MouseInterface3D::checkIntersectionRay_AABB(const Qt3D::QRay3D ray, const Qt3D::QAxisAlignedBoundingBox aabb){



    QVector3D ray_dir_inv(1.0f/ray.direction().x(),1.0f/ray.direction().y(),1.0f/ray.direction().z());

    int sign[3];
    sign[0] = (ray_dir_inv.x() < 0);
    sign[1] = (ray_dir_inv.y() < 0);
    sign[2] = (ray_dir_inv.z() < 0);

    qreal tmin, tmax, tymin, tymax, tzmin, tzmax;

    QVector3D bounds[2]={aabb.minPoint(), aabb.maxPoint()};

    tmin = (bounds[sign[0]].x() - ray.origin().x()) * ray_dir_inv.x();
    tmax = (bounds[1-sign[0]].x() - ray.origin().x()) * ray_dir_inv.x();
    tymin = (bounds[sign[1]].y() - ray.origin().y()) * ray_dir_inv.y();
    tymax = (bounds[1-sign[1]].y() - ray.origin().y()) * ray_dir_inv.y();

    if ((tmin > tymax) || (tymin > tmax))
        return false;
    if (tymin > tmin)
        tmin = tymin;
    if (tymax < tmax)
        tmax = tymax;

    tzmin = (bounds[sign[2]].z() - ray.origin().z()) * ray_dir_inv.z();;
    tzmax = (bounds[1-sign[2]].z() - ray.origin().z()) * ray_dir_inv.z();;

    if ((tmin > tzmax) || (tzmin > tmax))
        return false;
    if (tzmin > tmin)
        tmin = tzmin;
    if (tzmax < tmax)
        tmax = tzmax;

    return true;

}

/*Based on http://geomalgorithms.com/a06-_intersect-2.html#intersect3D_RayTriangle%28%29*/
bool MouseInterface3D::checkIntersectionRay_Triangle(const Qt3D::QRay3D ray, const QVector<QVector3D> triangle,qreal &tnear){
    if(triangle.size()!=3){qWarning()<<"Triangle doesn't have 3 points!";return false;}

    QVector3D u=triangle[1]-triangle[0]; //precomputable
    QVector3D v=triangle[2]-triangle[0]; //precomputable

    QVector3D n= QVector3D::crossProduct(u,v);

    if(n.length()==0){
        qWarning()<<"Triangle is degenerate";return false;}

    QVector3D w0=ray.origin()-triangle[0];

    qreal a=-QVector3D::dotProduct(n,w0);
    qreal b=QVector3D::dotProduct(n,ray.direction());

    if(fabs(b)<0.00000001){
        if(a==0) {qWarning()<<"Ray lies in triangle plane";return false;}

        return false;
    }

    qreal r=a/b;

    if(r<0.0)
        return 0;

    QVector3D P=ray.origin()+r*ray.direction();

    qreal uu, uv, vv, wu, wv, D;

    uu = QVector3D::dotProduct(u,u);
    uv = QVector3D::dotProduct(u,v);
    vv = QVector3D::dotProduct(v,v);
    QVector3D w = P - triangle[0];
    wu = QVector3D::dotProduct(w,u);
    wv = QVector3D::dotProduct(w,v);
    D = uv * uv - uu * vv;

    qreal s, t;
    s = (uv * wv - vv * wu) / D;
    if (s < 0.0 || s > 1.0)         // I is outside T
        return false;
    t = (uv * wu - uu * wv) / D;
    if (t < 0.0 || (s + t) > 1.0)  // I is outside T
        return false;

    tnear=r;

    return 1;                       // I is in T




}



