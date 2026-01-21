/*
    SPDX-FileCopyrightText: 2024 WayMCA Contributors
    SPDX-License-Identifier: GPL-3.0-or-later
*/

#include "waymca.h"

#include <core/renderviewport.h>
#include <effect/effecthandler.h>
#include <effect/effectwindow.h>
#include <opengl/glshader.h>
#include <opengl/glshadermanager.h>

#include <QAction>
#include <QFile>
#include <QStandardPaths>
#include <QTextStream>

#include <KConfig>
#include <KConfigGroup>
#include <KGlobalAccel>

namespace KWin
{

WaymcaEffect::WaymcaEffect()
    : OffscreenEffect()
{
    reconfigure(ReconfigureAll);
    
    // Register global shortcut
    QAction *toggleAction = new QAction(this);
    toggleAction->setObjectName(QStringLiteral("ToggleWaymca"));
    toggleAction->setText(QStringLiteral("Toggle WayMCA Effect"));
    KGlobalAccel::self()->setDefaultShortcut(toggleAction, QList<QKeySequence>());
    KGlobalAccel::self()->setShortcut(toggleAction, QList<QKeySequence>());
    connect(toggleAction, &QAction::triggered, this, &WaymcaEffect::toggleEffect);
}

WaymcaEffect::~WaymcaEffect() = default;

bool WaymcaEffect::supported()
{
    return effects->isOpenGLCompositing();
}

bool WaymcaEffect::enabledByDefault()
{
    return supported();
}

void WaymcaEffect::reconfigure(ReconfigureFlags flags)
{
    Q_UNUSED(flags)

    KConfig config(QStringLiteral("kwinrc"));
    KConfigGroup conf(&config, QStringLiteral("Effect-waymca"));
    
    m_greenBlurRadius = conf.readEntry("GreenBlurRadius", 3);
    m_blueBlurRadius = conf.readEntry("BlueBlurRadius", 8);
    m_useGaussianBlur = conf.readEntry("UseGaussianBlur", true);
    m_fullScreenBlurMode = conf.readEntry("FullScreenBlur", false);
    m_fullScreenBlurRadius = conf.readEntry("FullScreenBlurRadius", 10);

    loadShader();
}

void WaymcaEffect::loadShader()
{
    m_valid = false;
    m_shader.reset();

    if (!supported()) {
        return;
    }

    // Load shader from file
    const QString fragmentPath = QStandardPaths::locate(
        QStandardPaths::GenericDataLocation,
        QStringLiteral("kwin/effects/waymca/shaders/waymca.frag"));

    if (fragmentPath.isEmpty()) {
        qWarning() << "WayMCA: Could not find shader file";
        return;
    }

    QFile file(fragmentPath);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qWarning() << "WayMCA: Could not open shader file:" << fragmentPath;
        return;
    }

    QTextStream stream(&file);
    const QString fragmentSource = stream.readAll();
    file.close();

    m_shader = ShaderManager::instance()->generateCustomShader(ShaderTrait::MapTexture, QByteArray(), fragmentSource.toUtf8());

    if (!m_shader || !m_shader->isValid()) {
        qWarning() << "WayMCA: Failed to load shader";
        m_shader.reset();
        return;
    }

    m_valid = true;
    updateShaderUniforms();
}

void WaymcaEffect::updateShaderUniforms()
{
    if (!m_valid || !m_shader) {
        return;
    }

    ShaderBinder binder(m_shader.get());
    
    m_shader->setUniform("greenBlurRadius", static_cast<float>(m_greenBlurRadius));
    m_shader->setUniform("blueBlurRadius", static_cast<float>(m_blueBlurRadius));
    m_shader->setUniform("useGaussianBlur", m_useGaussianBlur ? 1.0f : 0.0f);
    m_shader->setUniform("fullScreenBlur", m_fullScreenBlurMode ? 1.0f : 0.0f);
    m_shader->setUniform("fullScreenBlurRadius", static_cast<float>(m_fullScreenBlurRadius));
}

void WaymcaEffect::prePaintScreen(ScreenPrePaintData &data, std::chrono::milliseconds presentTime)
{
    if (m_valid) {
        const auto windows = effects->stackingOrder();
        for (EffectWindow *window : windows) {
            if (window->isOnCurrentDesktop() && !window->isMinimized()) {
                redirect(window);
            }
        }
    }

    effects->prePaintScreen(data, presentTime);
}

void WaymcaEffect::drawWindow(const RenderTarget &renderTarget, const RenderViewport &viewport,
                              EffectWindow *window, int mask, const QRegion &region,
                              WindowPaintData &data)
{
    if (m_valid && m_shader) {
        setShader(window, m_shader.get());
        updateShaderUniforms();
    }

    effects->drawWindow(renderTarget, viewport, window, mask, region, data);

    if (m_valid) {
        unredirect(window);
    }
}

bool WaymcaEffect::isActive() const
{
    return m_valid && effects->isOpenGLCompositing();
}

int WaymcaEffect::requestedEffectChainPosition() const
{
    return 98; // Just before the final composition
}

void WaymcaEffect::toggleEffect()
{
    // Toggle the effect on/off
    if (m_valid) {
        m_valid = false;
    } else {
        loadShader();
    }
    effects->addRepaintFull();
}

} // namespace KWin
