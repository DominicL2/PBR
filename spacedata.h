#ifndef SPACEDATA_H
#define SPACEDATA_H

#include <glm/glm.hpp>

/// Default distance unit is "cm"
///
#define GL_SPACE_DEFUALT_DISTANCE_1KM (1000 * 100)

#define GL_SPACE_DEFUALT_LIGHT_SOURCE_POS_X (-400)
#define GL_SPACE_DEFUALT_LIGHT_SOURCE_POS_Y (700)
#define GL_SPACE_DEFUALT_LIGHT_SOURCE_POS_Z (400)

#define GL_SPACE_DEFUALT_VIEW_POINT_POS_X (0)
#define GL_SPACE_DEFUALT_VIEW_POINT_POS_Y (0)
#define GL_SPACE_DEFUALT_VIEW_POINT_POS_Z (100)


namespace GLSpace {
    typedef struct {
        glm::vec3 leftTop;
        glm::vec3 rightTop;
        glm::vec3 rightBottom;
        glm::vec3 leftBottom;
    } SurfaceOfVolume;

    typedef struct {
        glm::vec3 lightSource;
        glm::vec3 viewPoint;
        GLSpace::SurfaceOfVolume near;
        GLSpace::SurfaceOfVolume far;
        float fov;
    } SpaceInfo;

    inline glm::vec3 getSideVector(glm::vec3 foward, glm::vec3 upVector) {
        return glm::cross(foward, upVector);
    }
    inline glm::vec3 getUpVector() {
        return glm::vec3(0.0, 1.0, 0.0);
    }
}

#endif // SPACEDATA_H
