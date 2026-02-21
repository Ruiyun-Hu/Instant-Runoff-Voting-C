/* Program to implement Australian House of Representatives preferential
   voting and determine election outcomes from vote preference orderings.

   Skeleton program written by Alistair Moffat, ammoffat@unimelb.edu.au,
   August 2025, with the intention that it be modified by students
   to add functionality, as required by the assignment specification.
   All included code is (c) Copyright University of Melbourne, 2025

   Student Authorship Declaration:

   (1) I certify that except for the code provided in the initial skeleton
   file, the  program contained in this submission is completely my own
   individual work, except where explicitly noted by further comments that
   provide details otherwise.  I understand that work that has been
   developed by another student, or by me in collaboration with other
   students, or by non-students as a result of request, solicitation, or
   payment, may not be submitted for assessment in this subject.  I
   understand that submitting for assessment work developed by or in
   collaboration with other students or non-students constitutes Academic
   Misconduct, and may be penalized by mark deductions, or by other
   penalties determined via the University of Melbourne Academic Honesty
   Policy, as described at https://academicintegrity.unimelb.edu.au.

   (2) I also certify that I have not provided a copy of this work in either
   softcopy or hardcopy or any other form to any other student, and nor will
   I do so until after the marks are released. I understand that providing
   my work to other students, regardless of my intention or any undertakings
   made to me by that other student, is also Academic Misconduct.

   (3) I further understand that providing a copy of the assignment
   specification to any form of code authoring or assignment tutoring
   service, or drawing the attention of others to such services and code
   that may have been made available via such a service, may be regarded as
   Student General Misconduct (interfering with the teaching activities of
   the University and/or inciting others to commit Academic Misconduct).  I
   understand that an allegation of Student General Misconduct may arise
   regardless of whether or not I personally make use of such solutions or
   sought benefit from such actions.

   Signed by: Ruiyun Hu 1744073
   Dated:     11/09/2025

*/

#include <assert.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>

///////////////////////////////////////////////////////////////////////

#define MAX_CANDIDATES 20
#define MAX_NAME_LEN 21
#define MAX_VOTES 999

// add your other function prototypes here

// Get the input
int getword(char W[], int limit);

// Calculate the numbers of candidates who haven't been eliminated
int remain_numbers(int A[], int a);

// To record the percentage of the votes for each candidate
void create_percentage(int A[], double B[], int cd_number, int v_number);

// Give the votes from the eliminated candidate to the next candidate
// which the voter choose as the next position
int distribute_votes(int C[], int V[][MAX_CANDIDATES], int m, int n);

// All the operations for stage 1
int stage1(int *m, int *n, char candidates[][MAX_NAME_LEN],
           int votes[][MAX_CANDIDATES]);

// All the operations for stage 2
void stage2(int m, int n, char candidates[][MAX_NAME_LEN],
            int votes[][MAX_CANDIDATES], int votes_for_3[][MAX_CANDIDATES],
            int counts_for_3[]);

// All the operations for stage 3
void stage3(int m, int n, char candidates[][MAX_NAME_LEN],
            int votes_for_3[][MAX_CANDIDATES], int counts_for_3[]);

///////////////////////////////////////////////////////////////////////

// main program provides traffic control

int main(int argc, char *argv[]) {
  // The number of candidates
  int m;
  // The number of votes
  int n = 0;

  // A double-dimensional string used to record
  // the names of the candidates
  char candidates[MAX_CANDIDATES][MAX_NAME_LEN];

  // A double-dimensional array used to record
  // the ranking given by each voter
  int votes[MAX_VOTES][MAX_CANDIDATES];

  // The same content as votes but used for stage 3
  int votes_for_3[MAX_VOTES][MAX_CANDIDATES];

  // The same content as counts but used for stage 3
  int counts_for_3[MAX_CANDIDATES] = {0};

  stage1(&m, &n, candidates, votes);
  stage2(m, n, candidates, votes, votes_for_3, counts_for_3);
  stage3(m, n, candidates, votes_for_3, counts_for_3);

  // all done, time to go home
  // algorithms are fun
  printf("tadaa!\n");
  return 0;
}

///////////////////////////////////////////////////////////////////////

// extract a single word out of the standard input, of not
// more than limit characters, argument array W must be
// limit+1 characters or bigger.
// function adapted from Figure 7.13 of Programming, Problem Solving,
// and Abstraction with C, by Alistair Moffat
//
int getword(char W[], int limit) {
  int c, len = 0;
  /* first, skip over any non alphabetics */
  while ((c = getchar()) != EOF && !isalpha(c)) {
    /* do nothing more */
  }
  if (c == EOF) {
    return EOF;
  }
  /* ok, first character of next word has been found */
  W[len++] = c;
  while (len < limit && (c = getchar()) != EOF && isalpha(c)) {
    /* another character to be stored */
    W[len++] = c;
  }
  /* now close off the string */
  W[len] = '\0';
  return 0;
}

