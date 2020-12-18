import QtQuick 2.12
import QtQuick.Window 2.12
import QtQuick.Controls 2.12 as Controls
import QtQuick.Layouts 1.12
import QtQuick.Dialogs 1.0 as Dialogs
import org.gmn.viewcontroller 1.0

Dialogs.FileDialog {
    id: imageChooseDialog
    nameFilters: ["Images (*.png *.xpm *.jpg *.jpeg *.bmp *.gif *.pgb *.pgm *.ppm *.xbm *.xpm)", "All (*)"]
    onAccepted: {
        
    }
    
}
