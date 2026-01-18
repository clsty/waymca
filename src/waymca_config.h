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

#ifndef WAYMCA_CONFIG_H
#define WAYMCA_CONFIG_H

#include <KCModule>

namespace Ui
{
class WayMCAConfig;
}

namespace KWin
{

class WayMCAEffectConfig : public KCModule
{
    Q_OBJECT

public:
    explicit WayMCAEffectConfig(QObject *parent, const KPluginMetaData &data);
    ~WayMCAEffectConfig() override;

    void save() override;

private:
    Ui::WayMCAConfig *m_ui;
};

} // namespace KWin

#endif // WAYMCA_CONFIG_H
