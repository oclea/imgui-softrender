#include <imgui/imgui.h>
#include "imgui/imgui_softrender.h"
#include <chrono>
#include <string>
#include <opencv2/opencv.hpp>

using namespace cv;

#define SCREENX 1024
#define SCREENY 768

texture_color32_t screen;
texture_alpha8_t fontAtlas;

unsigned long millis()
{
    using namespace std::chrono;
    static auto start = high_resolution_clock::now();
    return duration_cast<milliseconds>(high_resolution_clock::now() - start)
        .count();
}

void screen_draw(int frame_index)
{
    // export pixels as a PNG using opencv
    cv::Mat img(SCREENY, SCREENX, CV_8UC4, screen.pixels);
    cv::imwrite("frame_" + std::to_string(frame_index) + ".png", img);
}

int main(int, char **)
{
    // Initialize the screen texture
    screen.init(SCREENX, SCREENY);

    ImGui::CreateContext();

    ImGui_ImplSoftrender_Init(&screen);

    ImGuiStyle &style      = ImGui::GetStyle();
    style.AntiAliasedLines = false;
    style.AntiAliasedFill  = false;
    style.WindowRounding   = 0.0f;

    ImGuiIO &io = ImGui::GetIO();
 
    uint8_t *pixels;
    int width, height;
    io.Fonts->GetTexDataAsAlpha8(&pixels, &width, &height);
    fontAtlas.init(width, height, (alpha8_t *)pixels);
    io.Fonts->TexID = reinterpret_cast<uintptr_t>(&fontAtlas);

    io.MouseDrawCursor = true;

    int k = 0;
    float f = 0.0f;
    unsigned long drawTime = 0;
    unsigned long renderTime = 0;
    unsigned long rasterTime = 0;
    unsigned int t = millis();
    while (k < 3)
    {
        ImGuiIO &io  = ImGui::GetIO();
        io.DeltaTime = 1.0f / 60.0f;

        io.MousePos.x = 100*k;
        io.MousePos.y = 200*k;

        ImGui_ImplSoftrender_NewFrame();
        ImGui::NewFrame();
        ImGui::SetWindowPos(ImVec2(0.0, 0.0));
        ImGui::SetWindowSize(ImVec2(SCREENX, SCREENY));

        f += 0.05;
        if (f > 1.0f) f = 0.0f;

        unsigned int deltaTime = millis() - t;
        t += deltaTime;

        deltaTime -= (drawTime + renderTime + rasterTime);

        ImGui::Text("Hardware write time %ld ms", drawTime);
        ImGui::Text("Render time %ld ms", renderTime);
        ImGui::Text("Raster time %ld ms", rasterTime);
        ImGui::Text("Remaining time %d ms", deltaTime);
        ImGui::Text("abcdefghijklmnopqrstuvwxyz");
        ImGui::Text("ABCDEFGHIJKLMNOPQRSTUVWXYZ");
        ImGui::Text("0123456789");
        ImGui::Text("!@#$%%^&*()_+");
        ImGui::Text("~`-=[]\\;',./{}|:\"<>?");
        ImGui::Text("Mouse position: (%f, %f)", io.MousePos.x, io.MousePos.y);
        ImGui::SliderFloat("SliderFloat", &f, 0.0f, 1.0f);

        ImGui::ShowDemoWindow();

        renderTime = millis();
        ImGui::Render();
        renderTime = millis() - renderTime;

        rasterTime = millis();
        ImGui_ImplSoftrender_RenderDrawData(ImGui::GetDrawData());
        rasterTime = millis() - rasterTime;

        drawTime = millis();
        screen_draw(k);
        drawTime = millis() - drawTime;
        k++;
    }

}
