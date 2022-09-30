<?php

/**
 * file:parse.php
 * author: Tomas Tomala (xtomal02)
 * last modification: 17.03.2021
 */
################################################################################
ini_set('display_errors', 'stderr');
$xml = new DOMDocument("1.0", "UTF-8");
$xml->formatOutput = true;
$xmlProgram = $xml->createElement("program");
$xmlProgram->setAttribute("language", "IPPcode21");
$xmlProgram = $xml->appendChild($xmlProgram);
executeArgs();
checkHeader();
loadInstructions($xml, $xmlProgram);
echo $xml->saveXML();

function executeArgs() {
  global $argc, $argv;
  if ($argc > 1) {
    #Command line args exist
    if (in_array("--help", $argv)) {
      if ($argc != 2) {
        #Exit 10, combined --help
        fprintf(STDERR, "ERROR: Parameter \"--help\" cannot be combined with other parameters\n");
        exit(10);
      } else {
        #Print help 
        fprintf(STDOUT, "This script loads source code written in IPP-code21, checks correctness of syntax and lexical analysis and prints out XML representation on the standart output.\n");
        exit(0);
      }
    } else {
      fprintf(STDERR, "ERROR: Non-existent parameter\n");
      exit(10);
    }
  }
}

function checkHeader() {
  #Searching header
  $line = fgets(STDIN);
  #Removing comments, whitespaces, converting string to lowercase
  $line = preg_replace("~#.*~", "", $line, -1);
  $line = trim($line);
  $line = strtolower($line);
  if (!empty($line)) {
    #Line isnt empty, comparing header
    if (strcmp($line, ".ippcode21") != 0) {
      #Line isnt empty and isnt header
      fprintf(STDERR, "Source code is missing mandatory header \".IPPcode21\"\n");
      exit(21);
    }
  } else if (feof(STDIN)) {
    #End of file, missing header
    fprintf(STDERR, "Source code is missing mandatory header \".IPPcode21\"\n");
    exit(21);
  } else {
    #Empty line or comment, skipping to a new line
    checkHeader();
  }
}

function loadInstructions($xml, $xmlProgram) {
  $instCounter = 0;
  while (!feof(STDIN)) {
    $line = fgets(STDIN);
    #Removing comments, whitespaces
    $line = preg_replace("~#.*~", "", $line, -1);
    $line = trim($line);
    #If line isnt empty, expect instruction
    if (!empty($line)) {
      $instCounter++;
      $xmlInstruction = $xml->createElement("instruction");
      $instruction = new Instruction($line, $xml, $xmlInstruction);
      $xmlInstruction->setAttribute("order", $instCounter);
      $xmlInstruction->setAttribute("opcode", $instruction->type);
      $xmlProgram->appendChild($xmlInstruction);
    }
  }
}

class Instruction {
  public $type;
  public $argCounter;
  public $args = array();

  function __construct($line, $xml, $xmlInstruction) {
    $this->processLine($line);
    $this->checkArgs($xml, $xmlInstruction);
  }
  private function processLine($line) {
    #Slit string into array
    $this->args = explode(" ", $line);
    #Check if some args are empty
    $argCounter = 0;
    foreach ($this->args as $argument) {
      if (strlen($argument) == 0) {
        unset($this->args[$argCounter]);
      }
      $argCounter++;
    }
    $this->type = strtoupper(array_shift($this->args));
    $this->argCounter = count($this->args);
  }

  private function checkLbl($argument, $argumentNumber, $xml, $xmlInstruction) {
    if (preg_match("~^[a-zA-Z%!?&_*$-][a-zA-Z%!?&*_$0-9-]*$~", $argument)) {
      $safe_value = preg_replace('/&(?!\w+;)/', '&amp;', $argument);
      $xmlArg = $xml->createElement("arg$argumentNumber", $safe_value);
      $xmlArg->setAttribute("type", "label");
      $xmlInstruction->appendChild($xmlArg);
      return true;
    } else {
      return false;
    }
  }

  private function checkType($argument, $argumentNumber, $xml, $xmlInstruction) {
    if (preg_match("~^(int|bool|string)$~", $argument)) {
      $xmlArg = $xml->createElement("arg$argumentNumber", $argument);
      $xmlArg->setAttribute("type", "type");
      $xmlInstruction->appendChild($xmlArg);
      return true;
    } else {
      return false;
    }
  }

