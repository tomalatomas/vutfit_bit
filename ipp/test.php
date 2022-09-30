<?php

/**
 * file: test.php
 * author: Tomas Tomala (xtomal02)
 * last modification: 18.04.2021
 */
ini_set('display_errors', 'stderr');
//================VARIABLES======================
//----argument variables-----
$dirPath = getcwd();
$recursive = false;
$parseScript = getcwd() . "/parse.php";
$intScript = getcwd() . "/interpret.py";
$parseOnly = false;
$intOnly = false;
$jexamXml = "/pub/courses/ipp/jexamxml/jexamxml.jar";
$jexamCfg = "/pub/courses/ipp/jexamxml/options";
executeArgs($dirPath, $recursive, $parseScript, $intScript, $parseOnly, $intOnly, $jexamXml, $jexamCfg);
//----test variables-----
$testCounter = 0;
$failedTestCounter = 0;

$tests = findTests($dirPath, $recursive);
executeTests($tests, $parseScript, $intScript, $parseOnly, $intOnly, $jexamXml, $jexamCfg);
printHTML($tests, $dirPath, $parseOnly, $intOnly, $parseScript, $intScript);
//==================ARGUMENTS====================
/**
 * Parses arguments from command line
 *
 * @param string  $dirPath   path to directory with test files
 * @param bool    $recursive  searching for test files recursively in given directory
 * @param string  $parseScript   path to parser script
 * @param string  $intScript  path to interpret script
 * @param bool    $parseOnly for testing only parser
 * @param bool    $intOnly for testing only interpret 
 * @param string  $jexamXml  path to jexamxml jar file
 * @param string  $jexamCfg  path to jexamxml options file
 * @return 
 */
