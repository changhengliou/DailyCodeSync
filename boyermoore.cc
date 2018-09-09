#include <iostream>
#include <string>
#include <vector>

#define NUM_OF_CHAR 256

using namespace std;

class BoyerMoore {
 private:
  char lookup[NUM_OF_CHAR];
  string pattern;

 public:
  int bad_char_rule(string text) {
    int shift = 0;

    while (shift <= (text.size() - pattern.size())) {
      int index = pattern.size() - 1;

      while (index >= 0 && text[index + shift] == pattern[index]) index--;

      if (index < 0) {  // it is a match
        return shift;
        // shift += (shift + pattern.size() < text.size()) ?
        //           pattern.size() - lookup[text[shigt + pattern.size()]] : 1;
      } else {
        // in case negative shift may happen, which means we don't know the
        // match before the bad character, so just shift 1 Ex. GCTTCTGCTACCTT
        //     CCTTTTGC
        // Bad character is at index 4, which is 'C'
        // 4 - 7 is -3, we only know the 'C' occur at index 7
        // so we only shift by 1
        int look = lookup[static_cast<int>(text[shift + index])];
        shift += max(1, index - look);
      }
    }
    return -1;
  }

  int good_suffix_rule() {}

  void preprocess_strong_suffix(int *shift, int *bpos) {
    int i = pattern.size(), j = pattern.size() + 1;
    bpos[i] = j;

    while (i > 0) {
      /*if character at position i-1 is not equivalent to
        character at j-1, then continue searching to right
        of the pattern for border */
      while (j <= pattern.size() && pattern[i - 1] != pattern[j - 1]) {
        /* the character preceding the occurence of t in
           pattern P is different than mismatching character in P,
           we stop skipping the occurences and shift the pattern
           from i to j */
        if (shift[j] == 0) shift[j] = j - i;

        // Update the position of next border
        j = bpos[j];
      }
      /* p[i-1] matched with p[j-1], border is found.
         store the  beginning position of border */
      i--;
      j--;
      bpos[i] = j;
    }
  }

  void preprocess_case2(int *shift, int *bpos) {
    int i, j;
    j = bpos[0];
    for (i = 0; i <= pattern.size(); i++) {
      /* set the border postion of first character of pattern
         to all indices in array shift having shift[i] = 0 */
      if (shift[i] == 0) shift[i] = j;

      /* suffix become shorter than bpos[0], use the position of
         next widest border as value of j */
      if (i == j) j = bpos[j];
    }
  }

  BoyerMoore(string pattern) {
    // preprocessing for bad character rule
    fill(lookup, lookup + NUM_OF_CHAR, -1);
    for (int i = 0; i < pattern.size(); ++i) {
      lookup[static_cast<int>(pattern[i])] = i;
    }
    this->pattern = pattern;
  }
};

int main() {
  // ABBABAZ AABBABAB ABACBCBBABAB
  // ABBABAB
  // ABBAZABBABAB
  // ABBABAB
  // ABAAABCD
  // ABC
  // GCTTCTGCTACCTT
  // CCTTTTGC
  BoyerMoore b("CCTTTTGC");
  cout << b.bad_char_rule("GCTTCTGCTACCTT") << endl;
}