#pragma once

#include <raindance/Core/Headers.hh>
#include <raindance/Core/Camera/Camera.hh>
#include <OVR.h>

class OculusRift
{
public:

    enum EyeType
    {
        LEFT,
        RIGHT
    };

    OculusRift()
    {
        ovr_Initialize();

        m_HMD = ovrHmd_Create(0);

        if (!m_HMD)
        {
            LOG("[OVR] Unable to create HMD: %s\n", ovrHmd_GetLastError(NULL));
            return;
        }

        if (!ovrHmd_ConfigureTracking(m_HMD, ovrTrackingCap_Orientation | ovrTrackingCap_MagYawCorrection | ovrTrackingCap_Position, 0))
        {
            LOG("[OVR] Unable to locate Rift sensor device!\n");
            return;
        }

        m_Camera.setPerspectiveProjection(74, (0.5f * (float)m_HMD->Resolution.w) / (float)m_HMD->Resolution.h, 0.1f, 1024.0f);
    }

    virtual ~OculusRift()
    {
        ovr_Shutdown();
    }

    GLFWmonitor* findMonitor()
    {   
        int count;
        GLFWmonitor** monitors = glfwGetMonitors(&count);

        for (int i = 0;  i < count;  i++)
        {
            #if defined(_WIN32)
                if (strcmp(glfwGetWin32Monitor(monitors[i]), m_HMD->DisplayDeviceName) == 0)
                    return monitors[i];
            
            #elif defined(__APPLE__)
                if (glfwGetCocoaMonitor(monitors[i]) == static_cast<unsigned int>(m_HMD->DisplayId))
                    return monitors[i];

            #elif defined(__linux__)
                int xpos, ypos;
                const GLFWvidmode* mode = glfwGetVideoMode(monitors[i]);
                glfwGetMonitorPos(monitors[i], &xpos, &ypos);
                if (m_HMD->WindowsPos.x == xpos &&
                    m_HMD->WindowsPos.y == ypos &&
                    m_HMD->Resolution.w == mode->width &&
                    m_HMD->Resolution.h == mode->height)
                    return monitors[i];
            #endif
        }
    
        LOG("[OVR] No HMD monitor found!\n");
        return NULL;
    }

    void idle()
    {
        ovrTrackingState ts  = ovrHmd_GetTrackingState(m_HMD, ovr_GetTimeInSeconds());

        if (ts.StatusFlags & (ovrStatus_OrientationTracked | ovrStatus_PositionTracked))
        {
            m_HeadPosition = toGLM(ts.HeadPose.ThePose.Position);

            m_HeadOrientation = glm::normalize(toGLM(ts.HeadPose.ThePose.Orientation));
            m_HeadOrientation = glm::rotate(m_HeadOrientation, (float) M_PI, glm::vec3(0, 0, 1));   
        }
    }

    void lookAt(EyeType type, const glm::vec3& position, const glm::vec3& target, const glm::vec3& up, Camera& camera)
    {
        camera.lookAt(position, target, up);
        camera.rotate(m_HeadOrientation);

        if (false)
        {
            float ipd = 0.20; // Inter-Pupillary Distance

            glm::vec3 step = 0.5f * ipd * camera.right();

            if (type == LEFT)
                camera.move(-step);
            else
                camera.move(step);
        }
    }

    glm::vec3 toGLM(ovrVector3f v) { return glm::vec3(v.x, v.y, v.z); }
    glm::quat toGLM(ovrQuatf q) { return glm::quat(q.z, -q.y, q.x, q.w); }

private:
    ovrHmd m_HMD;
    Camera m_Camera;

    glm::vec3 m_Position;

    glm::vec3 m_HeadPosition;
    glm::quat m_HeadOrientation;
};