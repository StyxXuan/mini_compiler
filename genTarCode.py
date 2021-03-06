import os
import sys
import argparse

parser = argparse.ArgumentParser()
parser.add_argument('-s', required=True, help='path to src')
parser.add_argument('-d', required=True, help='path to dst')
args = parser.parse_args()

map = {}
globalVarList = []
file_path = args.s
out_path = args.d
line_num = 0
sp = 0
codes = []
temp_reg = 0
reg_a = 0
global_def = True
label_add = {}



def is_number(s):
    try:
        float(s)
        return True
    except ValueError:
        pass
    return False

def applyReg():
    global temp_reg
    temp_reg += 1
    return "$t"+str(temp_reg-1)

def inputReg():
    global reg_a
    return reg_a

def releaseReg():
    global temp_reg
    temp_reg = 0

def release_inputReg():
    global reg_a
    reg_a = 0


def is_global(var_name):
    if var_name in globalVarList:
        return True

    return False

def get_var_value(var_name):
    global sp
    if var_name[0] == "#":
        return "$a"+var_name[1]

    reg_name = applyReg()

    if '*' in var_name:
        var_name = var_name.replace('*', '')
        add_reg = get_var_value(var_name)
        codes.append("lw "+reg_name+",0("+add_reg+")")
        return reg_name

    if var_name in globalVarList:
        if '&' in var_name:
            var_name = var_name.replace('&', '')
            codes.append("la "+reg_name+","+var_name)
        else:
            codes.append("lw "+reg_name+","+var_name)
        return reg_name

    # if 

    if not '&' in var_name:
        add_var(var_name)
        codes.append("addi $at,$sp,"+str(map[var_name]))
        codes.append("lw "+reg_name+",0($at)")
    else:
        var_name = var_name.replace('&', '')
        codes.append("addi "+reg_name+",$sp,"+str(map[var_name]))
        
    return reg_name

def debug_print(regName):
    codes.append("move $a0,"+regName)
    codes.append("li $v0,1")
    codes.append("syscall")
    codes.append("la $a0,LB")
    codes.append("li $v0,4")
    codes.append("syscall")

def save_var_value(var_name, value_reg):
    global sp
    if '*' in var_name:
        var_name = var_name.replace('*', '')
        res_reg = get_var_value(var_name)
        codes.append("sw "+value_reg+",0("+res_reg+")")
        return

    if var_name in globalVarList:
        codes.append("la $at,"+var_name)
        codes.append("sw "+value_reg+",0($at)")
        return
    
    elif var_name not in map:
        add_var(var_name)

    codes.append("addi $at,$sp,"+str(map[var_name]))
    codes.append("sw "+value_reg+",0($at)")

def add_var(var_name, data_size=4):
    global sp
    # print(map)
    if var_name not in map:
        map[var_name] = sp
        # print(map)
        sp = sp + data_size


def genTarCodeBinExp(tokens):
    if is_number(tokens[2]):
        codes.append("li $s0,"+str(tokens[2]))
        save_var_value(tokens[0], "$s0")        
    else:
        reg1 = get_var_value(tokens[2])
        save_var_value(tokens[0], reg1)   
    

def genTarCodeTriExp(tokens):
    # reg_des = get_var_reg(tokens[0])
    reg_res = applyReg()
    op = tokens[3]
    reg1 = get_var_value(tokens[2])
    reg2 = get_var_value(tokens[4])
    if op == "&&":
        codes.append("and " + reg_res +","+reg1+","+reg2)
    elif op == "||":
        codes.append("or " + reg_res +","+reg1+","+reg2)
    elif op == "+":
        codes.append("add " + reg_res +","+reg1+","+reg2)
    elif op == "-":
        codes.append("sub " + reg_res +","+reg1+","+reg2)
    elif op == "*":
        codes.append("mult " + reg1+ "," + reg2)
        codes.append("mflo " + reg_res)
        label_add[tokens[1]] = line_num
    elif op == "/":
        codes.append("div " + reg1+ "," + reg2)
        codes.append("mflo " + reg_res)
        label_add[tokens[1]] = line_num
    elif op == "%":
        codes.append("div " + reg1+ "," + reg2)
        codes.append("mfhi " + reg_res)
        label_add[tokens[1]] = line_num
    elif op == "<":
        codes.append("slt " + reg_res +","+reg1+","+reg2)
    elif op == ">":
        codes.append("slt " + reg_res +","+reg2+","+reg1)
    elif op == "<=":
        codes.append("sle " + reg_res +","+reg2+","+reg1)
    elif op == ">=":
        codes.append("sge " + reg_res +","+reg1+","+reg2)
    elif op == "==":
        codes.append("seq " + reg_res +","+reg1+","+reg2)

    save_var_value(tokens[0], reg_res)

