// Copyright (C) 2024 Marvin-VW
#pragma once

#include "shape.h"

#include <cstdint>



class Shape;
class CameraModel;
class HomogenousTransformationMatrix;
class FpsCounter;
class ClippingSpace;
class Vectors;
class Color;

class RenderSystem
{
public:
    RenderSystem();

	bool release();
    CameraModel* create_matrices(std::vector<double> trackbarPos);
    HomogenousTransformationMatrix* init_matrices();
    ClippingSpace* init_clipping();
    FpsCounter* update_fps();
    Vectors* init_vector();
    Color* init_color();

    Shape* createCube();
    CameraModel* createCamera(  double sensorWidth,
                                                double sensorHeight,
                                                double focalLength,
                                                uint32_t resolutionX,
                                                uint32_t resolutionY,
                                                uint32_t u0,
                                                uint32_t v0);

    cv::Mat renderFrame();
    bool isRunning();
    void shutdown();
    void update_movement(int key);

public:
    CameraModel* cm;


private:
    friend class Shape;
    friend class HomogenousTransformationMatrix;
    friend class FpsCounter;
    friend class Vectors;
    friend class Color;
private:
    std::vector<triangle> mesh;
private:
    HomogenousTransformationMatrix* ht;
    ClippingSpace * cs;
    Vectors* v;
    Color* c;
    FpsCounter* fc;
};
