#include "engine/geometric/navgraph.h"
#include "engine/common/rendergeometry/obj.h"
#include <QQueue>

NavGraph::NavGraph(OBJ* o) {
    trianglecount_ = o->triangles.length();
    triangles_ = new Triangle[trianglecount_]();
    {
        int i = 0;
        QList<OBJ::Triangle*>::iterator tri;
        for (tri = o->triangles.begin(); tri != o->triangles.end(); ++tri) {
            Triangle t = {{(*tri)->vertices[0],(*tri)->vertices[1],(*tri)->vertices[2]}, (*tri)->normal};
            triangles_[i] = t;
            i++;

        }
    }
    int portalcount = 0;
    QList<Portal> portals;
    for (int i = 0; i < trianglecount_; i++) {
        for (int j = i+1; j < trianglecount_; j++) {
            int sharedcount = 0;
            glm::vec3 point1 = glm::vec3(0,0,0);
            glm::vec3 point2 = glm::vec3(0,0,0);
            glm::vec3 center = glm::vec3(0,0,0);
            for (int k = 0; k < 3; k++) {
                for (int l = 0; l < 3; l++) {
                    if (glm::length2(triangles_[i].vertices[k]-triangles_[j].vertices[l]) < .01f) {
                        if (sharedcount == 0) {
                            sharedcount++;
                            point1 = triangles_[i].vertices[k];
                            break;
                        } else if (sharedcount == 1) {
                            sharedcount++;
                            point2 = triangles_[i].vertices[k];
                            break;
                        }
                    }
                }
            }
            if (sharedcount == 2) {
                center = .5f*(point2-point1)+point1;
                portalcount++;
                Portal p = {i, j, center, point1, point2};
                portals.append(p);
            }
        }
    }
    portals_ = new Portal[portalcount];
    portalcount_ = portalcount;
    for (int i = 0; i < portalcount; i++) {
        portals_[i] = portals.at(i);
    }
}

NavGraph::~NavGraph() {
    delete[] triangles_;
    delete[] portals_;
}

QList<int> NavGraph::Dijkstras(glm::vec3 start, glm::vec3 end) {
    QList<int> toReturn;
    int startnode = query(start);
    int endnode = query(end);
    if (startnode < 0 || endnode < 0 || startnode == endnode) {
        return toReturn;
    }
    QList<int> queue;
    int parent[portalcount_];
    GLfloat distance[portalcount_];
    for (int i = 0; i < portalcount_; i++) {
        distance[i] = 99999999.f;
        parent[i] = -1;
        if (startnode == portals_[i].index1 || startnode == portals_[i].index2) {
            distance[i] = 0;
        }
        queue.append(i);
    }
    while (queue.length() > 0) {
        int current = queue.at(0);
        for (int i = 1; i < queue.length(); i++) {
            if (distance[queue.at(i)] < distance[current]) {
                current = queue.at(i);
            }
        }
        queue.removeAll(current);
        if (portals_[current].index1 == endnode || portals_[current].index2 == endnode) {
            toReturn.append(current);
            while(parent[current] != -1) {
                current = parent[current];
                toReturn.append(current);
            }
            break;
        }

        QList<int> neighbors;
        for (int i = 0; i < portalcount_; i++) {
            if (current != i && (portals_[current].index1 == portals_[i].index1 ||
                                 portals_[current].index1 == portals_[i].index2 ||
                                 portals_[current].index2 == portals_[i].index1 ||
                                 portals_[current].index2 == portals_[i].index2)) {
                neighbors.append(i);
            }
        }

        for (int i = 0; i < neighbors.length(); i++) {
            int next = neighbors.at(i);
            GLfloat alt = distance[current] + glm::length(portals_[current].center - portals_[next].center);
            if (alt < distance[next]) {
                distance[next] = alt;
                parent[next] = current;
            }
        }
    }

    return toReturn;
}

