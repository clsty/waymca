import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import org.kde.kcmutils as KCM

KCM.SimpleKCM {
    id: root
    
    property alias cfg_GreenBlurRadius: greenBlurSlider.value
    property alias cfg_BlueBlurRadius: blueBlurSlider.value
    property alias cfg_UseGaussianBlur: gaussianCheckBox.checked
    
    ColumnLayout {
        spacing: 20
        
        GroupBox {
            title: "Blur Settings"
            Layout.fillWidth: true
            
            ColumnLayout {
                spacing: 10
                anchors.fill: parent
                
                // Green channel blur
                Label {
                    text: "Green Channel Blur Radius: " + greenBlurSlider.value
                }
                
                RowLayout {
                    Label {
                        text: "0"
                    }
                    
                    Slider {
                        id: greenBlurSlider
                        Layout.fillWidth: true
                        from: 0
                        to: 20
                        stepSize: 1
                        snapMode: Slider.SnapAlways
                    }
                    
                    Label {
                        text: "20"
                    }
                }
                
                // Blue channel blur
                Label {
                    text: "Blue Channel Blur Radius: " + blueBlurSlider.value
                }
                
                RowLayout {
                    Label {
                        text: "0"
                    }
                    
                    Slider {
                        id: blueBlurSlider
                        Layout.fillWidth: true
                        from: 0
                        to: 20
                        stepSize: 1
                        snapMode: Slider.SnapAlways
                    }
                    
                    Label {
                        text: "20"
                    }
                }
                
                // Gaussian blur option
                CheckBox {
                    id: gaussianCheckBox
                    text: "Use Gaussian Blur"
                }
            }
        }
        
        Label {
            text: "The WayMCA effect simulates myopic chromatic aberration by blurring the green and blue color channels while keeping red sharp. This may help with myopia control during computer work."
            wrapMode: Text.WordWrap
            Layout.fillWidth: true
        }
        
        Item {
            Layout.fillHeight: true
        }
    }
}
