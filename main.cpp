#ifdef BX_PLATFORM_LINUX
    #define GLFW_EXPOSE_NATIVE_X11
#elif BX_PLATFORM_WINDOWS
    #define GLFW_EXPOSE_NATIVE_WIN32
#elif BX_PLATFORM_OSX
    #define GLFW_EXPOSE_NATIVE_COCOA
#endif
#define S_WIDTH 1600
#define S_HEIGHT 900

#include <iostream>
#include "BGFX/bgfx.h"
#include "GLFW/glfw3native.h"
#include "GLFW/glfw3.h"

int main() {
    int width, height = 0;
    if (!glfwInit()) {
        std::cout << "Error loading GLFW\n";
        return 1;
    }
    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    GLFWwindow* window = glfwCreateWindow(S_WIDTH, S_HEIGHT, "Hellope, worl!", glfwGetPrimaryMonitor(), NULL);
    if (!window) {
        std::cout << "Error starting window\n";
        glfwTerminate();
        return 1;
    }

    bgfx::Init init;
    #if BX_PLATFORM_LINUX
        init.platformData.ndt = glfwGetX11Display();
        init.platformData.nwh = glfwGetX11Window();
    #elif BX_PLATFORM_WINDOWS
        init.platformData.nwh = glfwGetWin32Window();
    #elif BX_PLATFORM_OSX
        init.platformData.nwh = glfwGetCocoaWindow();
    #endif
    init.type = bgfx::RendererType::Count;
    init.resolution.width = S_WIDTH;
    init.resolution.height = S_HEIGHT;
    init.resolution.reset = BGFX_RESET_VSYNC;

    if (!bgfx::init(init)) {
        std::cout << "Failed to initialise BGFX\n";
        glfwTerminate();
        return 1;
    }
    bgfx::ViewId mView = 0;
    bgfx::setViewClear(mView, BGFX_CLEAR_COLOR | BGFX_CLEAR_DEPTH, 0x443355FF);
    bgfx::setViewRect(mView, 0, 0, S_WIDTH, S_HEIGHT);
    
    std::cout << "Hellope, worl!\n";
    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();
        int oldWidth = width;
        int oldHeight = height;
        glfwGetWindowSize(window, &width, &height);
        if (width != oldWidth || height != oldHeight) {
            bgfx::reset(width, height, BGFX_RESET_VSYNC);
            bgfx::setViewRect(mView, 0, 0, width, height);
        }
        bgfx::touch(mView);
        bgfx::frame();
    }
    bgfx::shutdown();
    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}
