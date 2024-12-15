#include <cmath>
#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <algorithm>
#include <functional>

#include <raylib.h>

// TODO: Use custom shader to draw the plot instead of inefficiently drawing lines

class Plot {
public:
    // plot has 2 main functions
    // plot, and graph
    // plot takes a dataset and plots it
    // graph takes a function and plots it
    // no windows or anything are opened until show is called
    enum class PlotType {
        Plot,
        Graph
    };
    std::vector<double> plot_x, plot_y;
    std::function<double(double)> func;
    PlotType type;

    double x_min, x_max, y_min, y_max;

    Plot() {
    }

    void plot(const std::vector<double>& x, const std::vector<double>& y, double x_min, double x_max, double y_min, double y_max) {
        plot_x = x;
        plot_y = y;
        type = PlotType::Plot;
        this->x_min = x_min;
        this->x_max = x_max;
        this->y_min = y_min;
        this->y_max = y_max;
        std::cout << "Plotting is not yet implemented" << std::endl;
    }
    void plot(const std::vector<double>& x, const std::vector<double>& y) {
        double x_min = *std::min_element(x.begin(), x.end());
        double x_max = *std::max_element(x.begin(), x.end());
        double y_min = *std::min_element(y.begin(), y.end());
        double y_max = *std::max_element(y.begin(), y.end());
        plot(x, y, x_min, x_max, y_min, y_max);
    }

    void graph(std::function<double(double)> f, double x_min, double x_max, double y_min, double y_max) {
        func = f;
        type = PlotType::Graph;
        this->x_min = x_min;
        this->x_max = x_max;
        this->y_min = y_min;
        this->y_max = y_max;
    }

    void graph(std::function<double(double)> f) {
        graph(f, -10, 10, -10, 10);
    }

    // the show function will either call showPlot or showGraph
    // depending on the type of plot
    
    void showGraph() {
        int width = 800;
        int height = 800;
        InitWindow(width, height, "Graph");
        width *= 5;
        height *= 5;
        SetTargetFPS(60);

        double x_range = x_max - x_min;
        double y_range = y_max - y_min;

        double x_scale = width / x_range;
        double y_scale = height / y_range;

        double drag_x = 0;
        double drag_y = 0;
        bool dragging = false;
        double zoom = 1;
        double camera_offset_x = 0;
        double camera_offset_y = 0;

        // raylib camera
        Camera2D camera = { 0 };
        camera.target = { 0, 0 };
        camera.offset = { 800 / 2, 800 / 2 };
        camera.rotation = 0.0f;
        camera.zoom = 1.0f;



        while (!WindowShouldClose()) {
            // camera movement
            // mouse drag to move camera
            // mouse wheel to zoom in and out
            if (IsMouseButtonDown(MOUSE_LEFT_BUTTON)) {
                if (!dragging) {
                    drag_x = GetMouseX();
                    drag_y = GetMouseY();
                    dragging = true;
                } else {
                    double dx = GetMouseX() - drag_x;
                    double dy = GetMouseY() - drag_y;
                    drag_x = GetMouseX();
                    drag_y = GetMouseY();
                    camera_offset_x -= dx / camera.zoom;
                    camera_offset_y -= dy / camera.zoom;
                }
            } else {
                dragging = false;
            }

            zoom += GetMouseWheelMove() * 0.1;
            if (zoom < 0.1) zoom = 0.1;


            // apply camera changes
            camera.zoom = zoom;
            camera.target.x = camera_offset_x;
            camera.target.y = camera_offset_y;

            BeginDrawing();
            ClearBackground(WHITE);

            BeginMode2D(camera);

            DrawLine(0, height / 2, width, height / 2, GRAY);
            DrawLine(width / 2, 0, width / 2, height, GRAY);
            // draw some grid lines with labels
            for (int i = -10; i <= 10; i++) {
                if (i == 0) continue;

                // font size will get bigger as we zoom out
                int font_size = 30 / zoom;
                if (font_size < 10) font_size = 10;
                if (font_size > 60) font_size = 60;
                double x = i;
                double y = i;
                double x_screen = (x - x_min) * x_scale;
                double y_screen = (y - y_min) * y_scale;
                DrawLine(x_screen, 0, x_screen, height, LIGHTGRAY);
                DrawLine(0, height - y_screen, width, height - y_screen, LIGHTGRAY);
                DrawText(std::to_string(i).c_str(), x_screen + 5, height / 2 - 10, font_size, BLACK);
                DrawText(std::to_string(i).c_str(), width / 2 + 5, height - y_screen - 10, font_size, BLACK);

            }
            int numLines = 1000*(zoom);
            numLines = std::max(numLines, 100);
            numLines = std::min(numLines, 10000);
            if (type == PlotType::Graph) {
                for (int i = 0; i < numLines; i++) {
                    double x1 = x_min + i * x_range / numLines;
                    double x2 = x_min + (i+1) * x_range / numLines;
                    double y1 = func(x1);
                    double y2 = func(x2);
                    double x1_screen = (x1 - x_min) * x_scale;
                    double y1_screen = (y1 - y_min) * y_scale;
                    double x2_screen = (x2 - x_min) * x_scale;
                    double y2_screen = (y2 - y_min) * y_scale;
                    DrawLine(x1_screen, height - y1_screen, x2_screen, height - y2_screen, RED);
                }
            }

            EndMode2D();

            EndDrawing();
        }

        CloseWindow();
    }

    void show() {
        if (type == PlotType::Plot) {
            // showPlot();
        } else if (type == PlotType::Graph) {
            showGraph();
        }
    }
};