QList<glm::vec3> NavGraph::findpath(glm::vec3 start, glm::vec3 end) {
    QList<glm::vec3> toReturn;
    QList<int> path = Dijkstras(start,end);
    glm::vec3 point = start;
    for (int i = path.length()-1; i >= 0 ; i--) {
        Portal n = portals_[path.at(i)];
        glm::vec3 point2 = glm::vec3(point.x, 0,point.z);
        glm::vec3 left2 = glm::vec3(n.leftpoint.x, 0, n.leftpoint.z);
        glm::vec3 right2 = glm::vec3(n.rightpoint.x, 0, n.rightpoint.z);
        if (glm::cross(left2-point2, right2-point2).y >= 0) {
            portals_[path.at(i)].leftpoint = n.rightpoint;
            portals_[path.at(i)].rightpoint = n.leftpoint;
        }
        point = n.center;
    }
    glm::vec3 apex = start;
    int index = path.length()-1;
    Portal p = portals_[path.at(index)];
    glm::vec3 left = p.leftpoint*.99f + p.rightpoint*.01f;
    glm::vec3 right = p.rightpoint*.99f + p.leftpoint*.01f;
    glm::vec3 leftVec = glm::vec3(left.x-apex.x, 0, left.z-apex.z);
    glm::vec3 rightVec = glm::vec3(right.x-apex.x, 0, right.z-apex.z);
    GLfloat width;
    if (glm::length2(leftVec) == 0 || glm::length2(rightVec) == 0) {
        width = 9999.f;
    } else {
        width = acos(glm::dot(left,right)/(glm::length2(leftVec)*glm::length2(rightVec)));
    }
    bool done = false;
    while (glm::length2(apex-end) > .01f && !done) {
        index--;
        if (index < 0) {
            p = {0,0,end,end,end};
            done = true;
        } else {
            p = portals_[path.at(index)];
        }
        glm::vec3 newLeft = p.leftpoint*.99f + p.rightpoint*.01f;
        leftVec = (newLeft-apex)*glm::vec3(1,0,1);
        rightVec = (right-apex)*glm::vec3(1,0,1);
        GLfloat newWidth;
        if (glm::length2(leftVec) == 0 || glm::length2(rightVec) == 0) {
            newWidth = 0;
        } else {
            newWidth = acos(glm::dot(leftVec,rightVec)/(glm::length2(leftVec)*glm::length2(rightVec)));
        }
        if (glm::cross(rightVec, leftVec).y < 0) {
            toReturn.append(right);
            apex = right;
        } else {
            if (newWidth < width) {
                left = newLeft;
                width = newWidth;
            }
        }
        glm::vec3 newRight = p.rightpoint*.99f + p.leftpoint*.01f;
        leftVec = (left-apex)*glm::vec3(1,0,1);
        rightVec = (newRight-apex)*glm::vec3(1,0,1);
        if (glm::length2(leftVec) == 0 || glm::length2(rightVec) == 0) {
            newWidth = 0;
        } else {
            newWidth = acos(glm::dot(leftVec,rightVec)/(glm::length2(leftVec)*glm::length2(rightVec)));
        }
        if (glm::cross(rightVec, leftVec).y < 0) {
            toReturn.append(left);
            apex = left;
        } else {
            if (newWidth < width) {
                right = newRight;
                width = newWidth;
            }
        }
    }
    return toReturn;
}

int NavGraph::query(glm::vec3 point) {
    GLfloat minimum = 99999999;
    int toReturn = -1;
    for (int i = 0; i < trianglecount_; i++) {
        Triangle triangle = triangles_[i];
        GLfloat t = shootRayTriangle(&triangle, point, glm::vec3(0,-1.f,0));
        if (t > 0 && t < minimum) {
            toReturn = i;
            minimum = t;
        }
    }
    return toReturn;
}

GLfloat NavGraph::shootRayTriangle(Triangle* tri, glm::vec3 pos, glm::vec3 dir) {
    if (glm::dot(tri->normal,dir) == 0) {
        return 0;
    } else {
        GLfloat t = -(glm::dot(tri->normal,pos-tri->vertices[0])/(glm::dot(tri->normal,dir)));
        if (t <= 0) {
            return 0;
        } else {
            glm::vec3 collisionPoint = pos + t*dir;
            glm::vec3 normal1 = glm::cross(tri->vertices[0]-collisionPoint, tri->vertices[1]-collisionPoint);
            glm::vec3 normal2 = glm::cross(tri->vertices[1]-collisionPoint, tri->vertices[2]-collisionPoint);
            glm::vec3 normal3 = glm::cross(tri->vertices[2]-collisionPoint, tri->vertices[0]-collisionPoint);
            if (glm::dot(normal1,normal2) > 0 && glm::dot(normal2,normal3) > 0 && glm::dot(normal3,normal1) > 0) {
                return t;
            } else {
                return 0;
            }
        }
    }
}
