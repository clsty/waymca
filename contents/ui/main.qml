import QtQuick
import org.kde.kwin

Item {
    id: waymcaEffect
    
    // Load configuration values
    property int greenRadius: KWin.readConfig("GreenBlurRadius", 3)
    property int blueRadius: KWin.readConfig("BlueBlurRadius", 8)
    property bool useGaussian: KWin.readConfig("UseGaussianBlur", true)
    property bool fullScreenBlurMode: KWin.readConfig("FullScreenBlur", false)
    property int fullScreenRadius: KWin.readConfig("FullScreenBlurRadius", 10)
    
    // Shader properties
    ShaderEffectSource {
        id: screenSource
        sourceItem: null
        hideSource: false
    }
    
    ShaderEffect {
        id: shader
        anchors.fill: parent
        
        property variant source: screenSource
        property real greenBlurRadius: waymcaEffect.greenRadius
        property real blueBlurRadius: waymcaEffect.blueRadius
        property real useGaussianBlur: waymcaEffect.useGaussian ? 1.0 : 0.0
        property real fullScreenBlur: waymcaEffect.fullScreenBlurMode ? 1.0 : 0.0
        property real fullScreenBlurRadius: waymcaEffect.fullScreenRadius
        
        fragmentShader: "file://" + KWin.readConfig("shaderPath", "") + "waymca.frag"
    }
}
