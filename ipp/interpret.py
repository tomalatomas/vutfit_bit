#!/usr/bin/env python3.8

"""
file: interpret.py
author: Tomas Tomala (xtomal02)
last modification: 18.04.2021
"""

import sys
import re
import os
import argparse
import xml.etree.ElementTree as ET

opcodes = ["MOVE", "CREATEFRAME", "PUSHFRAME",
           "POPFRAME", "DEFVAR", "CALL", "RETURN",
           "PUSHS", "POPS", "ADD", "SUB", "MUL",
           "IDIV", "LT", "GT", "EQ", "AND", "OR",
           "NOT", "INT2CHAR", "STRI2INT", "READ",
           "WRITE", "CONCAT", "STRLEN", "GETCHAR",
           "SETCHAR", "TYPE", "LABEL", "JUMP", "JUMPIFEQ",
           "JUMPIFNEQ", "EXIT", "DPRINT", "BREAK"
           ]
types = ["var", "label", "string", "bool", "int", "nil", "type"]


def parseArgs():
    """
    Parsing arguments for source and input files
    :return: path to source and input file
    """ 
    if len(sys.argv) > 3:
        sys.stderr.write("You have specified too many arguments"+"\n")
        sys.exit(10)
    if len(sys.argv) < 2:
        sys.stderr.write("You have specified not enough arguments"+"\n")
        sys.exit(10)
    # Load arguments
    parser = argparse.ArgumentParser(
        description="Interpret for IPPCode21", add_help=False)
    parser.add_argument("--help", action="store_true")
    parser.add_argument("--source", type=str, metavar="FILE", )
    parser.add_argument("--input", type=str, metavar="FILE")
    arguments = parser.parse_args()
    if arguments.help == 1:
        if len(sys.argv) != 2:
            print("Argument --help cannot be combined with other parameters"+"\n")
            sys.exit(10)
        else:
            print("""-----------------
Interpret for IPPCode21
-----------------
USAGE: interpret.py [--help] [--source=file] [--input=file]
-----------------
ARGUMENTS:
--help          | show this help message and exit, cannot be combined with other arguments
--source=<file> | file with XML representation of IPPCode21
--input=<file>  | file with input for interpretation of source code
                | Atleast one of last two arguments must be always specified, if one of these arguments is not specified, interpret loads them from standart input"""+"\n")
            sys.exit(0)
    if arguments.source is None and arguments.input is None:
        sys.stderr.write(
            "ERROR: Atleast one of input or source arguments must be specified"+"\n")
        sys.exit(10)
    if arguments.input:
        # Check if file for interpret input exists
        if not os.path.exists(arguments.input):
            sys.stderr.write(
                "ERROR: File with input for interpret doesnt exist"+"\n")
            sys.exit(11)
    else:
        arguments.input = ""
    if arguments.source:
        # Check if file for interpret source exists
        if not os.path.exists(arguments.source):
            sys.stderr.write("ERROR: File with source code doesnt exist"+"\n")
            sys.exit(11)
    else:
        arguments.source = ""
    return arguments.source, arguments.input


class Instruction:
    """
    Class that emulates instruction
    """ 
    def __init__(self, order, opcode, arg1, arg2, arg3):
        self.order = order
        self.opcode = opcode
        self.arg1 = arg1
        self.arg2 = arg2
        self.arg3 = arg3


class Variable:
    """
    Class that emulates variable
    """ 
    def __init__(self, name, type=None, value=None):
        self.name = name
        self.type = type
        self.value = value


class Stack:
    """
    Class that emulates stack
    """ 
    def __init__(self):
        self.contents = []

    def init(self):
        self.contents = []

    def isInit(self):
        return self.contents is not None

    def deinit(self):
        self.contents = None

    def pop(self):
        if self.size() == 0:
            return None
        return self.contents.pop()

    def push(self, variable):
        self.contents.append(variable)

    def size(self):
        return len(self.contents)

    def top(self):
        if self.size() == 0:
            return None
        return self.contents[self.size()-1]

    def isEmpty(self):
        if self.size() == 0:
            return True
        return False


def removeEscapeSequences(string):
    """
    Removes XML escaping sequences
    :param string: string to modify
    :return: modified string
    """ 
    if string is None:
        return string
    return re.sub(r"\\(\d\d\d)", lambda x: chr(int(x.group(1), 10)), string)

