/*
 * WayMCA - Myopic Chromatic Aberration Effect for KWin
 * Copyright (C) 2024 WayMCA Contributors
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

#ifndef WAYMCA_H
#define WAYMCA_H

#include <kwineffects.h>
#include <kwinglutils.h>

namespace KWin
{

class WayMCAEffect : public Effect
{
    Q_OBJECT

public:
    WayMCAEffect();
    ~WayMCAEffect() override;

    static bool supported();
    static bool enabledByDefault();

    void reconfigure(ReconfigureFlags flags) override;
    void prePaintScreen(ScreenPrePaintData &data, std::chrono::milliseconds presentTime) override;
    void paintScreen(const RenderTarget &renderTarget, const RenderViewport &viewport, int mask, const QRegion &region, Output *screen) override;
    bool isActive() const override;

public Q_SLOTS:
    void slotWindowAdded(EffectWindow *w);
    void slotWindowDeleted(EffectWindow *w);

private:
    void loadShader();
    void loadConfig();

    std::unique_ptr<GLShader> m_shader;
    std::unique_ptr<GLTexture> m_offscreenTexture;
    std::unique_ptr<GLFramebuffer> m_offscreenTarget;

    int m_greenBlurRadius;
    int m_blueBlurRadius;
    bool m_useGaussianBlur;
    bool m_valid;
};

} // namespace KWin

#endif // WAYMCA_H