  private function checkVar($argument, $argumentNumber, $xml, $xmlInstruction) {
    if (preg_match("~^(LF|GF|TF)@[a-zA-Z%!?&_*$-][a-zA-Z%!?&*_$0-9-]*$~", $argument)) {
      $safe_value = preg_replace('/&(?!\w+;)/', '&amp;', $argument);
      $xmlArg = $xml->createElement("arg$argumentNumber", $safe_value);
      $xmlArg->setAttribute("type", "var");
      $xmlInstruction->appendChild($xmlArg);
      return true;
    } else {
      return false;
    }
  }
  private function checkSymb($argument, $argumentNumber, $xml, $xmlInstruction) {
    if (preg_match("~@~", $argument)) {
      //Contains @ 
      if (!$this->checkVar($argument, $argumentNumber, $xml, $xmlInstruction)) {
        //&Its not variable
        if (preg_match("~^(int|bool|string|nil)@.*$~", $argument)) {
          //&Its constant
          $constantSeparated = explode("@", $argument, 2);
          if (strcmp($constantSeparated[0], "bool") == 0) {
            //?Constant is bool
            if (!((strcmp($constantSeparated[1], "true") == 0) || (strcmp($constantSeparated[1], "false") == 0))) {
              return false;
            }
            $xmlArg = $xml->createElement("arg$argumentNumber", $constantSeparated[1]);
            $xmlArg->setAttribute("type", "bool");
            $xmlInstruction->appendChild($xmlArg);
          } else if (strcmp($constantSeparated[0], "int") == 0) {
            //?Constant is int 
            //if(preg_match("~[+-]?[0-9]+~",$constantSeparated[1])){
            // Lexical analysis of int value isnt mandatory
            if (strlen($constantSeparated[1]) == 0) {
              return false;
            }
            $xmlArg = $xml->createElement("arg$argumentNumber", $constantSeparated[1]);
            $xmlArg->setAttribute("type", "int");
            $xmlInstruction->appendChild($xmlArg);
            // } else {
            //  return false;
            // }
          } else if (strcmp($constantSeparated[0], "nil") == 0) {
            //?Constant is nil
            if (!(strcmp($constantSeparated[1], "nil") == 0)) {
              return false;
            }
            $xmlArg = $xml->createElement("arg$argumentNumber", $constantSeparated[1]);
            $xmlArg->setAttribute("type", "nil");
            $xmlInstruction->appendChild($xmlArg);
          } else {
            //?Constant is string
            //Checks  if string cointans forbidden character  # 
            if (strpos($constantSeparated[1], '#') != false) return false;
            //Checks  if string cointans forbidden character \ that isnt escape sequence
            $backslashPosition = strpos($constantSeparated[1], '\\');
            while ($backslashPosition != false) {
              if ((strlen($constantSeparated[1]) - $backslashPosition) < 4) {
                //Backlash is at the end of the string
                return false;
              }
              //String contains \
              for ($i = 1; $i <= 3; $i++) {
                if (!is_numeric($constantSeparated[1][$backslashPosition + $i])) {
                  //String cointains nondigit in escape sequence
                  return false;
                }
              }
              $backslashPosition = strpos($constantSeparated[1], '\\', $backslashPosition + 1);
            }
            $safe_value = preg_replace('/&(?!\w+;)/', '&amp;', $constantSeparated[1]);
            $xmlArg = $xml->createElement("arg$argumentNumber", $safe_value);
            $xmlArg->setAttribute("type", "string");
            $xmlInstruction->appendChild($xmlArg);
          }
        } else {
          //&Its not variable or constant
          return false;
        }
      }
      return true;
    } else {
      return false;
    }
  }