def parseXML(argSource):
    """
    Goes through XML representation of code and gathers information about instructions
    :param argSource: path to source file
    :return: list of instructions
    """ 
    if argSource == "":
        sourceFile = sys.stdin
    else:
        sourceFile = open(argSource, "r")
    try:
        tree = ET.parse(argSource)
    except:
        sys.stderr.write("XML ERROR: Cannot parse XML source\n")
        sys.exit(31)
    root = tree.getroot()
    # *Header check
    if root.tag != "program" or "language" not in root.attrib:
        sys.stderr.write("XML ERROR: XML Root is not valid\n")
        sys.exit(32)
    if root.attrib["language"] != "IPPcode21":
        sys.stderr.write("XML ERROR: Uknown IPPcode language\n")
        sys.exit(32)
    # *Instruction checking
    instructions = []
    usedOrders = []
    for child in root:
        try:
            opcode = child.attrib["opcode"].upper()
            order = child.attrib["order"]
        except KeyError:
            sys.stderr.write("XML ERROR: Missing instruction information\n")
            sys.exit(32)  
        if child.tag != "instruction":
            sys.stderr.write("XML ERROR: Wrong instruction tag\n")
            sys.exit(32)
        if opcode not in opcodes:
            sys.stderr.write("XML ERROR: Unknown opcode attribute\n")
            sys.exit(32)
        if not order.isdigit():
            sys.stderr.write("XML ERROR: Order is not digit\n")
            sys.exit(32)

        order = int(order)
        if order in usedOrders or order < 1:
            sys.stderr.write("XML ERROR: Duplicate or negative order number\n")
            sys.exit(32)
        usedOrders.append(order)
        arg1 = None
        arg2 = None
        arg3 = None
        # &Argument checking
        if len(child) > 3:
            sys.stderr.write("XML ERROR: Excess arguments of instruction no. "+str(order)+"\n")
            sys.exit(32)
        for argument in child:
            if "type" not in argument.attrib:
                sys.stderr.write("XML ERROR: Missing type of argument of instruction no. "+str(order)+"\n")
                sys.exit(32)
            if argument.attrib["type"] not in types:
                sys.stderr.write("XML ERROR: Unknown type of argument of instruction no. "+str(order)+"\n")
                sys.exit(32)
            if len(argument.attrib) != 1:
                sys.stderr.write("XML ERROR: Wrong argument attribute structure of instruction no. "+str(order)+"\n")
                sys.exit(32)
            if argument.tag == "arg1":
                if arg1 is not None:
                    sys.stderr.write("XML ERROR: Duplicate arg1 of instruction no. "+str(order)+"\n")
                    sys.exit(32)
                arg1 = {}
                arg1["type"] = argument.attrib["type"]
                if arg1["type"] == "string":
                    if argument.text is None:
                        arg1["value"] = ""
                    else:
                        arg1["value"] = removeEscapeSequences(argument.text)
                else:
                    arg1["value"] = argument.text
            elif argument.tag == "arg2":
                if arg2 is not None:
                    sys.stderr.write("XML ERROR: Duplicate arg2 of instruction no. "+str(order)+"\n")
                    sys.exit(32)
                arg2 = {}
                arg2["type"] = argument.attrib["type"]
                if arg2["type"] == "string":
                    if argument.text is None:
                        arg2["value"] = ""
                    else:
                        arg2["value"] = removeEscapeSequences(argument.text)
                else:
                    arg2["value"] = argument.text
            elif argument.tag == "arg3":
                if arg3 is not None:
                    sys.stderr.write("XML ERROR: Duplicate arg3 of instruction no. "+str(order)+"\n")
                    sys.exit(32)
                arg3 = {}
                arg3["type"] = argument.attrib["type"]
                if arg3["type"] == "string":
                    if argument.text is None:
                        arg3["value"] = ""
                    else:
                        arg3["value"] = removeEscapeSequences(argument.text)
                else:
                    arg3["value"] = argument.text
            else:
                sys.stderr.write( "XML ERROR: Unknown argument tag of instruction no. "+str(order)+"\n")
                sys.exit(32)
        if (arg2 is not None and arg1 is None) or (arg3 is not None and (arg1 is None or arg2 is None)):
                sys.stderr.write( "XML ERROR: Invalid argument structure of instruction no. "+str(order)+"\n")
                sys.exit(32)
        instruction = Instruction(order, opcode, arg1, arg2, arg3)
        # &Create instruction
        if opcode == "LABEL":
            executeLabel(instruction)
        instructions.append(instruction)
    # *Sort instructions by instruction order
    instructions.sort(key=lambda x: x.order, reverse=False)
    sourceFile.close()
    return instructions

def checkIfLabelExists(labelName):
    for label in labels:
        if label["name"] == labelName:
            return label["order"]
    return -1

def executeLabel(instruction):
    """
    Creates label if it doesnt exits and checks lexical and semantic analysis
    :param instruction: called instruction
    """ 
    # Searching all label for two pass
    label = {}
    label["order"] = instruction.order
    label["name"] = instruction.arg1["value"]
    # ~Check arguments
    if instruction.arg2 or instruction.arg3 or instruction.arg1 is None:
        sys.stderr.write("SEM ERROR: Excess arguments of \"LABEL\" instruction no. " + str(instruction.order)+"\n")
        sys.exit(32)
    # ~Checking argument type
    if instruction.arg1["type"] != "label":
        sys.stderr.write("SEM ERROR: Invalid label type (\"" + instruction.arg1["type"]+"\") of instruction no. "+str(instruction.order)+"\n")
        sys.exit(52)
    # ~Checking label name
    if re.fullmatch(r"^[a-zA-Z%!?&_*$-][a-zA-Z%!?&*_$0-9-]*$", instruction.arg1["value"]) == None:
        sys.stderr.write("SEM ERROR: Invalid label name (\"" + label["name"]+"\") of argument in instruction no. "+str(instruction.order)+"\n")
        sys.exit(32)
    # ~Checking duplicate label
    if checkIfLabelExists(label["name"]) != -1:
        sys.stderr.write("SEM ERROR: Duplicate label (\"" + label["name"]+"\") of argument in instruction no. "+str(instruction.order)+"\n")
        sys.exit(52)
    # ~Adding label to list of labels
    labels.append(label)


def checkType(argument, order):
    """
    Check correctness of value and type of Type
    :param argument: argument of functions
    :param order: order of instruction
    """ 
    if argument["type"] != "type":
        sys.stderr.write("SEM ERROR: Invalid type of argument in instruction no. " + str(order) + "Expected: \"type\", Actual: \""+argument["type"]+"\""+"\n")
        sys.exit(52)
    if re.fullmatch(r"^(int|bool|string)$", argument["value"]) == None:
        sys.stderr.write("SEM ERROR: Invalid type value (\"" + argument["value"]+"\") of argument in instruction no. "+str(order)+"\n")
        sys.exit(32)


def checkVar(argument, order):
    """
    Check correctness of value and type of Var
    :param argument: argument of functions
    :param order: order of instruction
    """ 
    if argument["type"] != "var":
        sys.stderr.write("SEM ERROR: Invalid type of argument in instruction no. " + str(order) + "Expected: \"var\", Actual: \""+argument["type"]+"\"")
        sys.exit(52)
    if re.fullmatch(r"^(LF|GF|TF)@[a-zA-Z%!?&_*$-][a-zA-Z%!?&*_$0-9-]*$", argument["value"]) == None:
        sys.stderr.write("SEM ERROR: Invalid argument value (\"" + argument["value"]+"\") of instruction no. "+str(order))
        sys.exit(32)


def checkSymb(argument, order):
    """
    Check correctness of value and type of Symb
    :param argument: argument of functions
    :param order: order of instruction
    """ 
    possibleTypes = ["string", "int", "bool", "nil", "var"]
    if argument["type"] not in possibleTypes:
        sys.stderr.write("SEM ERROR: Invalid type \"" + argument["type"]+"\" of argument in instruction no. "+str(order)+"\n")
        sys.exit(52)
    if argument["type"] == "var":
        checkVar(argument, order)
    elif argument["type"] == "bool":
        if argument["value"] != "true" and argument["value"] != "false":
            sys.stderr.write("SEM ERROR: Invalid argument value \"" + argument["value"]+"\" of argument "+argument["type"]+" in instruction no. "+str(order)+"\n")
            sys.exit(52)
    elif argument["type"] == "int":
        if re.fullmatch(r"^[+-]?[0-9]+$", argument["value"]) == None:
            sys.stderr.write("SEM ERROR: Invalid argument value (\"" + argument["value"]+"\") of instruction no. "+str(order)+"\n")
            sys.exit(32)
    elif argument["type"] == "nil":
        if argument["value"] != "nil":
            sys.stderr.write("SEM ERROR: Invalid argument value (\"" + argument["value"]+"\") of instruction no. "+str(order)+", Expected: \"nil\""+"\n")
            sys.exit(52)


