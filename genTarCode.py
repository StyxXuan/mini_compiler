import os

map = {}
is_global = True
file_path = "./src/interCode.txt"
line_num = 0
sp = 0
codes = []
temp_reg = 0
reg_a = 0

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
    return temp_reg-1

def inputReg():
    global reg_a
    reg_a += 1
    return reg_a-1

def releaseReg():
    global temp_reg
    temp_reg = 0

def get_var_add(var_name):
    global sp
    if var_name not in map:
        map[var_name] = sp
        sp = sp - 4
    
    return map[var_name]

def get_var_reg(var_name):
    if var_name[0] == "#":
        return "a"+var_name[1]
    
    add = get_var_add(var_name)
    bias = add - sp
    reg_1 = "$t"+str(applyReg())
    codes.append("lw "+reg_1+","+str(bias)+"($sp)")
    return reg_1

def save_var_reg(var_name, reg_name):
    add = get_var_add(var_name)
    bias = add-sp
    codes.append("sw "+reg_name+","+str(bias)+"($sp)")

def genTarCodeBinExp(tokens):
    # reg1 = get_var_reg(tokens[0])
    if is_number(tokens[2]):
        reg1 = "$t"+str(applyReg())
        codes.append("addi "+reg1+",$zero,"+str(tokens[2]))
        save_var_reg(tokens[0], reg1)
    else:
        reg1 = "$t"+str(applyReg())
        reg2 = get_var_reg(tokens[2])
        codes.append("add "+reg1+",$zero,"+reg2)
        save_var_reg(tokens[0], reg1)
    

def genTarCodeTriExp(tokens):
    # reg_des = get_var_reg(tokens[0])
    reg_des = "$t"+str(applyReg())
    op = tokens[3]
    reg1 = get_var_reg(tokens[2])
    reg2 = get_var_reg(tokens[4])
    if op == "&&":
        codes.append("and " + reg_des +","+reg1+","+reg2)
    elif op == "||":
        codes.append("or " + reg_des +","+reg1+","+reg2)
    elif op == "+":
        codes.append("add " + reg_des +","+reg1+","+reg2)
    elif op == "-":
        codes.append("sub " + reg_des +","+reg1+","+reg2)
    elif op == "*":
        codes.append("mult " + reg1+ "," + reg2)
        codes.append("mflo " + reg_des)
        label_add[tokens[1]] = line_num
    elif op == "/":
        codes.append("div " + reg1+ "," + reg2)
        codes.append("mflo " + reg_des)
        label_add[tokens[1]] = line_num
    elif op == "<":
        codes.append("slt " + reg_des +","+reg1+","+reg2)
    elif op == ">":
        codes.append("slt " + reg_des +","+reg2+","+reg1)
    elif op == "<=":
        codes.append("slt " + reg_des +","+reg1+","+reg2)
        codes.append("xori " + reg_des +","+reg_des+",1")
    elif op == ">=":
        codes.append("slt " + reg_des +","+reg2+","+reg1)
        codes.append("xori " + reg_des +","+reg_des+",1")


    label_add[tokens[1]] = line_num
    save_var_reg(tokens[0], reg_des)

def genTarCodeCallExp(tokens):
    codes.append("j " + tokens[3])
    reg_des = "$t"+str(applyReg())
    codes.append("add " + reg_des +",$v0"+",$zero")

def genTarCode(tokens):
    global line_num
    global sp
    releaseReg()
    if tokens[0] == "var:":
        get_var_add(tokens[1])

    elif tokens[0] == "Func:":
        codes.append(tokens[1]+":")
        label_add[tokens[1]] = line_num
        line_num += 1
    
    elif tokens[0] == "RETURN":
        bias = get_var_add(tokens[0]) - sp
        codes.append("lw $t0,"+str(bias)+"($sp)")
        codes.append("move $v0,$t0")
        codes.append("jr $ra")
        line_num += 3

    elif tokens[0] == "IF_NOT":
        reg1 = get_var_reg(tokens[1])
        codes.append("beq $"+reg1+",$zero "+tokens[3])

    elif tokens[0] == "Label":
        codes.append(tokens[1]+":")
        label_add[tokens[1]] = line_num
        line_num += 1

    elif tokens[0] == "GOTO":
        codes.append("j " + tokens[1])
        line_num += 1

    elif tokens[0] == "ARG":
        reg_1 = get_var_reg(tokens[1])
        a = "a" + str(inputReg())
        codes.append("move " + a + ","+ reg_1)
        line_num += 1
    else:
        if len(tokens) == 3:
            genTarCodeBinExp(tokens)
        elif len(tokens) == 5:
            genTarCodeTriExp(tokens)
        else:
            genTarCodeCallExp(tokens)





with open(file_path, 'r') as f:
    for line in f:
        line = line.strip()
        tokens = line.split(" ")
        genTarCode(tokens)

    for code in codes:
        print(code)