#include "patterns.h"

Patterns::Patterns()
{

}

Patterns::~Patterns()
{

}

QStringList Patterns::getPatternsNames()
{
    QDir dir("/home/miki/Qt/Projects/GameOfLife/GameOfLife/Patterns");

    QFileInfoList files = dir.entryInfoList();
    QStringList filesNames;
    foreach (QFileInfo file, files)
    {
        if (file.isFile())
        {
            filesNames.append(file.fileName());
        }
    }
    return filesNames;
}

