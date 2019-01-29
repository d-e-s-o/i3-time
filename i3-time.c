// i3-time.c

/***************************************************************************
 *   Copyright (C) 2017 Daniel Mueller (deso@posteo.net)                   *
 *                                                                         *
 *   This program is free software: you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation, either version 3 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program.  If not, see <http://www.gnu.org/licenses/>. *
 ***************************************************************************/

#include <errno.h>
#include <stdbool.h>
#include <stdio.h>
#include <time.h>
#include <unistd.h>


void printTime(void)
{
  char datetime[26];
  struct tm tm;
  struct tm *result;
  size_t size;
  ssize_t written;
  time_t seconds = time(NULL);

  result = localtime_r(&seconds, &tm);
  if (result == NULL) {
    perror("localtime_r");
    return;
  }

  size = strftime(datetime, sizeof(datetime), "%a %b %e, %H:%M", &tm);
  if (size <= 0) {
    fprintf(stderr, "strftime: insufficient buffer size");
    return;
  }

  do {
    written = write(STDOUT_FILENO, datetime, size);
  } while (written < 0 && errno == EAGAIN);

  if (written < 0) {
    perror("write");
  }
}


int main()
{
  while (true) {
    unsigned int slept = 60;
    printTime();
    do {
      slept = sleep(slept);
    } while (slept > 0);
  }
}
