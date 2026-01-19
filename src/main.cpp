/*
    SPDX-FileCopyrightText: 2024 WayMCA Contributors
    SPDX-License-Identifier: GPL-3.0-or-later
*/

#include "waymca.h"

namespace KWin
{

KWIN_EFFECT_FACTORY_SUPPORTED_ENABLED(WaymcaEffect,
                                      "metadata.json",
                                      return WaymcaEffect::supported();
                                      ,
                                      return WaymcaEffect::enabledByDefault();)

} // namespace KWin

#include "main.moc"
