#pragma once
#include "../include/glad/glad.h"
#include <cmath>
#include <iostream>

struct Rectangle{
    float w;
    float h;
    GLuint rotation;

    //x and y
    float center[2];
    float t[2];
    float l[2];
    float r[2];
    float b[2];

    float vertices[12];

    public:
        Rectangle(const float& w, const float& h, const float& x_center, const float& y_center, const GLuint& rotation){
            this->w = w;
            this->h = h;
            this->center[0] = x_center;
            this->center[1] = y_center;
            this->rotation = rotation % 360;

            float diagonal = sqrt(w*w + h*h);

            /*
             *                 t
             *                0,1
             *
             *              diagonal
             *         -----------------
             *         |               |
             *         l     center    r
             *        0,1              0,1
             *
             *
             *                 b
             *                0,1
             *
             *
             *
             */

            this->l[0] = this->center[0] - diagonal / 2;
            this->r[0] = this->center[0] + diagonal / 2;

            this->l[1] = this->center[1];
            this->r[1] = this->center[1];

            this->t[0] = this->center[0];
            this->b[0] = this->center[0];

            this->t[1] = this->center[1] + diagonal / 2;
            this->b[1] = this->center[1] - diagonal / 2;
        }

        void assignVerticesForDrawing(){
            //need draw 2 triangles, so gonna use 6 vertices
            float vertices[] = {
                //       t
                // l     |      r
                //       b
                //
                //first triangle

                this->l[0], this->l[1],
                this->t[0], this->t[1],
                this->b[0], this->b[1],

                //second triangle
                this->t[0], this->t[1],
                this->r[0], this->r[1],
                this->b[0], this->b[1]
            };
            for (int i = 0; i < 12; i++){
                this->vertices[i] = vertices[i];
            }
        }
};
