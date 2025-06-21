#include "PostFunctionalCompleteness.h"

#define POST_FUNCTIONAL_COMPLETENESS_CLASSES 5

#define MAX_LONG_LONG_INT 0xFFFFFFFFFFFFFFFF

static boolean *bitMaskToBooleanArray(long long int bitMask, int varCount) {
  if (varCount > 64) {
    // TODO: Too many variables error
    return NULL;
  }
  boolean *truthValueArray = malloc(varCount * sizeof(boolean));
  for (int i = 0; i < varCount; i++) {
    truthValueArray[i] = (bitMask & (1 << i)) != 0;
  }
  return truthValueArray;
}

static long long int initializeBitMask(int varCount) {
  long long int bitMask = 0;
  for (int i = 0; i < varCount; i++) {
    bitMask = (bitMask << 1) | 1;
  }
  return bitMask;
}

/**
 * Counts the number of 1s in the binary representation of x
 */
int count_ones(unsigned long long int x) {
  int count = 0;
  while (x) {
    x &= (x - 1); // clears the least significant set bit
    count++;
  }
  return count;
}

/*
 *   Given an operator and a class number, this function checks if the operator
 *   belongs to the class.
 */
static boolean isClassI(Operator *operator, int class,
                        SymbolEntry *symbolTable) {

  SymbolEntry *tableEntry = find_symbol(symbolTable, operator);
  if (tableEntry == NULL) {
    return false;
  }
  TruthTable *truthTable = tableEntry->data.operator_data.truth_table;
  int varCount = tableEntry->data.operator_data.argc;

  switch (class) {
    // Class 1
  case 0:
    if (strcmp(operator, "&") == 0) {
      return true;
    } else if (strcmp(operator, "|") == 0) {
      return true;
    } else if (strcmp(operator, "!") == 0) {
      return false;
    } else if (strcmp(operator, "=>") == 0) {
      return true;
    } else if (strcmp(operator, "<=>") == 0) {
      return true;
    }
    return isType1(truthTable, symbolTable, varCount);

    // Class 2
  case 1:
    if (strcmp(operator, "&") == 0) {
      return true;
    } else if (strcmp(operator, "|") == 0) {
      return true;
    } else if (strcmp(operator, "!") == 0) {
      return false;
    } else if (strcmp(operator, "=>") == 0) {
      return false;
    } else if (strcmp(operator, "<=>") == 0) {
      return true;
    }
    return isType2(truthTable, symbolTable, varCount);

    // Class 3
  case 2:
    if (strcmp(operator, "&") == 0) {
      return false;
    } else if (strcmp(operator, "|") == 0) {
      return false;
    } else if (strcmp(operator, "!") == 0) {
      return true;
    } else if (strcmp(operator, "=>") == 0) {
      return false;
    } else if (strcmp(operator, "<=>") == 0) {
      return true;
    }
    return isType3(truthTable, symbolTable, varCount);

    // Class 4
  case 3:
    if (strcmp(operator, "&") == 0) {
      return true;
    } else if (strcmp(operator, "|") == 0) {
      return true;
    } else if (strcmp(operator, "!") == 0) {
      return false;
    } else if (strcmp(operator, "=>") == 0) {
      return false;
    } else if (strcmp(operator, "<=>") == 0) {
      return true;
    }
    return isType4(truthTable, symbolTable, varCount);

    // Class 5
  case 4:
    if (strcmp(operator, "&") == 0) {
      return false;
    } else if (strcmp(operator, "|") == 0) {
      return false;
    } else if (strcmp(operator, "!") == 0) {
      return true;
    } else if (strcmp(operator, "=>") == 0) {
      return false;
    } else if (strcmp(operator, "<=>") == 0) {
      return true;
    }
    return isType5(truthTable, symbolTable, varCount);
  }
}

