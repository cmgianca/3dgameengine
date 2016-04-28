QT += core gui opengl

TARGET = final
TEMPLATE = app

win32 {
    DEFINES += GLEW_STATIC
    LIBS += -lopengl32
}

QMAKE_CXXFLAGS += -std=c++11

INCLUDEPATH += src libs glm libs/glew-1.10.0/include
DEPENDPATH += src libs glm libs/glew-1.10.0/include

SOURCES += \
    libs/glew-1.10.0/src/glew.c \
    src/engine/common/collisiongeometry/boundingbox.cpp \
    src/engine/common/collisiongeometry/collisionshape.cpp \
    src/engine/common/collisiongeometry/cylindershape.cpp \
    src/engine/common/noise/valuenoise.cpp \
    src/engine/common/rendergeometry/background.cpp \
    src/engine/common/rendergeometry/cylinder.cpp \
    src/engine/common/rendergeometry/quad.cpp \
    src/engine/common/rendergeometry/shape.cpp \
    src/engine/common/rendergeometry/skybox.cpp \
    src/engine/common/application.cpp \
    src/engine/common/camera.cpp \
    src/engine/common/entity.cpp \
    src/engine/common/graphics.cpp \
    src/engine/common/manager.cpp \
    src/engine/common/world.cpp \
    src/engine/voxel/blockcover.cpp \
    src/engine/voxel/blocks.cpp \
    src/engine/voxel/chunk.cpp \
    src/engine/voxel/voxelmanager.cpp \
    src/game/bullet.cpp \
    src/game/enemy.cpp \
    src/game/game.cpp \
    src/game/gameentity.cpp \
    src/game/gameworld.cpp \
    src/game/player.cpp \
    src/game/startworld.cpp \
    src/game/worldblocks.cpp \
    src/util/ResourceLoader.cpp \
    src/main.cpp \
    src/mainwindow.cpp \
    src/view.cpp \
    src/engine/common/rendergeometry/obj.cpp \
    src/engine/common/rendergeometry/ellipse.cpp \
    src/engine/geometric/geometricmanager.cpp \
    src/engine/geometric/rendermesh.cpp \
    src/engine/geometric/navgraph.cpp \
    src/engine/common/menuworld.cpp \
    src/engine/common/collisiongeometry/polyhedrashape.cpp \
    src/engine/common/rendergeometry/polyhedron.cpp \
    src/game/gameobject.cpp




HEADERS += \
    libs/glew-1.10.0/include/GL/glew.h \
    src/mainwindow.h \
    src/engine/common/collisiongeometry/boundingbox.h \
    src/engine/common/collisiongeometry/collisionshape.h \
    src/engine/common/collisiongeometry/cylindershape.h \
    src/engine/common/collisiongeometry/nullshape.h \
    src/engine/common/noise/valuenoise.h \
    src/engine/common/rendergeometry/background.h \
    src/engine/common/rendergeometry/cylinder.h \
    src/engine/common/rendergeometry/quad.h \
    src/engine/common/rendergeometry/shape.h \
    src/engine/common/rendergeometry/skybox.h \
    src/engine/common/application.h \
    src/engine/common/camera.h \
    src/engine/common/direction.h \
    src/engine/common/entity.h \
    src/engine/common/graphics.h \
    src/engine/common/manager.h \
    src/engine/common/raycollision.h \
    src/engine/common/world.h \
    src/engine/voxel/blockcover.h \
    src/engine/voxel/blocks.h \
    src/engine/voxel/chunk.h \
    src/engine/voxel/voxelmanager.h \
    src/game/bullet.h \
    src/game/enemy.h \
    src/game/game.h \
    src/game/gameentity.h \
    src/game/gameworld.h \
    src/game/player.h \
    src/game/startworld.h \
    src/game/worldblocks.h \
    src/util/CommonIncludes.h \
    src/util/CylinderData.h \
    src/util/ResourceLoader.h \
    src/view.h \
    src/engine/common/rendergeometry/NewCylinderData.h \
    src/engine/common/rendergeometry/obj.h \
    src/engine/common/rendergeometry/SphereData.h \
    src/engine/common/rendergeometry/ellipse.h \
    src/engine/geometric/geometricmanager.h \
    src/engine/geometric/rendermesh.h \
    src/engine/geometric/navgraph.h \
    src/engine/common/menuworld.h \
    src/engine/common/collisiongeometry/polyhedrashape.h \
    src/engine/common/rendergeometry/polyhedron.h \
    src/game/gameobject.h


FORMS += src/mainwindow.ui

RESOURCES += \
    res/shaders/shaders.qrc \
    res/images/images.qrc \
    res/meshes.qrc

OTHER_FILES += \
    res/images/grass.png \

DISTFILES += \
    res/shaders/shader.vert \
    res/shaders/shader.frag \
    res/meshes/level_easy.obj \
    res/meshes/level_easy_navmesh.obj \
    res/meshes/level_hard.obj \
    res/meshes/level_island.obj \
    res/meshes/level_island_navmesh.obj \
    res/meshes/level_island_navmesh_eroded.obj