///////////////////////////////////////////////////////////////////////

// add your other functions here

// Reading the input of all information, including
// the numbers of voters, the numbers of candidates,
// the names of the candidates, finally printing the voting Condition
// of the last voter. Since the function needs to change the value of
// m and n in global scope, it chooses pointers to achieve the goal.
int stage1(int *m, int *n, char candidates[][MAX_NAME_LEN],
           int votes[][MAX_CANDIDATES]) {
  // Record the number of candidates;
  scanf("%d", m);

  // Condition to decide whether the input ends or not
  int in_input = 1;
  // Number of the voters, a transformer from int to a pointer *n
  int count = 0;

  for (int i = 0; i < *m; i++) {
    // Record each candidate's name
    getword(candidates[i], MAX_NAME_LEN);
  }
  while (in_input) {
    for (int j = 0; j < *m; j++) {
      if (scanf("%d", &votes[count][j]) != 1) {
        in_input = 0;
        // If receive no input, ends the loop
        break;
      }
    }
    // Every time j reaches m - 1, meaning a voter's votes
    // have been recorded, numbers of votes plus one
    count += 1;
  }
  // final numbers of votes
  *n = count - 1;

  printf("\n");
  printf("Stage 1\n");
  printf("=======\n");
  printf("read %d candidates and %d votes\n", *m, *n);
  printf("voter %d preferences...\n", *n);
  for (int rank = 1; rank <= *m; rank++) {
    for (int a = 0; a < *m; a++) {
      // Check the last voter's votes to match the rank
      // loop over the rank to print out all the candidates
      if (votes[*n - 1][a] == rank) {
        printf("    rank  %d: %s\n", rank, candidates[a]);
      }
    }
  }
  printf("\n");
  return 0;
}

// The function records each candidate's votes and their respective percentage,
// and in each round eliminate a candidate who gets least votes
// and print out the current votes condition, such kind of loop continues
// until there are two candidates left.
// Finally, it's declaration of the winner depends on whether
// the votes of the last two candidates are same or not.
void stage2(int m, int n, char candidates[][MAX_NAME_LEN],
            int votes[][MAX_CANDIDATES], int votes_for_3[][MAX_CANDIDATES],
            int counts_for_3[]) {
  // An array used to record the votes each candidate gets
  // in every round (The one who gets "1" from the voter is regarded
  // as getting one vote), while the value -1 in the array
  // is regarded as elminated
  int counts[MAX_CANDIDATES] = {0};

  // A double-dimensional string used to record the
  // percentages of the votes for each candidate
  double percentages[MAX_CANDIDATES];

  int round = 1;
  // the number of votes
  int total = n;

  for (int i = 0; i < n; i++) {
    for (int j = 0; j < m; j++) {
      if (votes[i][j] == 1) {
        counts[j] += 1;
      }
    }
  }

  for (int i = 0; i < n; i++) {
    for (int j = 0; j < m; j++) {
      votes_for_3[i][j] = votes[i][j];
      if (votes_for_3[i][j] == 1) {
        counts_for_3[j] += 1;
      }
    }
  }

  printf("Stage 2\n");
  printf("=======\n");

  // As long as there is a candidate not be eliminated,
  // the loop continues
  while (remain_numbers(counts, m)) {

    create_percentage(counts, percentages, m, total);

    printf("round %d...\n", round);
    round += 1;

    for (int i = 0; i < m; i++) {
      if (counts[i] >= 0) {
        printf("    %-19s :%4d votes,%6.1lf%%\n", candidates[i], counts[i],
               percentages[i]);
      }
    }
    printf("    ----\n");

    // Using helper function to find the candidate who gets
    // the least votes in current round
    int min_index = distribute_votes(counts, votes, m, n);

    // Eliminate the candidate
    counts[min_index] = -1;

    // Check if the winner appears, as shown that one's votes
    // account for more than 50%
    int winner = 0;
    for (int i = 0; i < m; i++) {
      if (percentages[i] > 50.0) {
        winner = 1;
        printf("    %s is declared elected\n", candidates[i]);
        printf("\n");
        break;
      }
    }
    if (winner == 1) {
      break;
    }

    // If not, still print out eliminating candidates
    printf("    %s is eliminated and votes distributed\n",
           candidates[min_index]);
    printf("\n");
  }
  printf("\n");
}

