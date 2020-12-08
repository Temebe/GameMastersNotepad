#ifndef GMNUTILS_H
#define GMNUTILS_H

#include <QFile>
#include <QString>

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
}

#endif // GMNUTILS_H
