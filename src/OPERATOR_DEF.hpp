DEF_OPER(ADD, '+', {
    fprintf(output, "\"%s\", shape=\"circle\", color=\"grey\", thickness=4, fillcolor=\"aquamarine1\"", "+");
    }, 2)

DEF_OPER(SUB, '-', {
    fprintf(output, "\"%s\", shape=\"circle\", color=\"grey\", thickness=4, fillcolor=\"aquamarine1\"", "-");
    }, 2)

DEF_OPER(MUL, '*', {
    fprintf(output, "\"%s\", shape=\"circle\", color=\"grey\", thickness=4, fillcolor=\"aquamarine1\"", "*");
    }, 2)

DEF_OPER(DIV, '/', {
    fprintf(output, "\"%s\", shape=\"circle\", color=\"grey\", thickness=4, fillcolor=\"aquamarine1\"", "/");
    }, 2)

DEF_OPER(POW, '^', {
    fprintf(output, "\"%s\", shape=\"circle\", color=\"grey\", thickness=4, fillcolor=\"aquamarine1\"", "^");
    }, 2)


DEF_OPER(SIN, 'sin', {
    fprintf(output, "\"%s\", shape=\"circle\", color=\"grey\", thickness=4, fillcolor=\"aquamarine1\"","sin");
    }, 1)

DEF_OPER(COS, 'cos', {
    fprintf(output, "\"%s\", shape=\"circle\", color=\"grey\", thickness=4, fillcolor=\"aquamarine1\"","cos");
    }, 1)

DEF_OPER(TAN, 'tan', {
    fprintf(output, "\"%s\", shape=\"circle\", color=\"grey\", thickness=4, fillcolor=\"aquamarine1\"","tan");
    }, 1)

DEF_OPER(COT, 'cot', {
    fprintf(output, "\"%s\", shape=\"circle\", color=\"grey\", thickness=4, fillcolor=\"aquamarine1\"","cot");
    }, 1)

DEF_OPER(LOG, 'ln', {
    fprintf(output, "\"%s\", shape=\"circle\", color=\"grey\", thickness=4, fillcolor=\"aquamarine1\"","ln");
    }, 1)



