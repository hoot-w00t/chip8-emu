#include "chip8/emulator.h"
#include "logger.h"
#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>

void print_help(const char *cmd)
{
    printf("Usage: %s [-h] [-l {level}] filename\n\n", cmd);
    printf("Description:\nPositionnal argument:\n");
    printf("    filename     path to the Chip8 ROM to emulate\n\n");
    printf("Optionnal arguments:\n");
    printf("    -l level     logging level (default: warn)\n");
    printf("                 (debug, info, warn, error, critical)\n");
    printf("    -p size      rendered pixel size (default: 8)\n");
    printf("    -c cps       CPU cycles per second (default: 60)\n");
    printf("                 0 means as fast as possible\n");
    printf("    -s           step by step cycle emulation for debugging\n");
}

int main(int ac, char **av)
{
    const char optstring[] = "hl:p:c:s";
    int opt, pixel_size = 8, cps = 60;
    bool step = false;

    while ((opt = getopt(ac, av, optstring)) != -1) {
        switch (opt) {
            case 'h':
                print_help(av[0]);
                return EXIT_SUCCESS;

            case 'l':
                if (set_logger_level(optarg) < 0) {
                    fprintf(stderr, "Invalid logging level: %s\n", optarg);
                    return EXIT_FAILURE;
                }
                break;

            case 'p':
                pixel_size = atoi(optarg);
                if (pixel_size <= 0) {
                    fprintf(stderr, "Invalid pixel size: %s\n", optarg);
                    return EXIT_FAILURE;
                }
                break;

            case 'c':
                cps = atoi(optarg);
                if (cps < 0) {
                    fprintf(stderr, "Invalid cycles per second: %s\n", optarg);
                    return EXIT_FAILURE;
                }
                break;

            case 's':
                step = true;
                break;

            default: return EXIT_FAILURE;
        }
    }

    if (optind >= ac) {
        fprintf(stderr, "Expected one positionnal argument: filename\n");
        return EXIT_FAILURE;
    }

    return emulate_chip8_program(av[optind], cps, pixel_size, step);
}