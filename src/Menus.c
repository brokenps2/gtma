#include "WindowManager.h"
#define CIMGUI_DEFINE_ENUMS_AND_STRUCTS
#define CIMGUI_USE_GLFW
#define CIMGUI_USE_OPENGL3
#include "Files.h"
#include <GLFW/glfw3.h>
#include <cimgui.h>
#include <cimgui_impl.h>

ImGuiIO* ioptr;

void initUI() {
    #if __APPLE__
        const char *glslVersion = "#version 150";
    #else
        const char *glslVersion = "#version 130";
    #endif

    igCreateContext(NULL);
    ioptr = igGetIO();
    ioptr->ConfigFlags |= ImGuiConfigFlags_NoKeyboard;
    ImGui_ImplGlfw_InitForOpenGL(getWindow(), true);
    ImGui_ImplOpenGL3_Init(glslVersion);

    igStyleColorsDark(NULL);

    ImFontAtlas_AddFontFromFileTTF(ioptr->Fonts, res("fonts/gtmadefaultfont1.ttf"), 16, NULL, NULL);
}

ImVec4 clearColor;

void updateUI() {
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    igNewFrame();

    if (igBeginMainMenuBar()) {
        if (igBeginMenu("File", true)) {
            if (igMenuItemEx("New", NULL, NULL, NULL, true)) { 
            }
            igEndMenu();
        }
        igEndMainMenuBar();
    }


    {

      igBegin("Menu", NULL, 0);
      ImVec4 menuCol = {1, 0, 0, 1};
      igPushStyleColor_Vec4(ImGuiCol_TitleBgActive, menuCol);

      ImVec2 buttonSize = {0, 0};
      if(igButton("Lock Mouse", buttonSize) && glfwGetInputMode(getWindow(), GLFW_CURSOR) == GLFW_CURSOR_NORMAL) {
          glfwSetInputMode(getWindow(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);
      }
        
      igText("Average frame time: %.3f ms/frame (%.1f FPS)",
             1000.0f / igGetIO()->Framerate, igGetIO()->Framerate);

      igPopStyleColor(1);

      igEnd();
    }

    igRender();
    ImGui_ImplOpenGL3_RenderDrawData(igGetDrawData());

}

ImGuiIO* getIOPtr() {
    return ioptr;
}

void disposeUI() {
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    igDestroyContext(NULL);
}
