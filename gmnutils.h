#ifndef GMNUTILS_H
#define GMNUTILS_H

#include <QObject>
#include <QFile>
#include <QString>
#include <QPixmap>
#include <QTextStream>

namespace GMN {
    std::optional<QString> loadJsonFile(const QString &filePath) {
        QFile file(filePath);
        bool openResult = file.open(QFile::ReadOnly | QFile::Text);
        if (!openResult) {
            return std::optional<QString>();
        }

        auto fileContent = file.readAll();
        file.close();
        return fileContent;
    }

    bool saveToFile(const QString &path, const QString &content) {
        QFile saveFile(path);
        if (!saveFile.open(QIODevice::WriteOnly | QIODevice::Text)) {
            return false;
        }

        QTextStream out(&saveFile);
        out << content;
        saveFile.close();
        return true;
    }
}

#endif // GMNUTILS_H
