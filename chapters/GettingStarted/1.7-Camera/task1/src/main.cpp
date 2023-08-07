#include "glad/glad.h"
#include "GLFW/glfw3.h"

#include "Shader1/Shader.h"
#include "stb_image.h"

#include "glm.hpp"
#include "gtc/matrix_transform.hpp"
#include "gtc/type_ptr.hpp"

#include "GUI/ImGuiWrapper.h"
#include "Buffers/Buffers.h"
#include "Utility/Debug.h"
#include "Utility/Callbacks.h"
#include "Input/Input.h"
#include "Camera/FlyCamera.h"
#include "Data.h"


int g_WindowWidth = 800;
int g_WindowHeight = 600;

float g_DeltaTime = 0.0f;
float g_LastFrameTime = 0.0f;

float g_MouseLastX = 400.0f;
float g_MouseLastY = 300.0f;

bool g_bFirstMouseInput = true;

FlyCamera g_Camera(glm::vec3(0.0f, 0.0f, 3.0f));


int main()
{
    glfwInit();

    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(g_WindowWidth, g_WindowHeight, "LearnOpenGL", nullptr, nullptr);
    if (!window)
    {
        std::cerr << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cerr << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    //glViewport(0, 0, g_WindowWidth, g_WindowHeight);

    glfwSetWindowSizeCallback(window, Callbacks::OnWindowSizeChanged);
    glfwSetCursorPosCallback(window, Callbacks::OnMouseMoved);
    glfwSetScrollCallback(window, Callbacks::OnMouseScrolled);
    glfwSetMouseButtonCallback(window, Callbacks::OnMouseButtonAction);

#ifdef _DEBUG
    glEnable(GL_DEBUG_OUTPUT);
    glDebugMessageCallback(Debug::OnOpenGLdebugCallback, nullptr);
#endif

    ImGuiWrapper::Init(window, "#version 460 core");

    unsigned int textures[2];
    glGenTextures(2, textures);

    for (size_t i = 0; i < 2; ++i)
    {
        glBindTexture(GL_TEXTURE_2D, textures[i]);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    }

    stbi_set_flip_vertically_on_load(true);

    int width, height, NumColorChannels;
    std::string woodenContainerPath = (std::string(ASSETS_PATH) + "/wooden_container.jpg");
    unsigned char* data = stbi_load(woodenContainerPath.data(), &width, &height, &NumColorChannels, 0);

    glBindTexture(GL_TEXTURE_2D, textures[0]);
    if (data)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        std::cerr << "Failed to load texture" << std::endl;
    }

    stbi_image_free(data);

    glBindTexture(GL_TEXTURE_2D, textures[1]);


    std::string awesomeFacePath = (std::string(ASSETS_PATH) + "/awesomeface.png");
    data = stbi_load(awesomeFacePath.data(), &width, &height, &NumColorChannels, 0);

    glBindTexture(GL_TEXTURE_2D, textures[1]);
    if (data)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        std::cerr << "Failed to load texture" << std::endl;
    }

    stbi_image_free(data);

    std::shared_ptr<VertexArray> VAO = std::make_shared<VertexArray>();

    std::shared_ptr<VertexBuffer> vb = std::make_shared<VertexBuffer>(vertices, sizeof(vertices));
    vb->SetLayout(
        {
            {ShaderUtility::ShaderDataType::Float3, "aPos"},
            {ShaderUtility::ShaderDataType::Float2, "aTexCoord"}
        });

    VAO->AddVertexBuffer(vb);

    Shader defaultShader("shaders/shader.vert", "shaders/shader.frag");

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, textures[0]);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, textures[1]);

    defaultShader.Bind();
    defaultShader.SetInt("texture1", 0);
    defaultShader.SetInt("texture2", 1);

    glClearColor(0.3f, 0.6f, 0.6f, 1.0f);

    glEnable(GL_DEPTH_TEST);

    glm::vec3 translationVector = glm::vec3(0.0f, 0.0f, 0.0f);
    glm::vec3 rotationVector = glm::vec3(0.0f, 0.0f, 0.0f);
    glm::vec3 scaleVector = glm::vec3(1.0f, 1.0f, 1.0f);
    while (!glfwWindowShouldClose(window))
    {
        float currentFrame = static_cast<float>(glfwGetTime());
        g_DeltaTime = currentFrame - g_LastFrameTime;
        g_LastFrameTime = currentFrame;


        glfwPollEvents();
        Input::Process(window);

        glm::vec3 CurrentCameraPosition = g_Camera.GetPosition();
        CurrentCameraPosition.y = 0.0f;
        g_Camera.SetPosition(CurrentCameraPosition);

        glm::mat4 view = glm::mat4(1.0f);
        view = g_Camera.GetViewMatrix();
        defaultShader.SetMat4("view", view);

        glm::mat4 projection = glm::perspective(glm::radians(g_Camera.GetZoom()), g_WindowWidth / (float)g_WindowWidth, 0.1f, 100.0f);
        defaultShader.SetMat4("projection", projection);

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        for (unsigned int i = 0; i < 10; i++)
        {
            glm::mat4 model = glm::mat4(1.0f);
            model = glm::translate(model, translationVector);
            model = glm::translate(model, cubePositions[i]);
            //float angle = 20.0f * i;
            //model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
            model = glm::rotate(model, glm::radians(rotationVector.x), glm::vec3(1.0f, 0.0f, 0.0f));
            model = glm::rotate(model, glm::radians(rotationVector.y), glm::vec3(0.0f, 1.0f, 0.0f));
            model = glm::rotate(model, glm::radians(rotationVector.z), glm::vec3(0.0f, 0.0f, 1.0f));
            model = glm::scale(model, scaleVector);
            defaultShader.SetMat4("model", model);
            glDrawArrays(GL_TRIANGLES, 0, 36);
        }

        ImGuiWrapper::NewFrame();

        ImGui::Begin("Cubes");
        ImGui::SliderFloat("Translate X", &translationVector.x, -1.0f, 1.0f);
        ImGui::SliderFloat("Translate Y", &translationVector.y, -1.0f, 1.0f);
        ImGui::SliderFloat("Translate Z", &translationVector.z, -1.0f, 1.0f);

        ImGui::SliderFloat("Rotate X", &rotationVector.x, 0.0f, 360.0f);
        ImGui::SliderFloat("Rotate Y", &rotationVector.y, 0.0f, 360.0f);
        ImGui::SliderFloat("Rotate Z", &rotationVector.z, 0.0f, 360.0f);

        ImGui::SliderFloat("Scale X", &scaleVector.x, 0.1f, 10.0f);
        ImGui::SliderFloat("Scale Y", &scaleVector.y, 0.1f, 10.0f);
        ImGui::SliderFloat("Scale Z", &scaleVector.z, 0.1f, 10.0f);

        ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGuiWrapper::GetIO()->Framerate, ImGuiWrapper::GetIO()->Framerate);
        ImGui::End();

        ImGuiWrapper::EndFrameAndRender();

        glfwSwapBuffers(window);
    }

    ImGuiWrapper::Cleanup();

    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}