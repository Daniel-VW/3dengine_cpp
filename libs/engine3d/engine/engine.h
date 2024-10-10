// Copyright (C) 2024 Marvin-VW
#pragma once

#include "render_system.h"
#include "shape.h"
#include "camera_model.h"
#include "homogenous_transformation_matrix.h"
#include "clipping_space.h"
#include "vectors.h"
#include "color.h"
#include "fps_counter.h"

#include <opencv2/core.hpp>

#include <vector>

namespace engine3d::engine {

namespace HTM = engine3d::engine::homogeneous_transformation_matrix;

class Engine {
public:
    Engine(int frame_width, int frame_height);
    ~Engine();

	cv::Mat run(cv::Mat& frame, const HTM::Matrix::Parameter parameter, double lane_parameters[]);
    CameraModel camera;
    RenderSystem renderer;


private:
    Shape shape;
    ClippingSpace clipping;
    Vectors vector;
    Color color;
    FpsCounter fps;

    std::vector<triangle> mesh;
    std::vector<triangle> visible_mesh;
    std::vector<triangle> clipped_mesh;

    cv::Mat engine_frame;

    double is_triangle_facing_camera(triangle& tri, cv::Vec3d cam);
};

}