boolean isFunctionallyComplete(OpsetList *opset, SymbolEntry *symbolTable) {
  OpsetList *aux = opset;
  int num_operators = list_size(opset);

  // For each class, we check if there is an operator that doesn't belong to
  // the class
  for (int i = 0; i < POST_FUNCTIONAL_COMPLETENESS_CLASSES; i++) {
    for (int j = 0; j < num_operators; j++) {
      int varCount = list_size(aux->operator);
      // If there is an operator that doesn't belong to the class, the theorem
      // holds (for now) and we can try the next class
      if (!isClassI(aux->operator, i, symbolTable)) {
        aux = opset;
        break;
      }
      aux = aux->next;
    }
    // If we've checked all operators and they all belong to the class, the
    // theorem doesn't hold, so we return false
    return false;
  }
  return true;
}

boolean isType1(TruthTable *truthTable, SymbolEntry *symbolTable,
                int varCount) {
  boolean *truthValueArray = malloc((varCount) * sizeof(boolean));
  for (int i = 0; i < varCount; i++) {
    truthValueArray[i] = true;
  }
  ComputationResult result = computeTruthTableFromTruthValueArray(
      truthTable, truthValueArray, varCount);
  free(truthValueArray);
  return result.succeed && result.value;
}

boolean isType2(TruthTable *truthTable, SymbolEntry *symbolTable,
                int varCount) {
  boolean *truthValueArray = malloc(varCount * sizeof(boolean));
  for (int i = 0; i < varCount; i++) {
    truthValueArray[i] = false;
  }
  ComputationResult result = computeTruthTableFromTruthValueArray(
      truthTable, truthValueArray, varCount);
  free(truthValueArray);
  return result.succeed && result.value;
}

boolean isType3(TruthTable *truthTable, SymbolEntry *symbolTable,
                int varCount) {
  boolean isEvenCountingFunction;

  // We need to check if the operator is an even-counting function or an
  // odd-counting function.
  // We start by checking the maximum number of trues.
  long long int maxBitMaskTruthArray = initializeBitMask(varCount);
  boolean *maxTruthValueArray =
      bitMaskToBooleanArray(maxBitMaskTruthArray, varCount);
  ComputationResult result = computeTruthTableFromTruthValueArray(
      truthTable, maxTruthValueArray, varCount);
  if (!result.succeed) {
    free(maxTruthValueArray);
    // TODO: Error
    return false;
  }
  int ones = count_ones(maxBitMaskTruthArray);
  if ((ones % 2 == 0 && result.value == true) ||
      (ones % 2 == 1 && result.value == false)) {
    isEvenCountingFunction = true;
  } else {
    isEvenCountingFunction = false;
  }
  free(maxTruthValueArray);

  // Now we need to check if the operator is consistent for every other
  // combination of truth values.
  for (long long int bitMaskTruthArray = maxBitMaskTruthArray;
       bitMaskTruthArray >= 0; bitMaskTruthArray--) {
    boolean *truthValueArray =
        bitMaskToBooleanArray(bitMaskTruthArray, varCount);
    ComputationResult result = computeTruthTableFromTruthValueArray(
        truthTable, truthValueArray, varCount);
    if (!result.succeed) {
      free(maxTruthValueArray);
      // TODO: Error
      return false;
    }
    int ones = count_ones(bitMaskTruthArray);

    // If the operator is inconsistent, then it is not of type 3
    if ((isEvenCountingFunction && ones % 2 == 1 && result.value == true) ||
        (isEvenCountingFunction && ones % 2 == 0 && result.value == false) ||
        (!isEvenCountingFunction && ones % 2 == 0 && result.value == true) ||
        (!isEvenCountingFunction && ones % 2 == 1 && result.value == false)) {
      free(truthValueArray);
      return false;
    }
    free(truthValueArray);
  }
  return true;
}