  private function checkArgs($xml, $xmlInstruction) {
    switch ($this->type) {
      case 'CREATEFRAME':
      case 'PUSHFRAME':
      case 'POPFRAME':
      case 'RETURN':
      case 'BREAK':
        //No operand to check
        if (count($this->args) != 0) {
          fprintf(STDERR, "ERROR: Number of arguments for " . $this->type . " | Expected= 0, Actual:" . count($this->args) . "\n");
          exit(23);
        }
        break;
        //*⟨var⟩
      case 'POPS':
      case 'DEFVAR':
        if (count($this->args) != 1) {
          fprintf(STDERR, "ERROR: Number of arguments for " . $this->type . " | Expected= 1, Actual:" . count($this->args) . "\n");
          exit(23);
        }
        // checking ⟨var⟩
        if ($this->checkVar($this->args[0], 1, $xml, $xmlInstruction) == false) {
          fprintf(STDERR, "ERROR: Invalid argument: " . $this->args[0] . " \n");
          exit(23);
        }
        break;
        //*⟨symb⟩
      case 'PUSHS':
      case 'WRITE':
      case 'EXIT':
      case 'DPRINT':
        if (count($this->args) != 1) {
          fprintf(STDERR, "ERROR: Number of arguments for " . $this->type . " | Expected= 1, Actual:" . count($this->args) . "\n");
          exit(23);
        }
        // checking ⟨symb⟩
        if ($this->checkSymb($this->args[0], 1, $xml, $xmlInstruction) == false) {
          fprintf(STDERR, "ERROR: Invalid argument: " . $this->args[0] . " \n");
          exit(23);
        }
        break;
        //*⟨label⟩
      case 'CALL':
      case 'LABEL':
      case 'JUMP':
        if (count($this->args) != 1) {
          fprintf(STDERR, "ERROR: Number of arguments for " . $this->type . " | Expected= 1, Actual:" . count($this->args) . "\n");
          exit(23);
        }
        //checking ⟨label⟩
        if ($this->checkLbl($this->args[0], 1, $xml, $xmlInstruction) == false) {
          fprintf(STDERR, "ERROR: Invalid argument: " . $this->args[0] . " \n");
          exit(23);
        }
        break;
        //*⟨var⟩ ⟨symb⟩
      case 'INT2CHAR':
      case 'STRLEN':
      case 'TYPE':
      case 'MOVE':
      case 'NOT':
        if (count($this->args) != 2) {
          fprintf(STDERR, "ERROR: Number of arguments for " . $this->type . " | Expected= 2, Actual:" . count($this->args) . "\n");
          exit(23);
        }
        //^ checking ⟨var⟩
        if ($this->checkVar($this->args[0], 1, $xml, $xmlInstruction) == false) {
          fprintf(STDERR, "ERROR: Invalid argument: " . $this->args[0] . " \n");
          exit(23);
        }
        //^ checking ⟨symb⟩
        if ($this->checkSymb($this->args[1], 2, $xml, $xmlInstruction) == false) {
          fprintf(STDERR, "ERROR: Invalid argument: " . $this->args[1] . " \n");
          exit(23);
        }
        break;
        //*⟨var⟩ ⟨symb1⟩ ⟨symb2⟩
      case 'ADD':
      case 'SUB':
      case 'MUL':
      case 'IDIV':
      case 'LT':
      case 'GT':
      case 'EQ':
      case 'AND':
      case 'OR':
      case 'STRI2INT':
      case 'CONCAT':
      case 'GETCHAR':
      case 'SETCHAR':
        if (count($this->args) != 3) {
          fprintf(STDERR, "ERROR: Number of arguments for " . $this->type . " | Expected= 3, Actual:" . count($this->args) . "\n");
          exit(23);
        }
        //^ checking ⟨var⟩
        if ($this->checkVar($this->args[0], 1, $xml, $xmlInstruction) == false) {
          fprintf(STDERR, "ERROR: Invalid argument: " . $this->args[0] . " \n");
          exit(23);
        }
        //^ checking ⟨symb1⟩
        if ($this->checkSymb($this->args[1], 2, $xml, $xmlInstruction) == false) {
          fprintf(STDERR, "ERROR: Invalid argument: " . $this->args[1] . " \n");
          exit(23);
        }
        //^checking ⟨symb2⟩
        if ($this->checkSymb($this->args[2], 3, $xml, $xmlInstruction) == false) {
          fprintf(STDERR, "ERROR: Invalid argument: " . $this->args[2] . " \n");
          exit(23);
        }
        break;
        //*⟨label⟩ ⟨symb1⟩ ⟨symb2⟩
      case 'JUMPIFNEQ':
      case 'JUMPIFEQ':
        if (count($this->args) != 3) {
          fprintf(STDERR, "ERROR: Number of arguments for " . $this->type . " | Expected= 3, Actual:" . count($this->args) . "\n");
          exit(23);
        }
        //^ checking ⟨label⟩
        if ($this->checkLbl($this->args[0], 1, $xml, $xmlInstruction) == false) {
          fprintf(STDERR, "ERROR: Invalid argument: " . $this->args[0] . " \n");
          exit(23);
        }
        //^ checking ⟨symb1⟩
        if ($this->checkSymb($this->args[1], 2, $xml, $xmlInstruction) == false) {
          fprintf(STDERR, "ERROR: Invalid argument: " . $this->args[1] . " \n");
          exit(23);
        }
        //^checking ⟨symb2⟩
        if ($this->checkSymb($this->args[2], 3, $xml, $xmlInstruction) == false) {
          fprintf(STDERR, "ERROR: Invalid argument: " . $this->args[2] . " \n");
          exit(23);
        }
        break;
        //*⟨var⟩ ⟨type⟩ 
      case 'READ':
        if (count($this->args) != 2) {
          fprintf(STDERR, "ERROR: Number of arguments for " . $this->type . " | Expected= 2, Actual:" . count($this->args) . "\n");
          exit(23);
        }
        //^ checking ⟨var⟩
        if ($this->checkVar($this->args[0], 1, $xml, $xmlInstruction) == false) {
          fprintf(STDERR, "ERROR: Invalid argument: " . $this->args[0] . " \n");
          exit(23);
        }
        //^ checking ⟨type⟩ 
        if ($this->checkType($this->args[1], 2, $xml, $xmlInstruction) == false) {
          fprintf(STDERR, "ERROR: Invalid argument: " . $this->args[1] . " \n");
          exit(23);
        }
        break;
      default:
        fprintf(STDERR, "ERROR: Non-existent operation code: $this->type \n");
        exit(22);
    }
  }
}
