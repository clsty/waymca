/*
    SPDX-FileCopyrightText: 2024 WayMCA Contributors
    SPDX-License-Identifier: GPL-3.0-or-later
*/

#pragma once

#include <effect/offscreeneffect.h>

namespace KWin
{

class GLShader;

class WaymcaEffect : public OffscreenEffect
{
    Q_OBJECT

public:
    WaymcaEffect();
    ~WaymcaEffect() override;

    void reconfigure(ReconfigureFlags flags) override;
    void prePaintScreen(ScreenPrePaintData &data, std::chrono::milliseconds presentTime) override;
    void drawWindow(const RenderTarget &renderTarget, const RenderViewport &viewport,
                    EffectWindow *window, int mask, const QRegion &region,
                    WindowPaintData &data) override;

    bool isActive() const override;
    int requestedEffectChainPosition() const override;

    static bool supported();
    static bool enabledByDefault();

public Q_SLOTS:
    void toggleEffect();

private:
    void loadShader();
    void updateShaderUniforms();
    
    std::unique_ptr<GLShader> m_shader;
    
    // Configuration values
    int m_greenBlurRadius = 3;
    int m_blueBlurRadius = 8;
    bool m_useGaussianBlur = true;
    bool m_fullScreenBlurMode = false;
    int m_fullScreenBlurRadius = 10;
    
    bool m_valid = false;
    bool m_inited = false;
};

} // namespace KWin
