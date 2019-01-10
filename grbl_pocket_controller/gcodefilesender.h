#ifndef GCODEFILESENDER_H
#define GCODEFILESENDER_H

#include <QString>

class GCodeFileSender
{
public:
    GCodeFileSender();
    void set_filename(QString filename);
    void start();
    void pause();
    void stop();

private:
    QString mFilename;

};

#endif // GCODEFILESENDER_H
