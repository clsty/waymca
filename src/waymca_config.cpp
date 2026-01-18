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

#include "waymca_config.h"
#include "waymcaconfig.h"
#include "ui_waymca_config.h"

#include <KPluginFactory>
#include <KAboutData>

K_PLUGIN_CLASS(KWin::WayMCAEffectConfig)

namespace KWin
{

WayMCAEffectConfig::WayMCAEffectConfig(QObject *parent, const KPluginMetaData &data)
    : KCModule(parent, data)
    , m_ui(new Ui::WayMCAConfig)
{
    m_ui->setupUi(widget());
    
    addConfig(WayMCAConfig::self(), widget());
}

WayMCAEffectConfig::~WayMCAEffectConfig()
{
    delete m_ui;
}

void WayMCAEffectConfig::save()
{
    KCModule::save();
}

} // namespace KWin

#include "waymca_config.moc"
