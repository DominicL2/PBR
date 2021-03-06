/**
 * @file spacedata.h
 *
 * @brief define space data of GL
 *
 * @author DM Lee
 * Contact: kwoo2007@naver.com
 *
 */
#ifndef SPACEDATA_H
#define SPACEDATA_H

#include <glm/glm.hpp>

#define GL_SPACE_DEFUALT_MAX_DISTANCE (10.f)
#define GL_SPACE_DEFUALT_LIGHT_SOURCE_POS_X (1.3)
#define GL_SPACE_DEFUALT_LIGHT_SOURCE_POS_Y (0.0)
#define GL_SPACE_DEFUALT_LIGHT_SOURCE_POS_Z (0.16)

#define GL_SPACE_DEFUALT_VIEW_POINT_POS_X (0)
#define GL_SPACE_DEFUALT_VIEW_POINT_POS_Y (0.5)
#define GL_SPACE_DEFUALT_VIEW_POINT_POS_Z (1.0)

#define GL_SPACE_AXIS_LINE_LENGTH (5.0)

namespace GLSpace {
    typedef struct {
        glm::vec3 leftTop;
        glm::vec3 rightTop;
        glm::vec3 rightBottom;
        glm::vec3 leftBottom;
    } SurfaceOfVolume;

    typedef struct {
        glm::vec3 lightSource;
        glm::vec3 lightColor;
        glm::vec3 viewPoint;
        GLSpace::SurfaceOfVolume near;
        GLSpace::SurfaceOfVolume far;
        float fov;
    } SpaceInfo;

    typedef struct {
        glm::vec3 start;
        glm::vec3 end;
    } Line;

   class Rectangle
   {
    public:
        int32_t x;
        int32_t y;
        int32_t width;
        int32_t height;

        Rectangle() {
            this->x = 0;
            this->y = 0;
            this->width = 0;
            this->height = 0;
        }

        Rectangle(const int32_t _x, const int32_t _y, const int32_t _width, const int32_t _height) {
            this->x = _x;
            this->y = _y;
            this->width = _width;
            this->height = _height;
        }

        Rectangle(const Rectangle &_rect) {
            this->x = _rect.x;
            this->y = _rect.y;
            this->width = _rect.width;
            this->height = _rect.height;
        }

        Rectangle& operator=(const Rectangle& _rect) {
            this->x = _rect.x;
            this->y = _rect.y;
            this->width = _rect.width;
            this->height = _rect.height;
            return *this;
        }
    };

    inline glm::vec3 getSideVector(glm::vec3 foward, glm::vec3 upVector) {
        return glm::cross(foward, upVector);
    }
    inline glm::vec3 getUpVector() {
        return glm::vec3(0.0, 1.0, 0.0);
    }
}

#endif // SPACEDATA_H
