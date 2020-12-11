#ifndef GMNPATHS_H
#define GMNPATHS_H

#include <QStandardPaths>
#include <QString>


namespace GMN {
    const QString campaignsFolder = QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation) + "/" + "GMNCampaigns";
    const QString imagesFolderName = "images/";
    const QString charactersFileName = "characters.json";
    const QString infoFileName = "info.json";
    const QString defaultCharacterImagePath = ":/images/nopic.png";
}

#endif // GMNPATHS_H
