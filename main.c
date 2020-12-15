#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <ctype.h>
#include <string.h>
#include <sys/types.h>

char const * file_name = "./record.txt";

typedef struct students students;
struct students {
	char name[20];
  unsigned SID;
  float CGPA;
};

void collect(void);
void report(void);
void seek_sid(void);
void seek_cgpa(bool);

char * strip(char str[], size_t len);

int main() {
  printf("Mujtaba Inayat (11341) - CEP \n");

  bool looping = true;
  while (looping) {
    unsigned short opt;
    printf("\n"
           "1 : Write to file\n"
           "2 : Read from File\n"
           "3 : Locate student by SID\n"
           "4 : Find students with CGPA .LE. given value\n"
           "5 : Find students with CGPA .GE. given value\n"
           "0 : Exit\n");
    scanf("%hu", &opt);

    switch (opt) {
      case 1:
        collect();
        break;

      case 2:
        report();
        break;

      case 3:
        seek_sid();
        break;

      case 4:
        seek_cgpa(false);
        break;

      case 5:
        seek_cgpa(true);
        break;

      case 0:
        puts("All done.\n");
        looping = false;
        break;

      default:
        printf("%hu is an invalid choice\n\n", opt);
        break;
    }
  }

	return EXIT_SUCCESS;
}

void collect(void) {
  FILE * fptr;
  students info;
  unsigned num;
  fptr = fopen(file_name, "a");
  if (fptr == NULL) {
    printf("File does not exist.\n");
    exit(EXIT_FAILURE);
  }

  printf("How many records do you want to write : ");
  scanf("%u", &num);

  for(size_t i = 0; i < num; i++) {
    bool valid = true;
    printf("Enter Student ID: ");
//        fflush(stdin); /* fflush not valid for input streams */
    scanf("%u", &info.SID);
    int cc;
    while ((cc = getchar()) != '\n');
    if (info.SID > 99999U) {
      printf("%u exceeds maximum SID, rejected!\n\n", info.SID);
      valid = false;
      continue;
    }
    printf("Enter Name: ");
//        fflush(stdin); /* fflush not valid for input streams */
//    while ((cc = getchar()) != '\n');

    memset(info.name, '\0', 20);
    fgets(info.name, 20, stdin);
#define AS_DEBUG__ 1
#undef AS_DEBUG__
#ifdef AS_DEBUG__
    printf("> %s\n", info.name);
    for (size_t x_ = 0; x_ < 20; ++x_) {
      printf("%02x ", (unsigned char) info.name[x_]);
    }
    putchar('\n');
#endif

    strip(info.name, 20);

#ifdef AS_DEBUG__
    printf("> %s\n", info.name);
    for (size_t x_ = 0; x_ < 20; ++x_) {
      printf("%02x ", (unsigned char) info.name[x_]);
    }
    putchar('\n');
#endif
    if (strlen(info.name) <= 0) {
      puts("A name is required\n");
      valid = false;
      continue;
    }

    printf("Enter CGPA: ");
    scanf("%f", &info.CGPA);
    if (info.CGPA >= 1000.00 || info.CGPA < 0.0) {
      printf("A CGPA of %.2f is outside of acceptable range: rejected!\n\n",
      info.CGPA);
      valid = false;
      continue;
    }

    if (valid) {
      fprintf(fptr, "%05d %6.2f %s\n",
              info.SID, info.CGPA, info.name);
      putchar('\n');
    }
  }
  fclose(fptr);

  return;
}

void report(void) {
  FILE * fptr;
  students info;
  fptr = fopen(file_name, "r");
  if (fptr == NULL) {
    printf("File does not exist.\n");
    exit(EXIT_FAILURE);
  }
  else {
    rewind(fptr);
  }

  while (!feof(fptr)) {
    memset(info.name, '\0', sizeof info);
    fscanf(fptr, "%u ", &info.SID);
    fscanf(fptr, "%f ", &info.CGPA);

    fgets(info.name, 20, fptr);
    if (strlen(info.name) == 0) {
      continue;
    }
    printf("SID : %5u\n", info.SID);
    printf("Name: %s\n", strip(info.name, 20));
    printf("CGPA: %6.2f\n\n", info.CGPA);
  }
  fclose(fptr);

  return;
}

void seek_sid(void) {
  FILE * fptr;
  students info;

  unsigned seekSID;
  printf("Enter Student ID: ");
  scanf("%u", &seekSID);
  int cc;
  while ((cc = getchar()) != '\n');

  fptr = fopen(file_name, "r");
  if (fptr == NULL) {
    printf("File does not exist.\n");
    exit(EXIT_FAILURE);
  }
  else {
    rewind(fptr);
  }

  bool foundSID = false;
  while (!feof(fptr) && !foundSID) {
    memset(info.name, '\0', sizeof info);
    fscanf(fptr, "%u ", &info.SID);
    fscanf(fptr, "%f ", &info.CGPA);

    fgets(info.name, 20, fptr);
    if (strlen(info.name) == 0) {
      continue;
    }

    if (info.SID == seekSID) {
      foundSID = true;
      printf("SID : %5u\n", info.SID);
      printf("Name: %s\n", strip(info.name, 20));
      printf("CGPA: %6.2f\n\n", info.CGPA);
    }
  }
  fclose(fptr);

  if (!foundSID) {
    printf("Student ID %5u not found.\n", seekSID);
  }
 
  return;
}

void seek_cgpa(bool passing_grade) {

  FILE * fptr;
  students info;

  float cgpa;
  bool valid = true;
  printf("Enter CGPA: ");
  scanf("%f", &cgpa);
  if (info.CGPA >= 1000.00 || info.CGPA < 0.0) {
    printf("A CGPA of %.2f is outside of acceptable range: rejected!\n\n",
    info.CGPA);
    valid = false;
  }

  if (valid) {
    if (passing_grade) {
      printf("CGPA .GE. %6.2f: \n", cgpa);
    }
    else {
      printf("CGPA .LE. %6.2f: \n", cgpa);
    }

    fptr = fopen(file_name, "r");
    if (fptr == NULL) {
      printf("File does not exist.\n");
      exit(EXIT_FAILURE);
    }
    else {
      rewind(fptr);
    }

    while (!feof(fptr)) {
      memset(info.name, '\0', sizeof info);
      fscanf(fptr, "%u ", &info.SID);
      fscanf(fptr, "%f ", &info.CGPA);
      fgets(info.name, 20, fptr);
      strip(info.name, 20);
//      printf(".. %s\n", info.name);
      if (strlen(info.name) == 0) {
        continue;
      }

      bool choose = passing_grade
                  ? info.CGPA >= cgpa
                  : info.CGPA <= cgpa;
//      printf("%s\n", choose ? "true" : "false");
      if (choose) {
        printf("%5u, %6.2f, %s\n",
              info.SID, info.CGPA, strip(info.name, 20));
      }
    }
    fclose(fptr);
  }

  return;
}

char * strip(char * str, size_t len) {
  for (ssize_t i_ = len - 1; i_ >= 0; --i_) {
    if (str[i_] == '\0') { continue; }
    else if (isspace(str[i_])) { str[i_] = '\0'; }
    else { break; }
  }
  return str;
}
