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

#ifndef VERSION_HPP
#define VERSION_HPP

#include <QDir>
#include <QString>
#include <QStringList>

// Application
#define VERSION_STR "1.0"
#define APPLICATION_NAME "RemoveCopy"
#define DEFAULT_PATTERN_STRING " - Copie." // Yes, it's in French. Hi Yannis ;)
#define EXECUTABLE_NAME "RMCP"

// Options
#define OPTION_PATTERN_SHORT_STR "p"
#define OPTION_PATTERN_LONG_STR "pattern"
#define OPTION_DIRECTORY_SHORT_STR "d"
#define OPTION_DIRECTORY_LONG_STR "directory"

// Errors
typedef enum { NO_ERROR, ERROR_INVALID_DIRECTORY } ERROR;

// Functions
void parseDirectory(QDir directory, QString filter, QStringList& candidates);

#endif // VERSION_HPP