// Basically the same effect as the stage2, the only difference is
// to rearrange the sequence of the candidates due to their votes
// (from high to low, if the same, follow the alphabetic way),
// and is printed out using the new sequence
void stage3(int m, int n, char candidates[][MAX_NAME_LEN],
            int votes_for_3[][MAX_CANDIDATES], int counts_for_3[]) {

  // Same content as percentages but used for stage 3
  double percentages_temp[MAX_CANDIDATES];

  int round = 1;
  int total = n;
  printf("Stage 3\n");
  printf("=======\n");

  // Creating a new indexes for the candidates according to their votes,
  // initially the same as the beginning index
  int indices[MAX_CANDIDATES];
  for (int i = 0; i < m; i++) {
    indices[i] = i;
  }
  while (remain_numbers(counts_for_3, m)) {

    create_percentage(counts_for_3, percentages_temp, m, total);

    // Using insertionsort to change the index sequence in each round
    for (int i = 1; i < m; i++) {
      int j = i;
      while (j > 0) {
        // Set two values shown in the "indices" array,
        // but works as the indexes in "counts_for_3" array
        int a = indices[j - 1], b = indices[j];
        // Comparing the value first then the alphabetically comparison
        if (counts_for_3[a] < counts_for_3[b] ||
            (counts_for_3[a] == counts_for_3[b] &&
             strcmp(candidates[a], candidates[b]) > 0)) {
          // If cater the condition, swap the two values
          // to the correct sequence
          int tmp = indices[j - 1];
          indices[j - 1] = indices[j];
          indices[j] = tmp;
          // Continue the loop until the start of the array
          j--;
        } else {
          break;
        }
      }
    }
    printf("round %d...\n", round++);
    for (int k = 0; k < m; k++) {
      int idx = indices[k];
      if (counts_for_3[idx] >= 0) {
        // Print out using the new index
        printf("    %-19s :%4d votes,%6.1lf%%\n", candidates[idx],
               counts_for_3[idx], percentages_temp[idx]);
      }
    }
    printf("    ----\n");

    // Look for the smallest value
    int min_index_for_3 = distribute_votes(counts_for_3, votes_for_3, m, n);

    // Eliminate candidate
    counts_for_3[min_index_for_3] = -1;

    // Check winner
    int winner_for_3 = 0;
    for (int i = 0; i < m; i++) {
      if (percentages_temp[i] > 50.0) {
        winner_for_3 = 1;
        printf("    %s is declared elected\n", candidates[i]);
        printf("\n");
        break;
      }
    }
    if (winner_for_3 == 1) {
      break;
    }

    printf("    %s is eliminated and votes distributed\n",
           candidates[min_index_for_3]);
    printf("\n");
  }
}

// The function takes an array and the length of it,
// returns the numbers of value which are bigger than 0
// (which means the candidates who are not elminated)
int remain_numbers(int A[], int a) {
  int numbers = 0;
  for (int i = 0; i < a; i++) {
    if (A[i] >= 0) {
      numbers += 1;
    }
  }
  return numbers;
}

// The function calculates the percentage of each candidate's votes
// and put them in the "percentage" array
// in an order of the index of candidates
void create_percentage(int A[], double B[], int cd_number, int v_number) {
  for (int i = 0; i < cd_number; i++) {
    if (A[i] >= 0) {
      B[i] = (double)A[i] * 100 / v_number;
    } else {
      B[i] = -1.0;
    }
  }
}

// The function distributes the votes of eliminated candidate
// in each round to other candidate according to the ranking
// given by the voters who vote the eliminated candidate
int distribute_votes(int C[], int V[][MAX_CANDIDATES], int m, int n) {
  int min = MAX_VOTES;
  int min_number = 0;
  for (int i = 0; i < m; i++) {
    if (C[i] < min && C[i] >= 0) {
      // Continously updating the minimum value by
      // checking through the array, finally find
      // the index of minimum number
      min = C[i];
      min_number = i;
    }
  }

  for (int i = 0; i < n; i++) {
    // Firstly, find the eliminated candidate
    if (V[i][min_number] == 1) {
      int found = 0;
      int up_number = 0;
      // Since the eliminated candidate's value in votes is 1,
      // the function starts from 2 to find the next preferred candidate
      // instead of directly get the value "2" in case of the second
      // preferred candidate has already been eliminated
      for (int rank2 = 2; rank2 <= m; rank2++) {
        for (int j = 0; j < m; j++) {
          // The condition is not be the eliminated one
          if (V[i][j] == rank2 && C[j] != -1) {
            // Give the vote to second preferred
            C[j] += 1;
            found = 1;
            // Increase the ranking of the rest options using "up_number"
            // (e.g. "2" > "1" or "3" > "1" depending on the "up_number")
            up_number = rank2 - 1;
            break;
          }
        }
        if (found) {
          break;
        }
      }
      for (int j = 0; j < m; j++) {
        // After the step, the one who gets the new vote becomes
        // the first ranking of the voter, and the rest of the other
        // survived candidates all get a rank up
        V[i][j] -= up_number;
      }
    }
  }
  return min_number;
}