def checkLabel(argument, order):
    """
    Check correctness of value and type of Label
    :param argument: argument of functions
    :param order: order of instruction
    """ 
    if argument["type"] != "label":
        sys.stderr.write("RUN ERROR: Invalid type of instruction no. "+str(order) + "Expected: \"label\", Actual: \""+argument["type"]+"\""+"\n")
        sys.exit(52)
    if re.fullmatch(r"^[a-zA-Z%!?&_*$-][a-zA-Z%!?&*_$0-9-]*$", argument["value"]) == None:
        sys.stderr.write("SEM ERROR: Invalid type value (\"" + argument["value"]+"\") of argument in instruction no. "+str(order)+"\n")
        sys.exit(32)


def checkInstrArgs(instruction):
    """
    Semantic analysis of instruction
    :param instruction: instruction to be analyzed
    """ 
    noArgs = ["CREATEFRAME", "PUSHFRAME", "POPFRAME", "RETURN", "BREAK"]
    var = ["POPS", "DEFVAR"]
    symb = ["PUSHS", "WRITE", "EXIT", "DPRINT"]
    lbl = ["CALL", "LABEL", "JUMP"]
    varSymb = ["INT2CHAR", "STRLEN", "TYPE", "MOVE", "NOT"]
    varSymbs = ["ADD", "SUB", "MUL", "IDIV", "LT", "GT", "EQ",
                "AND", "OR", "STRI2INT", "CONCAT", "GETCHAR", "SETCHAR"]
    lblSymbs = ["JUMPIFEQ", "JUMPIFNEQ"]
    varType = ["READ"]
    if instruction.opcode in noArgs:
        # ?Excess of arguments
        if instruction.arg1 is not None or instruction.arg2 is not None or instruction.arg3 is not None:
            sys.stderr.write("SEM ERROR: Excess arguments of instruction no. " + str(instruction.order)+"\n")
            sys.exit(32)
    if instruction.opcode in var:
        # *⟨var⟩
        # ?Excess of arguments
        if instruction.arg2 is not None or instruction.arg3 is not None:
            sys.stderr.write("SEM ERROR: Excess arguments of instruction no. " + str(instruction.order)+"\n")
            sys.exit(32)
        # ?Lack of arguments
        if instruction.arg1 is None:
            sys.stderr.write("SEM ERROR: Lack of arguments of instruction no. " + str(instruction.order)+"\n")
            sys.exit(32)
        # ?Checking type and value
        checkVar(instruction.arg1, instruction.order)
    if instruction.opcode in symb:
        # *⟨symb⟩
        # ?Excess of arguments
        if instruction.arg2 is not None or instruction.arg3 is not None:
            sys.stderr.write("SEM ERROR: Excess arguments of instruction no. " + str(instruction.order)+"\n")
            sys.exit(32)
        # ?Lack of arguments
        if instruction.arg1 is None:
            sys.stderr.write("SEM ERROR: Lack of arguments of instruction no. " + str(instruction.order)+"\n")
            sys.exit(32)
        # ?Checking type and value
        checkSymb(instruction.arg1, instruction.order)
    if instruction.opcode in lbl:
        # *⟨label⟩
        # ?Excess of arguments
        if instruction.arg2 is not None or instruction.arg3 is not None:
            sys.stderr.write("SEM ERROR: Excess arguments of instruction no. " + str(instruction.order)+"\n")
            sys.exit(32)
        # ?Lack of arguments
        if instruction.arg1 is None:
            sys.stderr.write("SEM ERROR: Lack of arguments of instruction no. " + str(instruction.order)+"\n")
            sys.exit(32)
        # ?Checking type and value
        checkLabel(instruction.arg1, instruction.order)
    if instruction.opcode in varSymb:
        # *⟨var⟩ ⟨symb⟩
        # ?Excess of arguments
        if instruction.arg3 is not None:
            sys.stderr.write("SEM ERROR: Excess arguments of instruction no. " + str(instruction.order)+"\n")
            sys.exit(32)
        # ?Lack of arguments
        if instruction.arg1 is None or instruction.arg2 is None:
            sys.stderr.write("SEM ERROR: Lack of arguments of instruction no. " + str(instruction.order)+"\n")
            sys.exit(32)
        # ?Checking type and value
        checkVar(instruction.arg1, instruction.order)
        checkSymb(instruction.arg2, instruction.order)
    if instruction.opcode in varSymbs:
        # *⟨var⟩ ⟨symb1⟩ ⟨symb2⟩
        # ?Lack of arguments
        if instruction.arg1 is None or instruction.arg2 is None or instruction.arg3 is None:
            sys.stderr.write("SEM ERROR: Lack of arguments of instruction no. " + str(instruction.order)+"\n")
            sys.exit(32)
        # ?Checking type and value
        checkVar(instruction.arg1, instruction.order)
        checkSymb(instruction.arg2, instruction.order)
        checkSymb(instruction.arg3, instruction.order)
    if instruction.opcode in lblSymbs:
        # *⟨label⟩ ⟨symb1⟩ ⟨symb2⟩
        # ?Lack of arguments
        if instruction.arg1 is None or instruction.arg2 is None or instruction.arg3 is None:
            sys.stderr.write("SEM ERROR: Lack of arguments of instruction no. " + str(instruction.order)+"\n")
            sys.exit(32)
        # ?Checking type and value
        checkLabel(instruction.arg1, instruction.order)
        checkSymb(instruction.arg2, instruction.order)
        checkSymb(instruction.arg3, instruction.order)
    if instruction.opcode in varType:
        # *⟨var⟩ ⟨type⟩
        # ?Excess of arguments
        if instruction.arg3 is not None:
            sys.stderr.write("SEM ERROR: Excess arguments of instruction no. " + str(instruction.order)+"\n")
            sys.exit(32)
        # ?Lack of arguments
        if instruction.arg1 is None or instruction.arg2 is None:
            sys.stderr.write("SEM ERROR: Lack of arguments of instruction no. " + str(instruction.order)+"\n")
            sys.exit(32)
        # ?Checking type and value
        checkVar(instruction.arg1, instruction.order)
        checkType(instruction.arg2, instruction.order)


