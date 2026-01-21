/*
    SPDX-FileCopyrightText: 2024 WayMCA Contributors
    SPDX-License-Identifier: GPL-3.0-or-later
*/

#pragma once

#include <effect/effect.h>

namespace KWin
{

class GLShader;
class GLTexture;
class GLFramebuffer;
class GLVertexBuffer;

class WaymcaEffect : public Effect
{
    Q_OBJECT

public:
    WaymcaEffect();
    ~WaymcaEffect() override;

    void reconfigure(ReconfigureFlags flags) override;
    void prePaintScreen(ScreenPrePaintData &data, std::chrono::milliseconds presentTime) override;
    void paintScreen(const RenderTarget &renderTarget, const RenderViewport &viewport, int mask, const QRegion &region, Output *screen) override;
    void postPaintScreen() override;

    bool isActive() const override;
    int requestedEffectChainPosition() const override;

    static bool supported();
    static bool enabledByDefault();

public Q_SLOTS:
    void toggleEffect();

private:
    void loadShader();
    void updateShaderUniforms();
    bool ensureResources();
    
    std::unique_ptr<GLShader> m_shader;
    std::unique_ptr<GLTexture> m_texture;
    std::unique_ptr<GLFramebuffer> m_fbo;
    
    // Configuration values
    int m_greenBlurRadius = 3;
    int m_blueBlurRadius = 8;
    bool m_useGaussianBlur = true;
    bool m_fullScreenBlurMode = false;
    int m_fullScreenBlurRadius = 10;
    
    bool m_valid = false;
    bool m_inited = false;
    
    // Screen capture
    bool m_captureInProgress = false;
};

} // namespace KWin
