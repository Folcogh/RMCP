/*
 * RMCP - a short program cleaning files mistakenly copied
 * Copyright (C) 2020 Martial Demolins AKA Folco
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 *
 * mail: martial <dot> demolins <at> gmail <dot> com
 */

#include "RMCP.hpp"
#include <QCommandLineOption>
#include <QCommandLineParser>
#include <QCoreApplication>
#include <QDir>
#include <QFile>
#include <QFileInfo>
#include <QFileInfoList>
#include <QString>
#include <QStringList>
#include <QTextStream>

int main(int argc, char *argv[])
{
    // Create output streams
    QTextStream Stdin(stdin);
    QTextStream Stdout(stdout);
    QTextStream Stderr(stderr);

    // Preamble
    Stdout << APPLICATION_NAME " for YBT by MDS\n"
                               "martial.demolins@gmail.com\n"
                               "License: GPL v3\n"
                               "Type " EXECUTABLE_NAME " -h for help\n"
           << endl;

    // Setup application
    QCoreApplication App(argc, argv);
    App.setApplicationName(APPLICATION_NAME);
    App.setApplicationVersion(VERSION_STR);

    // Prepare and parse command line
    QCommandLineParser Parser;
    Parser.addHelpOption();
    Parser.addVersionOption();
    Parser.addOption({{OPTION_PATTERN_SHORT_STR, OPTION_PATTERN_LONG_STR}, "specify the string which triggers file deletion", "pattern"});
    Parser.addOption({{OPTION_DIRECTORY_SHORT_STR, OPTION_DIRECTORY_LONG_STR},
                      "Set the root directory of the cleaning process. Use the current directory if unspecified",
                      "directory"});
    Parser.setApplicationDescription("\n" APPLICATION_NAME " is intended to clean your OneDrive when you mess up your new laptop ;)\n"
                                     "\n"
                                     "It deletes recursively files whose name contains a pattern string\n"
                                     "Default string is ' - Copie.'\n"
                                     "You can specify another string with the 'pattern' option\n"
                                     "You can change the root directory of the process with the 'directory' option");
    Parser.process(App);

    // Check if a custom directory has been set
    // Default directory is the current one
    QDir BaseDir(QDir::current());
    if (Parser.isSet(OPTION_DIRECTORY_SHORT_STR)) {
        BaseDir.setPath(Parser.value(OPTION_DIRECTORY_SHORT_STR));

        // Check that the custom directory really exists
        if (!BaseDir.exists()) {
            Stderr << "The specified directory doesn't exist: " << BaseDir.absolutePath() << endl;
            return ERROR_INVALID_DIRECTORY;
        }
    }

    // Check if a custom pattern string has been set
    QString Pattern = DEFAULT_PATTERN_STRING;
    if (Parser.isSet(OPTION_PATTERN_SHORT_STR)) {
        Pattern = Parser.value(OPTION_PATTERN_SHORT_STR);
    }

    // Get the list of files to delete
    QStringList Candidates;
    parseDirectory(BaseDir, QString("*%1*").arg(Pattern), Candidates);

    //  Process files
    if (Candidates.isEmpty()) {
        Stdout << "No file found, nothing to do" << endl;
    }
    else {
        // Print file list
        for (int i = 0; i < Candidates.count(); i++) {
            Stdout << Candidates.at(i) << endl;
        }

        // Ask a confirmation from the user
        Stdout << endl << QString("Do you want to delete these %1 files? [Y/N]").arg(Candidates.count()) << endl;
        QString Input = Stdin.readLine(1);

        // Process depends on confirmation
        if ((Input == "y") || (Input == "Y")) {
            unsigned int Fail = 0;
            for (int i = 0; i < Candidates.count(); i++) {
                QString Filename = Candidates.at(i);
                Stdout << "Deleting file: " << Filename << endl;
                if (!QFile::remove(Filename)) {
                    Stderr << "Couldn't remove file:" << Filename << endl;
                    Fail ++;
                }
            }
            
            // Print process summary
            if (Fail == 0) {
                Stdout << "All files deleted" << endl;
            }
            else {
                Stdout << QString("%1 files couldn't be deleted").arg(Fail) << endl;
            }
        }
        else {
            Stdout << "Process cancelled by user" << endl;
        }
    }

    return NO_ERROR;
}

void parseDirectory(QDir directory, QString filter, QStringList& candidates)
{
    // Prepare directory parsing
    directory.setNameFilters({filter});
    directory.setSorting(QDir::DirsLast);
    directory.setFilter(QDir::AllEntries | QDir::AllDirs | QDir::NoDotAndDotDot);

    // Read directory
    QFileInfoList Entries = directory.entryInfoList();
    for (int i = 0; i < Entries.count(); i++) {
        QFileInfo FileInfo = Entries.at(i);
        if (FileInfo.isFile()) {
            candidates << FileInfo.absoluteFilePath();
        }
        else {
            directory.setPath(FileInfo.absoluteFilePath());
            parseDirectory(directory, filter, candidates);
        }
    }
}