boolean isType4(TruthTable *truthTable, SymbolEntry *symbolTable,
                int varCount) {
  // We can think of the truthValueArray as a binary number, where each bit
  // represents a truth value. This way, incrementing the truthValueArray
  // number until we reach 2^varCount will give us all the possible
  // combinations of truth values for the truth table. Using a long long int
  // we can have a maximum of 64 variables.

  // We store a tuple [trueCount, calculatedValue]
  // trueCount is the number of truth values that are true in the current
  // combination.
  // calculatedValue is the value of the operator for the current combination.
  // We start with trueCount = 0 and calculatedValue = 0.

  // Set each bit of the bitMaskTruthArray to 1
  long long int maxBitMaskTruthArray = initializeBitMask(varCount);
  boolean *maxTruthValueArray =
      bitMaskToBooleanArray(maxBitMaskTruthArray, varCount);

  ComputationResult firstResult = computeTruthTableFromTruthValueArray(
      truthTable, maxTruthValueArray, varCount);
  if (!firstResult.succeed) {
    free(maxTruthValueArray);
    // TODO: Error
    return false;
  }
  boolean calculatedValue = firstResult.value;
  int trueCount = count_ones(maxBitMaskTruthArray);
  free(maxTruthValueArray);

  for (long long int bitMaskTruthArray = maxBitMaskTruthArray;
       bitMaskTruthArray >= 0; bitMaskTruthArray--) {
    int ones = count_ones(bitMaskTruthArray);
    // If the number of ones is less or equal to the previous one, then we
    // need to check if the result is also less or equal to the previous one
    if (ones <= trueCount) {
      boolean *truthValueArray =
          bitMaskToBooleanArray(bitMaskTruthArray, varCount);
      ComputationResult result = computeTruthTableFromTruthValueArray(
          truthTable, truthValueArray, varCount);
      if (!result.succeed) {
        free(truthValueArray);
        // TODO: Error
        return false;
      }
      // If the calculated value is greater than the previous one, then the
      // property is not satisfied
      if (result.value > calculatedValue) {
        free(truthValueArray);
        return false;
      }
      trueCount = ones;
      calculatedValue = result.value;
      free(truthValueArray);
    }
  }
  // If we've checked all the possible combinations then we know that the
  // operator is of type 4
  return true;
}

boolean isType5(TruthTable *truthTable, SymbolEntry *symbolTable,
                int varCount) {

  // Same ideas isType4, but we need to check if f(x1, ... xn) = ¬f(¬x1, ...
  // ¬xn) for all the possible combinations of truth values.

  long long int maxBitMaskTruthArray = initializeBitMask(varCount);

  // Now we iterate through all the possible combinations of truth values and
  // check if f(x1, ... xn) = ¬f(¬x1, ... ¬xn)
  for (long long int bitMaskTruthArray = maxBitMaskTruthArray;
       bitMaskTruthArray >= 0; bitMaskTruthArray--) {

    boolean *truthValueArray =
        bitMaskToBooleanArray(bitMaskTruthArray, varCount);
    long long int negatedBitMaskTruthArray =
        (~bitMaskTruthArray & maxBitMaskTruthArray);
    boolean *negatedTruthValueArray =
        bitMaskToBooleanArray(negatedBitMaskTruthArray, varCount);
    ComputationResult result = computeTruthTableFromTruthValueArray(
        truthTable, truthValueArray, varCount);
    ComputationResult negatedResult = computeTruthTableFromTruthValueArray(
        truthTable, negatedTruthValueArray, varCount);

    // if f(x1, ... xn) != ¬f(¬x1, ... ¬xn) then the operator is not of type 5
    if (result.succeed && negatedResult.succeed &&
        (result.value == negatedResult.value)) {
      free(truthValueArray);
      free(negatedTruthValueArray);
      return false;
    }
    free(truthValueArray);
    free(negatedTruthValueArray);
  }

  // If we've checked all the possible combinations and for each f(x1, ... xn)
  // = ¬f(¬x1, ... ¬xn), then the operator is of type 5
  return true;
}
