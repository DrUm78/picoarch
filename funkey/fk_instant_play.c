/*
    FK - FunKey retro gaming console library
    Copyright (C) 2020-2021 Vincent Buso
    Copyright (C) 2020-2021 Michel Stempin

    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Lesser General Public
    License as published by the Free Software Foundation; either
    version 2.1 of the License, or (at your option) any later version.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public
    License along with this library; if not, write to the Free Software
    Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA

    Vincent Buso
    vincent.buso@funkey-project.com
    Michel Stempin
    michel.stempin@funkey-project.com
*/

/**
 *  @file FK_Instant_Play.c
 *  This is the Instant Play API for the FunKey retro gaming console library
 */

#include <stdio.h>
#include <unistd.h>
#include "fk_menu.h"
#include "fk_instant_play.h"
#include "core.h"
#include "util.h"

#ifndef SHELL_CMD_POWERDOWN
#define SHELL_CMD_POWERDOWN                 "powerdown"
#define SHELL_CMD_POWERDOWN_HANDLE          "powerdown handle"
#endif

static char *prog_name;

/* Handler for SIGUSR1, caused by closing the console */
static void handle_sigusr1(int signal)
{
    FILE *fp;

    printf("Caught signal USR1(%d)\n", signal);

    /* Exit menu if it was launched */
    FK_EndMenu();

    /* Send command to cancel any previously scheduled powerdown */
    fp = popen(SHELL_CMD_POWERDOWN_HANDLE, "r");
    if (fp == NULL)
    {
        /* Countdown is still ticking, so better do nothing
           than start writing and get interrupted!
        */
        printf("Failed to cancel scheduled shutdown\n");
        exit(0);
    }
    pclose(fp);

    if (in_menu) {
        FK_Suspend();
    } else {
        /* Wait for the core to be ready to save */
        should_suspend = true;
    }
}

void FK_Suspend(void)
{
    FILE *fp;
    char pidcmd[100];

    state_slot = AUTOSAVE_SLOT;
    if(state_write()) {
        printf("Save failed\n");
        state_slot = 0;
    }

    sram_write();
    save_config(CONFIG_TYPE_AUTO);

    PA_INFO("Suspending with %s %s %s %s %s\n", SHELL_CMD_INSTANT_PLAY, "save", prog_name, core_path, content->path);

    snprintf(pidcmd, array_size(pidcmd), "pid record %d", getpid());

    fp = popen(pidcmd, "r");
    if (fp == NULL)
        PA_ERROR("Failed to update stored pid\n");

    pclose(fp);

    fflush(stdout);

    /* Perform Instant Play save and shutdown */
    execlp(SHELL_CMD_INSTANT_PLAY, SHELL_CMD_INSTANT_PLAY, "save", prog_name, core_path, content->path, NULL);

    /* Should not be reached */
    printf("Failed to perform shutdown\n");

    /* Exit application */
    exit(0);
}


void FK_InitInstantPlay(int argc, char **argv)
{
    prog_name = argv[0];
    signal(SIGUSR1, handle_sigusr1);
}

