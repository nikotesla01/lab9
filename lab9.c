#include <stdio.h>
#include <stdlib.h>

// RecordType
struct RecordType {
  int id;
  char name;
  int order;
};

// Fill out this structure
struct HashType {
  int id;
  char name;
  int order;
  int isOccupied;
};

// Compute the hash function
int hash(int x) { return x % 10; }

// parses input file to an integer array
int parseData(char* inputFileName, struct RecordType** ppData) {
  FILE* inFile = fopen(inputFileName, "r");
  int dataSz = 0;
  int i, n;
  char c;
  struct RecordType* pRecord;
  *ppData = NULL;

  if (inFile) {
    fscanf(inFile, "%d\n", &dataSz);
    *ppData = (struct RecordType*)malloc(sizeof(struct RecordType) * dataSz);
    if (*ppData == NULL) {
      printf("Cannot allocate memory\n");
      exit(-1);
    }
    for (i = 0; i < dataSz; ++i) {
      pRecord = *ppData + i;
      fscanf(inFile, "%d ", &n);
      pRecord->id = n;
      fscanf(inFile, "%c ", &c);
      pRecord->name = c;
      fscanf(inFile, "%d ", &n);
      pRecord->order = n;
    }

    fclose(inFile);
  }

  return dataSz;
}

// prints the records
void printRecords(struct RecordType pData[], int dataSz) {
  int i;
  printf("\nRecords:\n");
  for (i = 0; i < dataSz; ++i) {
    printf("\t%d %c %d\n", pData[i].id, pData[i].name, pData[i].order);
  }
  printf("\n\n");
}

// display records in the hash structure
// skip the indices which are free
// the output will be in the format:
// index x -> id, name, order -> id, name, order ....
void displayRecordsInHash(struct HashType* pHashArray, int hashSz) {
  int i;

  for (i = 0; i < hashSz; ++i) {
    if (pHashArray[i].isOccupied) {
      printf("index %d -> %d, %c, %d", i, pHashArray[i].id, pHashArray[i].name,
             pHashArray[i].order);
      for (int j = i + 1; j < hashSz; j++) {
        if (pHashArray[j].isOccupied && pHashArray[j].id == pHashArray[i].id) {
          printf(" -> %d, %c, %d", pHashArray[j].id, pHashArray[j].name,
                 pHashArray[j].order);
        }
      }
      printf("\n");
    }
  }
}

int main(void) {
  struct RecordType* pRecords;
  int recordSz = 0;
  int hashSz = 10;
  struct HashType* pHashArray =
      (struct HashType*)malloc(sizeof(struct HashType) * hashSz);

  recordSz = parseData("input.txt", &pRecords);
  printRecords(pRecords, recordSz);

  // Initialize the hash array
  for (int i = 0; i < hashSz; i++) {
    pHashArray[i].isOccupied = 0;
  }

  // Insert records into the hash array
  for (int i = 0; i < recordSz; i++) {
    int index = hash(pRecords[i].id);
    while (pHashArray[index].isOccupied) {
      index = (index + 1) % hashSz;
    }
    pHashArray[index].id = pRecords[i].id;
    pHashArray[index].name = pRecords[i].name;
    pHashArray[index].order = pRecords[i].order;
    pHashArray[index].isOccupied = 1;
  }

  // Display the records in the hash array
  displayRecordsInHash(pHashArray, hashSz);

  free(pRecords);
  free(pHashArray);
  return 0;
}