class Program:
    """
    Class that emulates program, contains stacks, frames and emulates instructions
    """ 
    def __init__(self, input):
        # STACKS
        self.frameStack = Stack()
        self.dataStack = Stack()
        # Contains index of call instruction to be able to come back from jump
        self.callStack = Stack()
        # FRAMES
        self.tempFrame = None
        self.localFrame = None
        self.globalFrame = []
        if input == "":
            self.inputFile = sys.stdin
        else:
            self.inputFile = open(input, "r")

    def checkIfVarExists(self, variableName, instruction):
        """
        Checks if variable exists in given frame
        :param variableName: name of variable with frame
        :param instruction: called instruction
        :return: found variable
        """ 
        splitted = variableName.split("@", 1)
        frameToSearch = None
        if splitted[0] == "TF":
            frameToSearch = self.tempFrame
        elif splitted[0] == "LF":
            frameToSearch = self.localFrame
        elif splitted[0] == "GF":
            frameToSearch = self.globalFrame
        else:
            sys.stderr.write("RUN ERROR: Invalid frame \"" + splitted[0]+"\" "+"\n")
            sys.exit(52)
        if frameToSearch is None:
            sys.stderr.write("RUN ERROR: Reference to variable in undefined frame. " + str(instruction.order)+"\n")
            sys.exit(55)
        for var in frameToSearch:
            if var.name == splitted[1]:
                return var
        return None



    def getSymbTypeValue(self, argument, instruction):
        """
        If symb is variable, return variable, else retuns constant
        :param variableName: name of variable with frame
        :param instruction: called instruction
        :return: constant or variable
        """ 
        arg = {}
        var = None
        if argument["type"] == "var":
            var = self.checkIfVarExists(argument["value"], instruction)
            if var is None:
                sys.stderr.write(
                    "RUN ERROR: Undefined variable "+argument["value"]+"\n")
                sys.exit(54)
            arg["type"] = var.type
            arg["value"] = var.value
        else:
            arg = argument
        return arg

    def instrMove(self, instruction):
        """
        Emulating function Move
        :param instruction: instruction called
        """ 
        destVar = self.checkIfVarExists(instruction.arg1["value"], instruction)
        if destVar is None:
            sys.stderr.write("RUN ERROR: Destination variable doesnt exist in instruction no. " + str(instruction.order)+"\n")
            sys.exit(54)
        symb = instruction.arg2
        if symb["type"] == "var":
            sourceVar = self.checkIfVarExists(instruction.arg2["value"], instruction)
            if sourceVar is None:
                sys.stderr.write("RUN ERROR: Source variable doesnt exist in instruction no. " + str(instruction.order)+"\n")
                sys.exit(54)
            if  sourceVar.type is None or sourceVar.value is None:
                sys.stderr.write("RUN ERROR: Undefined value in boolean instruction no. " + str(instruction.order)+"\n")
                sys.exit(56)
            destVar.type = sourceVar.type
            destVar.value = sourceVar.value
        else:
            destVar.type = instruction.arg2["type"]
            destVar.value = instruction.arg2["value"]

    def instrCreateframe(self, instruction):
        """
        Emulating function Createframe
        :param instruction: instruction called
        """
        # Creates temporary frame
        self.tempFrame = []

    def instrPushframe(self, instruction):
        """
        Emulating function Pushframe
        :param instruction: instruction called
        """ 
        # Pushes temporary frame into frameStack, clears temporary frame
        if self.tempFrame is None:
            sys.stderr.write("RUN ERROR: Pushing undefined frame in instruction no. " + str(instruction.order)+"\n")
            sys.exit(55)
        self.frameStack.push(self.tempFrame)
        self.localFrame = self.frameStack.top()
        self.tempFrame = None

    def instrPopframe(self, instruction):
        """
        Emulating function Popframe
        :param instruction: instruction called
        """ 
        if self.localFrame is None:
            sys.stderr.write("RUN ERROR Poping undefined frame in instruction no. " + str(instruction.order)+"\n")
            sys.exit(55)
        self.tempFrame = self.frameStack.pop()
        self.localFrame = self.frameStack.top()

    def instrCall(self, instruction, currentInstructionCounter):
        """
        Emulating function Call
        :param instruction: instruction called
        """ 
        self.callStack.push(currentInstructionCounter+1)
        labelIndex = checkIfLabelExists(instruction.arg1["value"])
        if labelIndex == -1:
            sys.stderr.write("RUN ERROR: Jump to undefined label in instruction no. " + str(currentInstructionCounter)+"\n")
            sys.exit(52)
        return labelIndex

    def instrDefvar(self, instruction):
        """
        Emulating function Defvar
        :param instruction: instruction called
        """
        var = self.checkIfVarExists(instruction.arg1["value"], instruction)
        if var is not None:
            sys.stderr.write("RUN ERROR: Redefinition of variable (\"" + instruction.arg1["value"]+"\") in instruction no. "+str(instruction.order)+"\n")
            sys.exit(52)
        splitted = instruction.arg1["value"].split("@", 1)
        var = Variable(splitted[1])
        if splitted[0] == "TF":
            if self.tempFrame is None:
                sys.stderr.write("RUN ERROR: Var definition in undefined temporary frame in instruction no. " +str(instruction.order)+"\n")
                sys.exit(55)
            self.tempFrame.append(var)
        elif splitted[0] == "LF":
            if self.localFrame is None:
                sys.stderr.write("RUN ERROR: Var definition in undefined local frame in instruction no. " +str(instruction.order)+"\n")
                sys.exit(55)
            self.localFrame.append(var)
        elif splitted[0] == "GF":
            self.globalFrame.append(var)
    def instrPushs(self, instruction):
        """
        Emulating function Pushs
        :param instruction: instruction called
        """
        symb = instruction.arg1
        if symb["type"] == "var":
            var = self.checkIfVarExists(symb["value"], instruction)
            if var is None:
                sys.stderr.write("RUN ERROR: Undefined variable in instruction no. " +str(instruction.order)+"\n")
                sys.exit(54)
            if var.type is None or var.value is None :
                sys.stderr.write("RUN ERROR: Popped empty stack in instruction no. " + str(instruction.order)+"\n")
                sys.exit(56)
            toPush = {}
            toPush["type"] = var.type
            toPush["value"] = var.value
            self.dataStack.push(toPush)
        else:
            self.dataStack.push(symb)
    def instrPops(self, instruction):
        """
        Emulating function Pops
        :param instruction: instruction called
        """
        var = self.checkIfVarExists(instruction.arg1["value"], instruction)
        if var is None:
            sys.stderr.write("RUN ERROR: Undefined variable in instruction no. " + str(instruction.order)+"\n")
            sys.exit(54)
        symb = self.dataStack.pop()
        if symb is None:
            sys.stderr.write("RUN ERROR: Popped empty stack in instruction no. " + str(instruction.order)+"\n")
            sys.exit(56)
        if symb["type"] is None:
            sys.stderr.write("RUN ERROR: Using unset value in instruction no. " + str(instruction.order)+"\n")
            sys.exit(56)
        var.type = symb["type"]
        var.value = symb["value"]

    def instrArithmetics(self, instruction):
        """
        Emulating functions ADD, SUB, MUL, IDIV
        :param instruction: instruction called
        """
        argument2 = self.getSymbTypeValue(instruction.arg2, instruction)
        argument3 = self.getSymbTypeValue(instruction.arg3, instruction)
        var = self.checkIfVarExists(instruction.arg1["value"], instruction)
        if var is None:
            sys.stderr.write("RUN ERROR: Undefined variable in instruction no. " + str(instruction.order)+"\n")
            sys.exit(54)
        if  argument2["value"] is None or argument3["value"] is None:
            sys.stderr.write("RUN ERROR: Undefined value in arithmetic instruction no. " + str(instruction.order)+"\n")
            sys.exit(56)
        if not argument2["type"] == "int" or not argument3["type"] == "int":
            sys.stderr.write("RUN ERROR: Noninteger value in arithmetic instruction no. " + str(instruction.order)+"\n")
            sys.exit(53)
        var.type = "int"
        if instruction.opcode == "ADD":
            var.value = str(int(argument2["value"]) + int(argument3["value"]))
        elif instruction.opcode == "SUB":
            var.value = str(int(argument2["value"]) - int(argument3["value"]))
        elif instruction.opcode == "MUL":
            var.value = str(int(argument2["value"]) * int(argument3["value"]))
        elif instruction.opcode == "IDIV":
            if int(argument3["value"]) == 0:
                sys.stderr.write("RUN ERROR: Division by zero in instruction no. " +str(instruction.order)+"\n")
                sys.exit(57)
            var.value = str(int(argument2["value"]) // int(argument3["value"]))

    def instrRelation(self, instruction):
        """
        Emulating functions EQ, LT, GT
        :param instruction: instruction called
        """
        possibleTypes = ["int","string","bool"]
        argument2 = self.getSymbTypeValue(instruction.arg2, instruction)
        argument3 = self.getSymbTypeValue(instruction.arg3, instruction)
        var = self.checkIfVarExists(instruction.arg1["value"], instruction)
        if var is None:
            sys.stderr.write("RUN ERROR: Undefined variable in instruction no. " + str(instruction.order)+"\n")
            sys.exit(54)
        var.type = "bool"

        if  argument2["value"] is None or argument3["value"] is None:
            sys.stderr.write("RUN ERROR: Undefined values in arguments instruction no. " + str(instruction.order)+"\n")
            sys.exit(56)
        if argument2["type"] == "nil" or argument3["type"] == "nil":
            if instruction.opcode != "EQ":
                sys.stderr.write("RUN ERROR: Comparing nils in instruction no. " +str(instruction.order)+"\n")
                sys.exit(53)
            else:
                if argument2["value"] == argument3["value"]:
                    var.value = "true"
                else:
                    var.value = "false"
                return
        else:
            if argument2["type"] != argument3["type"]:
                sys.stderr.write("RUN ERROR: Comparing different types in instruction no. " +str(instruction.order)+"\n")
                sys.exit(53)
            if argument2["type"] not in possibleTypes or argument3["type"] not in possibleTypes:
                sys.stderr.write("RUN ERROR: Comparing invalid types in instruction no. " +str(instruction.order)+"\n")
                sys.exit(53)

        arg2val =argument2["value"]
        arg3val = argument3["value"]
        if argument2["type"] == "int":
            arg2val =int(argument2["value"])
            arg3val = int(argument3["value"])
        elif argument2["type"] == "bool":
            if argument2["value"] == "false":
                arg2val = 0
            if argument3["value"] == "false":
                arg3val = 0
            if argument2["value"] == "true":
                arg2val = 1
            if argument3["value"] == "true":
                arg3val = 1

        if instruction.opcode == "EQ":
            if arg2val == arg3val:
                var.value = "true"
            else:
                var.value = "false"
        elif instruction.opcode == "GT":
            if arg2val > arg3val:
                var.value = "true"
            else:
                var.value = "false"
        elif instruction.opcode == "LT":
            if arg2val < arg3val:
                var.value = "true"
            else:
                var.value = "false"

    def instrBool(self, instruction):
        """
        Emulating functions AND OR
        :param instruction: instruction called
        """        
        argument2 = self.getSymbTypeValue(instruction.arg2, instruction)
        argument3 = self.getSymbTypeValue(instruction.arg3, instruction)
        var = self.checkIfVarExists(instruction.arg1["value"], instruction)
        if var is None:
            sys.stderr.write("RUN ERROR: Undefined variable in instruction no. " + str(instruction.order)+"\n")
            sys.exit(54)
        if  argument2["value"] is None or argument2["type"] is None or argument3["value"] is None or argument3["type"] is None:
            sys.stderr.write("RUN ERROR: Undefined value in boolean instruction no. " + str(instruction.order)+"\n")
            sys.exit(56)
        if not (argument2["type"] == "bool") or not (argument3["type"] == "bool"):
            sys.stderr.write("RUN ERROR: Non booleans in instruction no. " + str(instruction.order)+"\n")
            sys.exit(53)
        if argument2["value"] == "true":
            arg2 = True
        else:
            arg2 = False

        if argument3["value"] == "true":
            arg3 = True
        else:
            arg3 = False

        if instruction.opcode == "AND":
            boolean = arg2 and arg3
        elif instruction.opcode == "OR":
            boolean = arg2 or arg3

        var.type = "bool"
        if boolean == True:
            var.value = "true"
        else:
            var.value = "false"

    def instrBoolNot(self, instruction):
        """
        Emulating function NOT
        :param instruction: instruction called
        """        
        argument2 = self.getSymbTypeValue(instruction.arg2, instruction)
        var = self.checkIfVarExists(instruction.arg1["value"], instruction)
        if var is None:
            sys.stderr.write("RUN ERROR: Undefined variable in instruction no. " + str(instruction.order)+"\n")
            sys.exit(54)
        if  argument2["value"] is None or argument2["type"] is None :
            sys.stderr.write("RUN ERROR: Undefined value in boolean instruction no. " + str(instruction.order)+"\n")
            sys.exit(56)
        if not argument2["type"] == "bool":
            sys.stderr.write("RUN ERROR: Non boolean in instruction no. " + str(instruction.order)+"\n")
            sys.exit(53)
        var.type = "bool"
        if argument2["value"] == "true":
            var.value = "false"
        else:
            var.value = "true"

    def instrInt2Char(self, instruction):
        """
        Emulating function Int2Char
        :param instruction: instruction called
        """        
        argument2 = self.getSymbTypeValue(instruction.arg2, instruction)
        var = self.checkIfVarExists(instruction.arg1["value"], instruction)
        if var is None:
            sys.stderr.write("RUN ERROR: Undefined variable in instruction no. " + str(instruction.order)+"\n")
            sys.exit(54)
        if  argument2["value"] is None:
            sys.stderr.write("RUN ERROR: Undefined value in  instruction no. " + str(instruction.order)+"\n")
            sys.exit(56)
        if not argument2["type"] == "int":
            sys.stderr.write("RUN ERROR: Non integer argument in instruction no. " + str(instruction.order)+"\n")
            sys.exit(53)
        try:
            var.value = chr(int(argument2["value"]))
            var.type = "string"
        except:
            sys.stderr.write(
                "RUN ERROR: Invalid value in instruction no. " + str(instruction.order)+"\n")
            sys.exit(58)

    def instrStri2Int(self, instruction):
        """
        Emulating function Stri2Int
        :param instruction: instruction called
        """        
        argument2 = self.getSymbTypeValue(instruction.arg2, instruction)
        argument3 = self.getSymbTypeValue(instruction.arg3, instruction)
        var = self.checkIfVarExists(instruction.arg1["value"], instruction)
        if var is None:
            sys.stderr.write("RUN ERROR: Undefined variable in instruction no. " + str(instruction.order)+"\n")
            sys.exit(54)
        if  argument2["value"] is None or argument3["value"] is None:
            sys.stderr.write("RUN ERROR: Undefined value in boolean instruction no. " + str(instruction.order)+"\n")
            sys.exit(56)
        if not (argument2["type"] == "string") or not (argument3["type"] == "int"):
            sys.stderr.write(
                "RUN ERROR: Invalid types of operands in instruction no. " + str(instruction.order)+"\n")
            sys.exit(53)
        string = argument2["value"]
        position = int(argument3["value"])
        if position >= len(string) or position < 0:
            sys.stderr.write(
                "RUN ERROR: Position operand out of range of string operand in instruction no. " + str(instruction.order)+"\n")
            sys.exit(58)
        try:
            var.value = str(ord(string[position]))
            var.type = "int"
        except:
            sys.stderr.write("RUN ERROR: Invalid value in instruction no. " + str(instruction.order)+"\n")
            exit(58)

    def instrRead(self, instruction):
        """
        Emulating function Read
        :param instruction: instruction called
        """
        var = self.checkIfVarExists(instruction.arg1["value"], instruction)
        inputValue = self.inputFile.readline()
        typeArg = instruction.arg2["value"]
        if var is None:
            sys.stderr.write("RUN ERROR: Undefined variable in instruction no. " + str(instruction.order)+"\n")
            sys.exit(54)
        if inputValue == "":
            var.type = "nil"
            var.value = "nil"
        else:
            inputValue = inputValue.strip()
            var.type = typeArg
            if typeArg == "int":
                if re.fullmatch(r"^[+-]?[0-9]+$", inputValue) == None:
                    var.type = "nil"
                    var.value = "nil"
                var.value = inputValue
            elif typeArg == "nil":
                if var.value != "nil":
                    var.type = "nil"
                    var.value = "nil"
                var.value = inputValue
            elif typeArg == "string":
                var.value = inputValue
            elif typeArg == "bool":
                if inputValue.lower() == "true":
                    var.value = "true"
                else:
                    var.value = "false"

    def instrWrite(self, instruction):
        """
        Emulating function Write
        :param instruction: instruction called
        """        
        argument = self.getSymbTypeValue(instruction.arg1, instruction)
        if  argument["value"] is None or argument["type"] is None:
            sys.stderr.write("RUN ERROR: Undefined value in instruction no. " + str(instruction.order)+"\n")
            sys.exit(56)
        if argument["type"] == "nil":
            print("", end="")
        else:
            toPrint = removeEscapeSequences(argument["value"])
            print(toPrint, end="")

    def instrConcat(self, instruction):
        """
        Emulating function Concat
        :param instruction: instruction called
        """
        argument2 = self.getSymbTypeValue(instruction.arg2, instruction)
        argument3 = self.getSymbTypeValue(instruction.arg3, instruction)
        var = self.checkIfVarExists(instruction.arg1["value"], instruction)
        if var is None:
            sys.stderr.write("RUN ERROR: Undefined variable in instruction no. " + str(instruction.order)+"\n")
            sys.exit(54)
        if  argument2["value"] is None or argument3["value"] is None:
            sys.stderr.write("RUN ERROR: Undefined value in boolean instruction no. " + str(instruction.order)+"\n")
            sys.exit(56)
        if not (argument2["type"] == "string") or not (argument3["type"] == "string"):
            sys.stderr.write("RUN ERROR: Non string arguments in instruction no. " + str(instruction.order)+"\n")
            sys.exit(53)
        var.type = "string"
        var.value = argument2["value"] + argument3["value"]

    def instrStrlen(self, instruction):
        """
        Emulating function Strlen
        :param instruction: instruction called
        """
        argument = self.getSymbTypeValue(instruction.arg2, instruction)
        var = self.checkIfVarExists(instruction.arg1["value"], instruction)
        if var is None:
            sys.stderr.write(
                "RUN ERROR: Undefined variable in instruction no. " + str(instruction.order)+"\n")
            sys.exit(54)
        if  argument["value"] is None or argument["type"] is None:
            sys.stderr.write("RUN ERROR: Undefined value in instruction no. " + str(instruction.order)+"\n")
            sys.exit(56)
        if argument["type"] != "string":
            sys.stderr.write(
                "RUN ERROR: Non string argument in instruction no. " + str(instruction.order)+"\n")
            sys.exit(53)
        var.type = "int"
        var.value = str(len(argument["value"]))

    def instrGetchar(self, instruction):
        """
        Emulating function Getchar
        :param instruction: instruction called
        """
        argument2 = self.getSymbTypeValue(instruction.arg2, instruction)
        argument3 = self.getSymbTypeValue(instruction.arg3, instruction)
        var = self.checkIfVarExists(instruction.arg1["value"], instruction)
        if var is None:
            sys.stderr.write("RUN ERROR: Undefined variable in instruction no. " + str(instruction.order)+"\n")
            sys.exit(54)
        if  argument2["value"] is None or argument3["value"] is None:
            sys.stderr.write("RUN ERROR: Undefined value in instruction no. " + str(instruction.order)+"\n")
            sys.exit(56)
        if not (argument2["type"] == "string") or not (argument3["type"] == "int"):
            sys.stderr.write("RUN ERROR: Invalid argument types in instruction no. " + str(instruction.order)+"\n")
            sys.exit(53)
        if (int(argument3["value"]) >= len(argument2["value"])) or (int(argument3["value"]) < 0):
            sys.stderr.write(
                "RUN ERROR: Position operand out of range of string operand in instruction no. " + str(instruction.order)+"\n")
            sys.exit(58)
        var.type = "string"
        var.value = argument2["value"][int(argument3["value"])]

    def instrSetchar(self, instruction):
        """
        Emulating function Setchar
        :param instruction: instruction called
        """        
        argument2 = self.getSymbTypeValue(instruction.arg2, instruction)
        argument3 = self.getSymbTypeValue(instruction.arg3, instruction)
        var = self.checkIfVarExists(instruction.arg1["value"], instruction)
        if var is None:
            sys.stderr.write("RUN ERROR: Undefined variable in instruction no. " + str(instruction.order)+"\n")
            sys.exit(54)
        if  argument2["value"] is None or argument3["value"] is None or var.type is None:
            sys.stderr.write("RUN ERROR: Undefined value in instruction no. " + str(instruction.order)+"\n")
            sys.exit(56)
        if not (argument2["type"] == "int") or not (argument3["type"] == "string") or not (var.type == "string"):
            sys.stderr.write("RUN ERROR: Invalid argument types in instruction no. " + str(instruction.order)+"\n")
            sys.exit(53)
        if int(argument2["value"]) >= len(var.value) or(int(argument2["value"]) < 0) or argument3["value"] == "":
            sys.stderr.write(
                "RUN ERROR: Position operand out of range of string operand in instruction no. " + str(instruction.order)+"\n")
            sys.exit(58)
        if int(argument2["value"]) >= len(var.value) or(int(argument2["value"]) < 0) or argument3["value"] == "":
            sys.stderr.write(
                "RUN ERROR: Position operand out of range of string operand in instruction no. " + str(instruction.order)+"\n")
            sys.exit(58) 
        stringlist = list(var.value)
        stringlist[int(argument2["value"])] = argument3["value"][0]
        var.value = "".join(stringlist) 

    def instrType(self, instruction):
        """
        Emulating function Type
        :param instruction: instruction called
        """        
        argument2 = self.getSymbTypeValue(instruction.arg2, instruction)
        var = self.checkIfVarExists(instruction.arg1["value"], instruction)
        if var is None:
            sys.stderr.write("RUN ERROR: Undefined variable in instruction no. " + str(instruction.order)+"\n")
            sys.exit(54)

        var.type = "string"
        if argument2["type"] is None:
            var.value = ""
        else:
            var.value = argument2["type"]

    def instrJump(self, instruction):
        """
        Emulating function Jump
        :param instruction: instruction called
        """        
        labelIndex = checkIfLabelExists(instruction.arg1["value"])
        if labelIndex == -1:
            sys.stderr.write("RUN ERROR: Jump to undefined label in instruction no. " + str(instruction.order)+"\n")
            sys.exit(52)
        return labelIndex

    def instrJumpIf(self, instruction):
        """
        Emulating function JumpIfEQ and jumpifNEQ
        :param instruction: instruction called
        """        
        labelIndex = checkIfLabelExists(instruction.arg1["value"])
        argument2 = self.getSymbTypeValue(instruction.arg2, instruction)
        argument3 = self.getSymbTypeValue(instruction.arg3, instruction)
        if labelIndex == -1:
            sys.stderr.write("RUN ERROR: Jump to undefined label in instruction no. " + str(instruction.order)+"\n")
            sys.exit(52)
        if  argument2["value"] is None or argument3["type"] is None:
            sys.stderr.write("RUN ERROR: Undefined value in  instruction no. " + str(instruction.order)+"\n")
            sys.exit(56)
        if argument2["type"] != argument3["type"] and (argument2["type"] != "nil" and argument3["type"] != "nil"):
            sys.stderr.write("RUN ERROR: Invalid argument types in instruction no. " + str(instruction.order)+"\n")
            sys.exit(53)
        if instruction.opcode == "JUMPIFEQ":
            if argument2["value"] == argument3["value"]:
                return labelIndex
        elif instruction.opcode == "JUMPIFNEQ":
            if argument2["value"] != argument3["value"]:
                return labelIndex
        return -1

    def instrExit(self, instruction):
        """
        Emulating function Exit
        :param instruction: instruction called
        """        
        argument1 = self.getSymbTypeValue(instruction.arg1, instruction)
        if  argument1["value"] is None or argument1["type"] is None:
            sys.stderr.write("RUN ERROR: Undefined value in exit instruction no. " + str(instruction.order)+"\n")
            sys.exit(56)
        if not (argument1["type"] == "int"):
            sys.stderr.write("RUN ERROR: Invalid argument types in instruction no. " + str(instruction.order)+"\n")
            sys.exit(53)
        if not (int(argument1["value"]) >= 0 and int(argument1["value"]) <= 49):
            sys.stderr.write("RUN ERROR: Invalid argument value in instruction no. " + str(instruction.order)+"\n")
            sys.exit(57)
        sys.exit(int(argument1["value"]))

    def instrDprint(self, instruction):
        """
        Emulating function Dprint
        :param instruction: instruction called
        """    
        argument1 = self.getSymbTypeValue(instruction.arg1, instruction)
        sys.stderr.write(argument1["value"])

    def printFrame(self, frame):
        """
        Printing info about frames
        :param frame: frame to printout
        """
        if frame is not None:
            for var in frame:
                varType = var.type
                varValue = var.value
                if var.type is None:
                    varType = ""
                if var.value is None:
                    varValue = ""
                sys.stderr.write("  Variable | name:"+var.name +" | type: "+varType+" | value:"+varValue+"\n")
        else:
            sys.stderr.write("  Frame not initialized"+"\n")

    def getPositionOfInstrOrder(self, instrOrder):
        position = 0
        for instruction in instructions:
            if instruction.order == instrOrder:
                return position
            position += 1
        sys.stderr.write("RUN ERROR: Jump to undefined label of order no. " + str(instrOrder)+"\n")
        sys.exit(52)

    def executeProgram(self, instructions):
        """
        Goes through every instruction found in xml and emulates it
        :param instructions: list of instructions
        """ 
        totalInstructionsInterpreted = 0
        currentInstructionCounter = 0
        while currentInstructionCounter < len(instructions):
            currInstr = instructions[currentInstructionCounter]
            currentInstructionCounter += 1
            totalInstructionsInterpreted += 1
            if currInstr.opcode == "MOVE":
                checkInstrArgs(currInstr)
                self.instrMove(currInstr)
            if currInstr.opcode == "CREATEFRAME":
                checkInstrArgs(currInstr)
                self.instrCreateframe(currInstr)
            if currInstr.opcode == "PUSHFRAME":
                checkInstrArgs(currInstr)
                self.instrPushframe(currInstr)
            if currInstr.opcode == "POPFRAME":
                checkInstrArgs(currInstr)
                self.instrPopframe(currInstr)
            if currInstr.opcode == "DEFVAR":
                checkInstrArgs(currInstr)
                self.instrDefvar(currInstr)
            if currInstr.opcode == "CALL":
                checkInstrArgs(currInstr)
                currentInstructionCounter = self.getPositionOfInstrOrder(self.instrCall(currInstr, currentInstructionCounter-1))
            if currInstr.opcode == "RETURN":
                checkInstrArgs(currInstr)
                if self.callStack.isEmpty():
                    sys.stderr.write("RUN ERROR: Empty call stack in instruction no. " +    str(currInstr.order)+"\n")
                    sys.exit(56)
                instrToJumpTo = self.callStack.pop()
                currentInstructionCounter = instrToJumpTo
            if currInstr.opcode == "PUSHS":
                checkInstrArgs(currInstr)
                self.instrPushs(currInstr)
            if currInstr.opcode == "POPS":
                checkInstrArgs(currInstr)
                self.instrPops(currInstr)
            if currInstr.opcode == "ADD":
                checkInstrArgs(currInstr)
                self.instrArithmetics(currInstr)
            if currInstr.opcode == "SUB":
                checkInstrArgs(currInstr)
                self.instrArithmetics(currInstr)
            if currInstr.opcode == "MUL":
                checkInstrArgs(currInstr)
                self.instrArithmetics(currInstr)
            if currInstr.opcode == "IDIV":
                checkInstrArgs(currInstr)
                self.instrArithmetics(currInstr)
            if currInstr.opcode == "LT":
                checkInstrArgs(currInstr)
                self.instrRelation(currInstr)
            if currInstr.opcode == "GT":
                checkInstrArgs(currInstr)
                self.instrRelation(currInstr)
            if currInstr.opcode == "EQ":
                checkInstrArgs(currInstr)
                self.instrRelation(currInstr)
            if currInstr.opcode == "AND":
                checkInstrArgs(currInstr)
                self.instrBool(currInstr)
            if currInstr.opcode == "OR":
                checkInstrArgs(currInstr)
                self.instrBool(currInstr)
            if currInstr.opcode == "NOT":
                checkInstrArgs(currInstr)
                self.instrBoolNot(currInstr)
            if currInstr.opcode == "INT2CHAR":
                checkInstrArgs(currInstr)
                self.instrInt2Char(currInstr)
            if currInstr.opcode == "STRI2INT":
                checkInstrArgs(currInstr)
                self.instrStri2Int(currInstr)
            if currInstr.opcode == "READ":
                checkInstrArgs(currInstr)
                self.instrRead(currInstr)
            if currInstr.opcode == "WRITE":
                checkInstrArgs(currInstr)
                self.instrWrite(currInstr)
            if currInstr.opcode == "CONCAT":
                checkInstrArgs(currInstr)
                self.instrConcat(currInstr)
            if currInstr.opcode == "STRLEN":
                checkInstrArgs(currInstr)
                self.instrStrlen(currInstr)
            if currInstr.opcode == "GETCHAR":
                checkInstrArgs(currInstr)
                self.instrGetchar(currInstr)
            if currInstr.opcode == "SETCHAR":
                checkInstrArgs(currInstr)
                self.instrSetchar(currInstr)
            if currInstr.opcode == "TYPE":
                checkInstrArgs(currInstr)
                self.instrType(currInstr)
            if currInstr.opcode == "JUMP":
                checkInstrArgs(currInstr)
                labelOrder = self.instrJump(currInstr)
                instrToJumpTo = self.getPositionOfInstrOrder(labelOrder)
                currentInstructionCounter = instrToJumpTo
            if currInstr.opcode == "JUMPIFEQ" or currInstr.opcode == "JUMPIFNEQ":
                checkInstrArgs(currInstr)
                toJump = self.instrJumpIf(currInstr)
                if toJump != -1:
                    instrToJumpTo = self.getPositionOfInstrOrder(toJump)
                    currentInstructionCounter = instrToJumpTo
            if currInstr.opcode == "EXIT":
                checkInstrArgs(currInstr)
                self.instrExit(currInstr)
            if currInstr.opcode == "DPRINT":
                checkInstrArgs(currInstr)
                self.instrDprint(currInstr)
            if currInstr.opcode == "BREAK":
                checkInstrArgs(currInstr)
                sys.stderr.write("----BREAK INTERPRET STATE----"+"\n")
                sys.stderr.write("Current instruction opcode:" +currInstr.opcode+", order:"+currInstr.order+"\n")
                sys.stderr.write("Total number of instructions interpreted:"+totalInstructionsInterpreted+"\n")
                sys.stderr.write("Local frame:"+"\n")
                self.printFrame(self.localFrame)
                sys.stderr.write("Temp frame:"+"\n")
                self.printFrame(self.tempFrame)
                sys.stderr.write(
                    "Global frame:"+totalInstructionsInterpreted+"\n")
                self.printFrame(self.globalFrame)
        self.inputFile.close()

################################################
####                MAIN                    ####
################################################


# List of all the labels created
labels = []
argSource, argInput = parseArgs()
instructions = parseXML(argSource)
program = Program(argInput)
program.executeProgram(instructions)
