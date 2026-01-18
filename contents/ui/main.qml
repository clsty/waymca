import QtQuick
import org.kde.kwin

FullScreenEffect {
    id: waymcaEffect
    
    // Load configuration values
    property int greenRadius: KWin.readConfig("GreenBlurRadius", 3)
    property int blueRadius: KWin.readConfig("BlueBlurRadius", 8)
    property bool useGaussian: KWin.readConfig("UseGaussianBlur", true)
    
    // Shader effect to apply the chromatic aberration
    FullScreenShaderEffect {
        id: shader
        
        property real greenBlurRadius: waymcaEffect.greenRadius
        property real blueBlurRadius: waymcaEffect.blueRadius
        property real useGaussianBlur: waymcaEffect.useGaussian ? 1.0 : 0.0
        
        fragmentShader: "waymca.frag"
    }
}
