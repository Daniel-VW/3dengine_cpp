// Copyright (C) 2024 Daniel-VW
#include <iostream>
#include <vector>
#include "shape.h"

cv::Mat Shape::createPoint(double x, double y, double z) {
    cv::Mat point(4,1,CV_64F);
    point.at<double>(0) = x;
    point.at<double>(1) = y;
    point.at<double>(2) = z;
    point.at<double>(3) = 1;
    return point;
}

std::vector<triangle> Shape::generate_mesh(double len, double wi) { // not used right now
    std::vector<triangle> mesh;
    // Edit parameters here:
    double length = 0.35;
    double width = 0.015;
    double solidLineWidth = width * 1;
    double stripedLineWidth = width * 1;
    double wantedStripeNum = 5;
    double stripedLineSpaceMultiplier = 0.5; // value * striped line len = space between stipes
    double stripeLength = 0.035;
    double LineDistance = 0.1;
    double stripeSpacing = length / wantedStripeNum * stripedLineSpaceMultiplier;
    double actStripeCount = 1;
    bool stripedLine = true;
    bool rightLine = true;
    bool leftLine = true;

    while (stripeLength * actStripeCount + stripeSpacing * (actStripeCount) < length) {
        actStripeCount = actStripeCount + 1;
        if (actStripeCount == wantedStripeNum) {
            break;
        }
    }

    if (rightLine) {
    cv::Mat SolidLine_P0 = createPoint(-length / 2, solidLineWidth / 2 - (LineDistance), 0);  // Top left
    cv::Mat SolidLine_P1 = createPoint(-length / 2, -solidLineWidth / 2 - (LineDistance), 0); // Bottom left
    cv::Mat SolidLine_P2 = createPoint(length / 2, -solidLineWidth / 2 - (LineDistance), 0);  // Bottom right
    cv::Mat SolidLine_P3 = createPoint(length / 2, solidLineWidth / 2 - (LineDistance), 0);   // Top right

    // Triangles
    mesh.push_back({ {SolidLine_P0, SolidLine_P1, SolidLine_P2} }); 
    mesh.push_back({ {SolidLine_P0, SolidLine_P2, SolidLine_P3} }); 
    }

    if (leftLine) {
    cv::Mat SecSolidLine_P0 = createPoint(-length / 2, solidLineWidth / 2 + (LineDistance), 0);  // Top left
    cv::Mat SecSolidLine_P1 = createPoint(-length / 2, -solidLineWidth / 2 + (LineDistance), 0); // Bottom left
    cv::Mat SecSolidLine_P2 = createPoint(length / 2, -solidLineWidth / 2 + (LineDistance), 0);  // Bottom right
    cv::Mat SecSolidLine_P3 = createPoint(length / 2, solidLineWidth / 2 + (LineDistance), 0);   // Top right

    // Triangles
    mesh.push_back({ {SecSolidLine_P0, SecSolidLine_P1, SecSolidLine_P2} }); 
    mesh.push_back({ {SecSolidLine_P0, SecSolidLine_P2, SecSolidLine_P3} }); 
    }

    if (stripedLine) {
    // double stripeLength = (length - stripeSpacing * (wantedStripeNum - 1)) / wantedStripeNum;
    for (int i = 0; i < actStripeCount; ++i) {
        double stripeStart = -length / 2 + i * (stripeLength + stripeSpacing);
        double stripeEnd = stripeStart + stripeLength;

        cv::Mat Stripe_P0 = createPoint(stripeStart, stripedLineWidth / 2, 0);  // Top left
        cv::Mat Stripe_P1 = createPoint(stripeStart, -stripedLineWidth / 2, 0); // Bottom left
        cv::Mat Stripe_P2 = createPoint(stripeEnd, -stripedLineWidth / 2, 0);   // Bottom right
        cv::Mat Stripe_P3 = createPoint(stripeEnd, stripedLineWidth / 2, 0);    // Top right

        // Triangles
        mesh.push_back({ {Stripe_P0, Stripe_P1, Stripe_P2} });
        mesh.push_back({ {Stripe_P0, Stripe_P2, Stripe_P3} });
    }
    }

    return mesh;
}


void Shape::set_position(double x, double y, double z, triangle* tri) {
    double temp_matrix[4*4] = {
        1, 0, 0, x,
        0, 1, 0, y,
        0, 0, 1, z,
        0, 0, 0, 1
    };

    cv::Mat translation_matrix(4, 4, CV_64F, temp_matrix);

    // Translation to each triangle point
    for (int i = 0; i < 3; ++i) {
        cv::Mat point = tri->point[i];
        cv::Mat transformed_point = translation_matrix * point;
        tri->point[i] = transformed_point;
        std::cout << tri->point[i] << std::endl;
    }
}
