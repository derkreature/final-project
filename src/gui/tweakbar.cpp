#include <easylogging.h>

#include "gui/tweakbar.h"

#include <FreeImage.h>
#include <stdexcept>

namespace gui
{
    // Use this for button callbacks, pass the bool
    static void TW_CALL btn_cb(void *user_data)
    {
        *((bool*)user_data) = true;
    }

    basic_bar::basic_bar(const std::string& name)
    {
        if (!(m_bar = TwNewBar((m_name = name).c_str()))) {
            LOG(ERROR) << "Failed to create tweakbar.";
            LOG(TRACE) << "TwNewBar failed.";
            throw std::runtime_error("");
        }
    }

    basic_bar::~basic_bar()
    {
        TwDeleteBar(m_bar);
    }

    void basic_bar::set_title(const std::string& title)
    {
        TwDefine((m_name + " label='" + title + "'").c_str());
    }

    void basic_bar::refresh()
    {
        TwRefreshBar(m_bar);
    }

    main_bar::main_bar(const std::string& name) : basic_bar(name)
    {
        rotation = 0;
        exposure = 0.18f;
        cam_move_speed = 3.5f;
        cam_sensitivity = 1.5f;
        cam_fov = 50;
        color = glm::vec3(0.75f, 0.25f, 0.25f);
        aperture_regen_btn = false;

        TwAddVarRW(m_bar,
            "rotation", TW_TYPE_FLOAT, &rotation,
            " label='Rotation'"
            " min=0 max=360 step=1.0 help='Rotate the skeleton'");

        TwAddVarRW(m_bar,
            "exposure", TW_TYPE_FLOAT, &exposure,
            " label='Exposure'"
            " min=0.01 max=3 step=0.01 help='Tonemapping exposure'");

        TwAddVarRW(m_bar,
            "color", TW_TYPE_COLOR3F, &color,
            " label='Color' colormode=hls");

        TwAddButton(m_bar, "btn", btn_cb, &aperture_regen_btn,
            " label='New Aperture'");

        TwAddVarRW(m_bar,
            "cam_move_speed", TW_TYPE_FLOAT, &cam_move_speed,
            " label='Camera Speed' group='Navigation'"
            " min=0.1 max=5 step=0.1 help='Movement speed (WASD)'");

        TwAddVarRW(m_bar,
            "cam_sensitivity", TW_TYPE_FLOAT, &cam_sensitivity,
            " label='Sensitivity' group='Navigation'"
            " min=0.01 max=2.5 step=0.01 help='Rotation sensitivity'");

        TwAddVarRW(m_bar,
            "cam_fov", TW_TYPE_FLOAT, &cam_fov,
            " label='Field of View' group='Navigation'"
            " min=10 max=90 step=0.1 help='Camera field of view'");

        TwAddVarRO(m_bar,
            "cam_locked", TW_TYPE_BOOLCPP, &cam_locked,
            " label='Cursor Locked' group='Navigation'"
            " help='Right-click to lock or unlock cursor'");

        TwDefine((m_name + " contained=true").c_str());
    }
}