function executeArgs(&$dirPath, &$recursive, &$parseScript, &$intScript, &$parseOnly, &$intOnly, &$jexamXml, &$jexamCfg) {

  $args = getopt("", ["help", "directory:", "recursive", "parse-script:", "int-script:", "parse-only", "int-only", "jexamxml:", "jexamcfg:"]);
  global $argc, $argv;

  //? help
  if (array_key_exists("help", $args)) {
    if ($argc != 2) {
      #Exit 10, combined --help
      fprintf(STDERR, "ERROR: Parameter \"--help\" cannot be combined with other parameters\n");
      exit(10);
    } else {
      #Print help 
      fprintf(STDOUT, "
Tester for IPPCode21 parser and interpret
-----------------
Script automatically goes through directory and test correctness of parser, interpret or both at the same time. HTML5 summary is printed out on standart output
-----------------
USAGE: test.php [--help] [--directory=<directory>] [--recursive] [--parse-script=<parser>] [--int-script=<interpret>] [--parse-only] [--int-only] [--jexamxml=<>] [--jexamcfg=<options>] \n");
      exit(0);
    }
  }
  //? dirPath
  if (array_key_exists("directory", $args)) {
    if (file_exists($args["directory"]) == false) {
      fprintf(STDERR, "ERROR: Directory doesnt exist\n");
      exit(41);
    } else {
      //TODO Check if its directory
      if (is_dir($args["directory"]) == false) {
        fprintf(STDERR, "ERROR: Directory given in arguments isnt a directory\n");
        exit(41);
      }
      if ($args["directory"][-1] == '/') {
        $dirPath = $args["directory"];
      } else {
        $dirPath = $args["directory"] . "/";
      }
    }
  }
  //? recursive
  if (array_key_exists("recursive", $args)) {
    $recursive = true;
  }
  //? parseScript
  if (array_key_exists("parse-script", $args)) {
    if (file_exists($args["parse-script"]) == false) {
      fprintf(STDERR, "ERROR: Parser script file doesnt exist\n");
      exit(41);
    } else {
      $parseScript = $args["parse-script"];
    }
  }
  //? intScript
  if (array_key_exists("int-script", $args)) {
    if (file_exists($args["int-script"]) == false) {
      fprintf(STDERR, "ERROR: Interpret script file doesnt exist\n");
      exit(41);
    } else {
      $intScript = $args["int-script"];
    }
  }
  //? parseOnly
  if (array_key_exists("parse-only", $args)) {
    if (array_key_exists("int-only", $args) || array_key_exists("int-script", $args)) {
      fprintf(STDERR, "ERROR: Invalid parameter combination\n");
      exit(10);
    } else {
      $parseOnly = true;
    }
  }
  //? intOnly
  if (array_key_exists("int-only", $args)) {
    if (array_key_exists("parse-only", $args) || array_key_exists("parse-script", $args)) {
      fprintf(STDERR, "ERROR: Invalid parameter combination\n");
      exit(10);
    } else {
      $intOnly = true;
    }
  }
  //? $jexamXml
  if (array_key_exists("jexamxml", $args)) {
    if (file_exists($args["jexamxml"]) == false) {
      fprintf(STDERR, "ERROR: Jexam xml file doesnt exist\n");
      exit(41);
    } else {
      $jexamXml = $args["jexamxml"];
    }
  }
  //? $jexamCfg
  if (array_key_exists("jexamcfg", $args)) {
    if (file_exists($args["jexamcfg"]) == false) {
      fprintf(STDERR, "ERROR: Jexam config file doesnt exist\n");
      exit(41);
    } else {
      $jexamCfg = $args["jexamcfg"];
    }
  }
}

//==================TESTFILES====================
/**
 * Checks if file exists, if not we create it, put string in it
 * and checks we are able to open the file 
 * @param string  $file path to file
 * @param string  $string  string to write into file
 * @param string  $parseScript   permission mode to work with the file
 */
function testFile($file, $string, $mode) {
  if (!file_exists($file)) {
    file_put_contents($file, $string);
  }
  $fileOpen = fopen($file, $mode);
  if ($fileOpen == false) {
    fprintf(STDERR, "ERROR: Unable to open file $file\n");
    exit(11);
  }
  fclose($fileOpen);
}

/**
 * Checks if test script is able to read from a file 
 * and reads a line
 * @param string  $file path to file
 * @return string $string read line
 */
function getFileString($file) {
  $fileOpen = fopen($file, "r");
  if ($fileOpen == false) {
    fprintf(STDERR, "ERROR: Unable to read $file\n");
    exit(11);
  }
  $string = fgets($fileOpen);
  fclose($fileOpen);
  return $string;
}

//==================FINDTESTS====================
/**
 * Finds all the needed test files 
 * @param string  $dirPath   path to directory with test files
 * @param bool    $recursive  searching for test files recursively in given directory
 * @return array  $testArray array with paths to test files
 */
function findTests(&$dirPath, &$recursive) {
  $testArray = array();
  if ($recursive == true) {
    $dir = new RecursiveDirectoryIterator($dirPath);
    $ite = new RecursiveIteratorIterator($dir);
    $files = new RegexIterator($ite, "~.*.src~", RegexIterator::MATCH);
    foreach ($files as $file) {
      $path = substr($file, 0, -4);
      testFile($path . '.rc', "0", "r");
      testFile($path . '.in', "", "r");
      testFile($path . '.out', "", "r");
      $expectedRC = getFileString($path . '.rc');
      $expectedRC = trim($expectedRC);
      $test = new Test($path, $expectedRC);
      array_push($testArray, $test);
    }
  } else {
    foreach (glob($dirPath . "*.src") as $file) {
      $path = substr($file, 0, -4);
      testFile($path . '.rc', "0", "r");
      testFile($path . '.in', "", "r");
      testFile($path . '.out', "", "r");
      $expectedRC = getFileString($path . '.rc');
      $expectedRC = trim($expectedRC);
      $test = new Test($path, $expectedRC);
      array_push($testArray, $test);
    }
  }
  return $testArray;
}

/**
 * Runs parser script and interpret scripts with test files, checks return codes and output
 *
 * @param array   $tests array with paths to test files
 * @param string  $parseScript   path to parser script
 * @param string  $intScript  path to interpret script
 * @param bool    $parseOnly for testing only parser
 * @param bool    $intOnly for testing only interpret 
 * @param string  $jexamXml  path to jexamxml jar file
 * @param string  $jexamCfg  path to jexamxml options file
 */
function executeTests($tests, $parseScript, $intScript, $parseOnly, $intOnly, $jexamXml, $jexamCfg) {
  global $testCounter;
  global $failedTestCounter;
  foreach ($tests as $test) {
    $testCounter++;
    $srcFile = $test->getPath() . ".src";
    $inFile = $test->getPath() . ".in";
    $passed = false;
    $expRetCode = $test->getExpectedRC();
    $actRetCode = "";
    $outputFile = $test->getPath() . "tmp.out";
    $referenceOutputFile = $test->getPath() . ".out";
    $outputFileXML = $test->getPath() . "xmltmp.out";

    //&*RETURN CODE

    if ($parseOnly) {
      //^------Testing only parser------
      exec("touch " . $outputFile);
      //&Getting actual return code
      exec("php7.4 " . $parseScript . " < " . $srcFile . " > " . $outputFile, $parserdump, $actRetCode);
    } else if ($intOnly) {
      //^------Testing only interpret------
      exec("python3.8 " . $intScript . " --source=" . $srcFile . " --input=" . $inFile . " > " . $outputFile, $parserdump,  $actRetCode);
    } else {
      exec("touch " . $outputFileXML);
      //^------Testing parser and interpret------
      exec("php7.4 " . $parseScript . " < " . $srcFile . " > " . $outputFileXML, $parserdump, $actRetCode);
      if ($actRetCode == 0) {
        //&Parser printed out XML representation of code, feed it to interpret
        exec("python3.8 " . $intScript . " --source=" . $outputFileXML . " --input=" . $inFile . " > " . $outputFile, $parserdump,  $actRetCode);
        unlink($outputFileXML);
      } else {
        //&If parser failed, test is completed as failed 
        $actRetCode = strval($actRetCode);
        $test->setActualRC($actRetCode);
        $test->setPassed(false);
        $test->setError("Parsing XML has failed!");
        $failedTestCounter++;
        unlink($outputFileXML);
        unlink($outputFile);
        continue;
      }
    }
    $actRetCode = strval($actRetCode);
    $test->setActualRC($actRetCode);
    //&*OUTPUT
    if ($expRetCode != $actRetCode) {
      //&Return codes dont match
      $failedTestCounter++;
      $test->setPassed(false);
      $test->setError("Return codes dont match");
      unlink($outputFile);
      continue;
    } else if ($actRetCode == 0) {
      //&Return codes match and are zero, need to test output
      if ($parseOnly) {
        //?Testing XML 
        //~Comparing via JExamXML
        exec("java -jar " . $jexamXml . " " . $outputFile . " " . $referenceOutputFile . " /dev/null /D " . $jexamCfg, $parserdump, $diffOutput);
        if ($diffOutput == 1 || $diffOutput == 2) {
          //?Outputs dont match, test failed 
          $failedTestCounter++;
          $test->setPassed(false);
          $test->setError("Outputs dont match");
          unlink($outputFile);
          continue;
        } else if ($diffOutput != 0) {
          //?JexamXML failed 
          fprintf(STDERR, "ERROR: Error executing JExamXML comparisons\n");
          unlink($outputFile);
          exit(99);
        } else {
          $test->setPassed(true);
        }
      } else { //?Testing output of interpret 
        //~Comparing via diff
        exec("diff " . $outputFile . " " . $referenceOutputFile, $diffdump, $diffOutput);
        if ($diffOutput != 0) {
          //?Outputs dont match, test failed 
          $failedTestCounter++;
          $test->setPassed(false);
          $test->setError("Outputs dont match");
          unlink($outputFile);
          continue;
        } else {
          $test->setPassed(true);
        }
      }
    } else {
      //? Return codes are same and not zero 
      //?No need to check output
      $test->setPassed(true);
    }
    //&*END OF TESTING - DELETING TMP FILES
    unlink($outputFile);
  }
}

class Test {
  /**
   * Class representing test, contains info about return codes, path to file and if test passed
   */
  private $path = "";
  private $passed = false;
  private $expectedRC = "";
  private $actualRC = "";
  private $error = "None";

  function __construct($path, $expectedRC) {
    $this->path = $path;
    $this->expectedRC = $expectedRC;
  }

  public function setPassed($boolean) {
    $this->passed = $boolean;
  }
  public function getPassed() {
    return $this->passed;
  }
  public function getActualRC() {
    return $this->actualRC;
  }
  public function setActualRC($actualRC) {
    $this->actualRC = $actualRC;
  }
  public function getExpectedRC() {
    return $this->expectedRC;
  }
  public function setexpectedRC($expectedRC) {
    $this->expectedRC = $expectedRC;
  }
  public function getError() {
    return $this->error;
  }
  public function setError($error) {
    $this->error = $error;
  }
  public function getPath() {
    return $this->path;
  }
  public function setPath($path) {
    $this->path = $path;
  }
}

/**
 * Prints HTML summary of tested files
 *
 * @param array   $tests        array with paths to test files
 * @param string  $directory    path to directory with test files
 * @param string  $parseScript  path to parser script
 * @param string  $intScript    path to interpret script
 * @param bool    $parseOnly    for testing only parser
 * @param bool    $intOnly      for testing only interpret 
 */
function printHTML($tests, $directory, $parseOnly, $intOnly, $parseScript, $intScript) {
  global $testCounter;
  global $failedTestCounter;
  if ($testCounter == 0) {
    $successRate = "Not enough tests to determine!";
  } else {
    $successfulTests = $testCounter - $failedTestCounter;
    $successRate = $successfulTests * 100 / $testCounter;
    $successRate = $successRate . " % ";
  }
  $typeofTests = "Parser and interpret";
  if ($parseOnly) {
    $typeofTests = "Parser";
  } else if ($intOnly) {
    $typeofTests = "Interpret";
  }
  //Head
  date_default_timezone_set("Europe/Prague");
  $today = date("d.m.Y H:i:s");
  echo ("<!DOCTYPE html>\n <html>\n<head>\n<h1 style=\"text-align: center;\"><span style=\"background-color: #ffff99;\" data-darkreader-inline-bgcolor=\"\">Test results for IPPcode21</span>&nbsp;&nbsp;</h1>\n<h2 style=\"text-align: center;\"><span style=\"background-color: #ffff99;\" data-darkreader-inline-bgcolor=\"\">EXECUTED AT: " . $today . "</span></h2>\n<style>\ntable {\n  font-family: arial, sans-serif;\n  border-collapse: collapse;\n  width: 100%;\n}\ntd, th {\n  border: 1px solid #dddddd;\n  text-align: left;\n  padding: 8px;\n}\ntr:nth-child(even) {\n  background-color: #dddddd;\n}\n</style> </head>");
  //Body
  echo ("<body>\n<h3 style=\"text-align: center;\"><span style=\"--darkreader-inline-bgcolor: #262a2b; background-color: #ffff00;\" data-darkreader-inline-bgcolor=\"\"><strong>TEST VARIABLES</strong></span></h3>\n<ul style=\"list-style-type: square;\">\n
  <li><strong>Tested:  $typeofTests</strong></li>\n
  <li><strong>Searched directory: </strong>$directory</li>\n
  <li><strong><strong>Parser script:&nbsp;</strong></strong>$parseScript</li>\n
  <li><strong>Interpret script:&nbsp;</strong>$intScript</li>\n
  </ul>\n
  <h3 style=\"text-align: center;\"><span style=\"background-color: #ffff00;\" data-darkreader-inline-bgcolor=\"\"><strong>TEST STATISTICS</strong></span></h3>\n
  <ul style=\"list-style-type: square;\">\n
  <li><strong>Total test count: </strong>$testCounter</li>\n
  <li><strong>Failed test count: </strong>$failedTestCounter</li>\n
  <li><strong>Success rate: </strong>$successRate</li>\n
  </ul>\n");
  //Table - FAILED 
  echo ("<h2 style=\"text-align: center;\"><span style=\"background-color: #ff0000;\">Failed tests</span></h2>\n
  <table class=\"table\" style=\"margin-left: auto; margin-right: auto;\">\n
  <tbody>\n
  <tr>\n
  <td><strong>No.</strong></td>\n
  <td><strong>Path</strong></td>\n
  <td><strong>Expected RC</strong></td>\n
  <td><strong>Actual RC</strong></td>\n
  <td><strong>Error</strong></td>\n
  </tr>");
  // Failed tests 
  $testNo = 0;
  foreach ($tests as $test) {
    if ($test->getPassed() == false) {
      $testNo++;
      $path = $test->getPath();
      $eRC = $test->getExpectedRC();
      $aRC = $test->getActualRC();
      $err = $test->getError();
      fprintf(
        STDOUT,
        "<tr>\n"
          . "<td>$testNo</td>\n"
          . "<td>$path</td>\n"
          . "<td>$eRC</td>\n"
          . "<td>$aRC</td>\n"
          . "<td>$err</td>\n"
          . "</tr>\n"
      );
    }
  } ///Failed tests
  fprintf(
    STDOUT,
    "</tr>\n"
      . "</tbody>\n"
      . "</table>\n"
      . "<p>&nbsp;</p>"
  );
  //Successful tests 
  fprintf(
    STDOUT,
    "<h2 style=\"text-align: center;\"><span style=\"background-color: #00ff00;\">Succesful tests</span></h2>\n"
      . "<table class=\"table\" style=\"margin-left: auto; margin-right: auto;\">\n"
      . "<tbody>\n"
      . "<tr>\n"
      . "<td><strong>No.</strong></td>\n"
      . "<td><strong>Path&nbsp;</strong></td>\n"
      . "<td><strong>Return code</strong></td>\n"
      . "</tr>\n"
  );
  $testNo = 0;
  foreach ($tests as $test) {
    if ($test->getPassed() == true) {
      $testNo++;
      $path = $test->getPath();
      $rC = $test->getExpectedRC();
      fprintf(
        STDOUT,
        "<tr>\n"
          . "<td>$testNo</td>\n"
          . "<td>$path</td>\n"
          . "<td>$rC</td>\n"
          . "</tr>\n"
      );
    }
  }
  ///Successful tests 
  fprintf(
    STDOUT,
    "</tr>\n"
      . "</tbody>\n"
      . "</table>\n"
      . "<p>&nbsp;</p>"
      . "</body>\n"
      . "</html>\n"
  );
}
