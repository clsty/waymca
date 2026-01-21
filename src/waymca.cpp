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
    
    // Connect window signals
    connect(effects, &EffectsHandler::windowAdded, this, &WaymcaEffect::slotWindowAdded);
    connect(effects, &EffectsHandler::windowDeleted, this, &WaymcaEffect::slotWindowDeleted);
    
    // Apply effect to all existing windows
    if (m_valid) {
        const auto windows = effects->stackingOrder();
        for (EffectWindow *window : windows) {
            applyEffect(window);
        }
    }
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

    // If we had a shader before, we need to reload and reapply
    const bool hadValidShader = m_valid;
    
    // Remove effect from all windows before reloading shader
    if (hadValidShader) {
        for (EffectWindow *w : m_windows) {
            unredirect(w);
        }
        m_windows.clear();
    }
    
    loadShader();
    
    // Reapply effect to all windows
    if (m_valid) {
        const auto windows = effects->stackingOrder();
        for (EffectWindow *window : windows) {
            applyEffect(window);
        }
        effects->addRepaintFull();
    }
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
    m_inited = true;
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

void WaymcaEffect::applyEffect(EffectWindow *window)
{
    if (!m_valid || !m_shader) {
        return;
    }
    
    if (m_windows.contains(window)) {
        return;
    }
    
    redirect(window);
    setShader(window, m_shader.get());
    m_windows.insert(window);
}

void WaymcaEffect::removeEffect(EffectWindow *window)
{
    if (auto it = m_windows.find(window); it != m_windows.end()) {
        unredirect(window);
        m_windows.erase(it);
    }
}

void WaymcaEffect::slotWindowAdded(KWin::EffectWindow *w)
{
    if (m_valid) {
        applyEffect(w);
    }
}

void WaymcaEffect::slotWindowDeleted(KWin::EffectWindow *w)
{
    if (auto it = m_windows.find(w); it != m_windows.end()) {
        m_windows.erase(it);
    }
}

void WaymcaEffect::drawWindow(const RenderTarget &renderTarget, const RenderViewport &viewport,
                              EffectWindow *window, int mask, const QRegion &region,
                              WindowPaintData &data)
{
    // The shader is already bound via setShader() in applyEffect()
    // Just call the base class implementation which will apply the shader
    OffscreenEffect::drawWindow(renderTarget, viewport, window, mask, region, data);
}

bool WaymcaEffect::isActive() const
{
    return m_valid && !m_windows.empty();
}

int WaymcaEffect::requestedEffectChainPosition() const
{
    return 98; // Just before the final composition
}

void WaymcaEffect::toggleEffect()
{
    if (m_valid) {
        // Turn off the effect
        for (EffectWindow *w : m_windows) {
            unredirect(w);
        }
        m_windows.clear();
        m_valid = false;
    } else {
        // Turn on the effect
        loadShader();
        if (m_valid) {
            const auto windows = effects->stackingOrder();
            for (EffectWindow *window : windows) {
                applyEffect(window);
            }
        }
    }
    effects->addRepaintFull();
}

} // namespace KWin
