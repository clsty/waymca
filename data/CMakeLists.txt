# Install shader files
install(FILES shaders/waymca.frag
        DESTINATION ${KDE_INSTALL_DATADIR}/kwin/effects/waymca/shaders)

# Install metadata
install(FILES waymca.json
        DESTINATION ${KDE_INSTALL_DATADIR}/kwin/effects)