def genTarCodeCallExp(tokens):
    # print(tokens)
    if(tokens[2] == "-"):
        reg1 = get_var_value(tokens[2])
        reg_res = applyReg()
        codes.append("sub " + reg_res +",$zero,"+reg1)
        save_var_value(tokens[0], reg_res)
        return

    release_inputReg()
    save_var_value("temp_ra", "$ra")
    codes.append("jal " + tokens[3])
    reg_res = applyReg()
    codes.append("move " + reg_res +",$v0")
    codes.append("addi $sp,$sp,80")
    save_var_value(tokens[0],reg_res)
    for i in range(3):
        reg_temp = get_var_value("temp_a"+str(i))
        codes.append("move $a"+str(i)+","+reg_temp)
    reg_link = get_var_value("temp_ra")
    codes.append("move $ra,"+reg_link)
    
    

def genTarCode(tokens):
    global global_def
    global line_num
    global sp
    global reg_a
    releaseReg()
    # print(tokens)
    if tokens[0] == 'var:':
        release_inputReg()
        # print("var:   " +  tokens[1])
        add_var(tokens[1])
    
    elif tokens[0] == "Array":
        release_inputReg()
        # print("var:   " +  tokens[1])
        dsize = int(tokens[2])*4
        add_var(tokens[1],data_size=dsize)

    elif tokens[0] == "Func:":
        release_inputReg()
        codes.append(tokens[1]+":")
        codes.append("subi $sp,$sp,80")
        for i in range(3):
            save_var_value("temp_a"+str(i), "$a"+str(i))

    elif tokens[0] == "RETURN":
        release_inputReg()
        reg_res = get_var_value(tokens[1])
        codes.append("move $v0,"+reg_res)
        codes.append("jr $ra")
        release_inputReg()

    elif tokens[0] == "IF_NOT":
        release_inputReg()
        reg_res = get_var_value(tokens[1])
        codes.append("beq "+reg_res+",$zero "+tokens[3])

    elif tokens[0] == "Label":
        release_inputReg()
        codes.append(tokens[1]+":")

    elif tokens[0] == "GOTO":
        release_inputReg()
        codes.append("j " + tokens[1])

    elif tokens[0] == "ARG":
        reg_1 = get_var_value(tokens[1])
        a = "$a" + str(inputReg())
        codes.append("move " + a + ","+ reg_1)
        reg_a += 1
    elif tokens[0] == "Print":
        reg_1 = get_var_value(tokens[1])
        codes.append("move $a0,"+reg_1)
        codes.append("li $v0,1")
        codes.append("syscall")

        codes.append("la $a0,LB")
        codes.append("li $v0,4")
        codes.append("syscall")
        reg_temp = get_var_value("temp_a0")
        codes.append("move $a0,"+reg_temp)
    else:
        if len(tokens) == 3:
            genTarCodeBinExp(tokens)
        elif len(tokens) == 5:
            genTarCodeTriExp(tokens)
        else:
            genTarCodeCallExp(tokens)



def add_global_var(tokens):
    codes.append(tokens[1]+":   "+".word")
    globalVarList.append(tokens[1])

with open(file_path, 'r') as f:
    codes.append(".data")
    codes.append("LB: .asciiz \"\\n\"")
    for line in f:
        line = line.strip()
        tokens = line.split(" ")
        if '' in tokens:
            tokens.remove('')
        # print(tokens)
        if tokens[0] == "global:":
            add_global_var(tokens)
        else:
            if global_def:
                global_def = False
                codes.append(".text")
                codes.append("j main")

            genTarCode(tokens)

f.close()
fo = open(out_path, "w")

for code in codes:
    fo.write(code+"\n")
fo.close()